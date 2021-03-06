/*
 * DSA disk class driver
 * drives RA-class disks
 * hooked up through an MSCP port
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/udaioc.h"
#include "sys/diskio.h"
#include "sys/ra.h"
#include "sys/mscp.h"
#include "sys/user.h"
#include "sys/file.h"
#include "sys/conf.h"

extern struct msaddr raaddr[];
extern struct radisk radisk[];
extern int racnt;
static long rarefno;			/* ref seq num */
extern struct buf rabuf[];
static struct buf rctbuf;		/* for reading replacement table */

static rareplace(), raonline(), rasonl(), racinit();

int raopen(), raread(), rawrite(), raioctl(), rastrategy(), raclose();
struct bdevsw rabdev = bdinit(raopen, raclose, rastrategy, 0);
struct cdevsw racdev = cdinit(raopen, raclose, raread, rawrite, raioctl);

/*
 * UNIT(d) == logical unit number;
 * the physical unit number is in raaddr[UNIT(d)].unit
 *
 * 0100 in the minor device is (temporarily?)
 * usurped to indicate bitmapped file systems
 * quietly ignore it for now
 * when things improve, change the UNIT mask to 037
 */
#define	UNIT(dev)	(((dev)>>3) & 027)
#define	PART(dev)	((dev)&07)
#define	HUGE	0x7fffffff	/* very large 32-bit number */

/*
 * default partition sizes
 */
static struct size {
	daddr_t	nblocks;
	daddr_t	blkoff;
} ra_sizes[NRAPART] = {
	10240,	0,
	20480,	10240,
	249848,	30720,
	249848,	280568,
	249848,	530416,
	HUGE,	780264,
	749544,	30720,		/* slices 2,3,4 */
	HUGE,	0,		/* whole drive */
};

/*
 * reused bits of buf/iobuf struct
 */

#define	b_next	av_forw		/* next buffer in queue */
#define	b_pkt	av_back		/* pointer to mscp command */
#define	b_crf	b_resid		/* saved refno for pending command */

/*
 * flags in radisk.flags
 */

#define	ONLINE	01		/* drive is online */
#define	WONLINE	02		/* waiting for online */
#define	GOTDI	04		/* got unit info */
#define	RPLOCK	010		/* replacement in progress */
#define	RPWANT	020		/* want RPLOCK */
#define	RPDONE	040		/* replacement done */
#define	SPDOWN	0100		/* spin down on last close */

/*
 * random numbers
 */

#define	PRIONL	(PZERO-1)
#define	SECTOR	512		/* size of an MSCP sector */
#define	IDRA	0		/* connection ID for MSCP */

/*
 * open a drive, if necessary
 */

int raseql(), radg();

raopen(dev, flag)
dev_t dev;
{
	register int unit;
	register struct radisk *ra;
	register struct msaddr *rp;
	register int part;

	unit = UNIT(dev);
	if (unit >= racnt) {
		u.u_error = ENXIO;
		return;
	}
	ra = &radisk[unit];
	rp = &raaddr[unit];
	if (ra->open == 0) {
		if (rp->ctype < 0 || rp->ctype >= nmsport
		||  (ra->port = msportsw[rp->ctype]) == NULL) {
			u.u_error = ENXIO;
			return;
		}
		ra->di.radsize = HUGE;
		if ((*ra->port->mp_init)(rp->ctl, rp->ctype, 0, IDRA, raseql, radg) == 0) {
			u.u_error = ENXIO;
			return;
		}
		racinit(ra, rp);
	}
	part = PART(dev);
	if ((ra->pinit & (1<<part)) == 0) {
		ra->nblocks[part] = ra_sizes[part].nblocks;
		ra->blkoff[part] = ra_sizes[part].blkoff;
		ra->pinit |= (1<<part);
	}
	ra->open |= 1<<part;
	spl6();
	if ((ra->flags & ONLINE) == 0)
		raonline(ra, rp);
	spl0();
	if ((ra->flags & ONLINE) == 0)
		u.u_error = ENXIO;
}

raclose(dev)
{
	register struct radisk *ra;
	register struct msaddr *rp;
	register struct mscmd *mp;

	ra = &radisk[UNIT(dev)];
	rp = &raaddr[UNIT(dev)];
	ra->open &=~ (1<<PART(dev));
	if (ra->open || (ra->flags & ONLINE) == 0)
		return;
	mp = (*ra->port->mp_get)(rp->ctl);
	mp->m_crf = ++rarefno;
	mp->m_unit = rp->unit;
	mp->m_opcd = OPAVL;		/* put it offline */
	if ((ra->flags & SPDOWN) == 0)
		mp->m_mod = 0;
	else {
		mp->m_mod = MDSPD;
		ra->flags &=~ SPDOWN;
	}
	mp->m_unfl = 0;
	mp->m_dvpm = 0;	/* ? */
	ra->flags &=~ ONLINE;
	(*ra->port->mp_send)(rp->ctl, IDRA, mp);
}

int rastrategy();

raread(dev)
{
	physio(rastrategy, &rabuf[UNIT(dev)], dev, B_READ, minphys);
}

rawrite(dev)
{
	physio(rastrategy, &rabuf[UNIT(dev)], dev, B_WRITE, minphys);
}

/*
 * strategy routine;
 * used as strategy by all port drivers
 * send the packet right away
 */

rastrategy(bp)
register struct buf *bp;
{
	register struct radisk *ra;
	register struct mscmd *mp;
	register int unit;
	register int part;
	register struct msaddr *rp;
	int count;
	daddr_t limit;

	unit = UNIT(minor(bp->b_dev));
	part = PART(minor(bp->b_dev));
	ra = &radisk[unit];
	rp = &raaddr[unit];
	limit = ra->di.radsize - ra->blkoff[part];
	if (limit > ra->nblocks[part])
		limit = ra->nblocks[part];
	if (bp->b_blkno >= limit && bp != &rctbuf) {
		if (bp->b_blkno == ra->nblocks[part])
			bp->b_resid = bp->b_bcount;
		else {
			bp->b_error = ENOSPC;
			bp->b_flags |= B_ERROR;
		}
		iodone(bp);
		return;
	}
	count = bp->b_bcount;
	if (count/SECTOR + bp->b_blkno > limit && bp != &rctbuf)
		count = (limit - bp->b_blkno) * SECTOR;
	spl6();
	if ((ra->flags & ONLINE) == 0 && raonline(ra, rp) == 0) {
		bp->b_flags |= B_ERROR;
		iodone(bp);
		spl0();
		return;
	}
	mp = (*ra->port->mp_get)(rp->ctl);
	mp->m_crf = ++rarefno;
	mp->m_unit = rp->unit;
	mp->m_opcd = (bp->b_flags & B_READ) ? OPRD : OPWR;
	mp->m_mod = 0;
	mp->m_bcnt = count;
	mp->m_lbn = bp->b_blkno + ra->blkoff[part];
	(*ra->port->mp_map)(rp->ctl, mp, bp);
	bp->b_pkt = (struct buf *)mp;
	bp->b_crf = mp->m_crf;
	bp->b_next = NULL;
	if (ra->actf)
		ra->actl->b_next = bp;
	else
		ra->actf = bp;
	ra->actl = bp;
	(*ra->port->mp_send)(rp->ctl, IDRA, mp);
	spl0();
}

/*
 * ioctl
 * principally for bad block replacement
 */

raioctl(dev, cmd, addr, flag)
dev_t dev;
caddr_t addr;
{
	register union arg {
		struct ud_rctbuf r;
		struct ud_repl b;
	} *uap;
	register struct radisk *ra;
	register int i;
	long parts[2];

	ra = &radisk[UNIT(dev)];
	if ((ra->flags & (ONLINE|GOTDI)) != (ONLINE|GOTDI)) {
		/* should bring it online here */
		u.u_error = EIO;
		return;
	}
	uap = (union arg *)addr;
	switch(cmd) {
	default:
		u.u_error = ENOTTY;
		return;

	case DIOSSIZ:
		if ((flag & FWRITE) == 0) {
			u.u_error = EBADF;
			return;
		}
		if (copyin(addr, (caddr_t)parts, sizeof(parts)) < 0) {
			u.u_error = EFAULT;
			return;
		}
		ra->blkoff[PART(dev)] = parts[0];
		ra->nblocks[PART(dev)] = parts[1];
		return;

	case DIOGSIZ:
		parts[0] = ra->blkoff[PART(dev)];
		parts[1] = ra->nblocks[PART(dev)];
		if (copyout((caddr_t)parts, addr, sizeof(parts)) < 0)
			u.u_error = EFAULT;
		return;

	case UIOCHAR:
		if (copyout((caddr_t)&ra->di, addr, sizeof(struct ud_unit)))
			u.u_error = EFAULT;
		return;

	case UIORRCT:
		if(uap->r.lbn < 0 || uap->r.lbn > ra->di.rctsize) {
			u.u_error = EIO;
			return;
		}
		/*
		 * try different copies until one works
		 */
		for (i = 0; i < ra->di.copies; i++) {
			u.u_count = SECTOR;	/* block size on disk */
			u.u_offset = ltoL((uap->r.lbn + ra->di.radsize) * SECTOR);
			u.u_offset = Lladd(u.u_offset,i * ra->di.rctsize * SECTOR);
			u.u_base = uap->r.buf;
			u.u_segflg = SEGUDATA;
			u.u_error = 0;
			physio(rastrategy, &rctbuf, dev, B_READ, minphys);
			if (u.u_error == 0)
				break;
		}
		return;

	case UIOWRCT:
		if ((flag & FWRITE) == 0) {
			u.u_error = EBADF;
			return;
		}
		if(uap->r.lbn < 0 || uap->r.lbn > ra->di.rctsize) {
			u.u_error = EIO;
			return;
		}
		/*
		 * write every copy we can
		 * should do read-after-write
		 */
		for (i = 0; i < ra->di.copies; i++) {
			u.u_count = SECTOR;	/* block size on disk */
			u.u_offset = ltoL((uap->r.lbn + ra->di.radsize) * SECTOR);
			u.u_offset = Lladd(u.u_offset, i * ra->di.rctsize * SECTOR);
			u.u_base = uap->r.buf;
			u.u_segflg = SEGUDATA;
			physio(rastrategy, &rctbuf, dev, B_WRITE, minphys);
			u.u_error = 0;
		}
		return;

	case UIOREPL:
		if ((flag & FWRITE) == 0) {
			u.u_error = EBADF;
			return;
		}
		rareplace(dev, uap->b.lbn, uap->b.replbn, uap->b.prim);
		return;

	case UIOSPDW:
		ra->flags |= SPDOWN;
		return;

	case UIORST:
		(*ra->port->mp_init)(raaddr[UNIT(dev)].ctl, raaddr[UNIT(dev)].ctype, 1, IDRA, raseql, radg);
		return;
	}

}

static
rareplace(dev, badlbn, replbn, prim)
int dev;
daddr_t badlbn;
daddr_t replbn;
int prim;
{
	register struct mscmd *mp;
	register struct radisk *ra;
	register struct msaddr *rp;
	register int unit;

	unit = UNIT(minor(dev));
	ra = &radisk[unit];
	rp = &raaddr[unit];
	spl6();
	while (ra->flags & RPLOCK) {
		ra->flags |= RPWANT;
		sleep((caddr_t)&ra->flags, PZERO + 1);
	}
	ra->flags |= RPLOCK;
	printf("ra%d replace %D with %D\n", unit, badlbn, replbn);
	mp = (*ra->port->mp_get)(rp->ctl);
	bzero((caddr_t)mp, sizeof(struct mscmd)); /* clear reserved fields */
	mp->m_crf = ++rarefno;
	mp->m_unit = rp->unit;
	mp->m_opcd = OPRPL;
	mp->m_rbn = replbn;
	mp->m_lbn = badlbn;
	mp->m_mod = prim ? MDPRI : 0;
	ra->cmdcrf = mp->m_crf;
	ra->cmdopc = OPRPL;
	(*ra->port->mp_send)(rp->ctl, IDRA, mp);
	while ((ra->flags & RPDONE) == 0)
		sleep((caddr_t)&ra->rplret, PZERO);
	u.u_error = ra->rplret;
	if (ra->flags & RPWANT)
		wakeup((caddr_t)&ra->flags);
	ra->flags &=~ (RPWANT|RPLOCK|RPDONE);
	spl0();

}

/*
 * here when the port gets a sequential message
 */
raseql(ctl, type, ep)
int ctl, type;
register struct msend *ep;
{
	register struct buf *bp;
	register struct radisk *ra;
	register int unit;
	register struct buf *obp;
	int sts;

	if (ep->m_opcd == 0 && ep->m_sts == STRST) {
		rareset(ctl);
		return;
	}
	for (unit = 0; unit < racnt; unit++)
		if (raaddr[unit].ctl == ctl
		&&  raaddr[unit].ctype == type
		&&  raaddr[unit].unit == ep->m_unit)
			break;
	if (unit >= racnt) {
		printf("ra%d ctl%d typ%d: stray mscp packet sts x%x opcode %o\n",
			ep->m_unit, ctl, type, ep->m_sts, ep->m_opcd);
		return;
	}
	ra = &radisk[unit];
	sts = ep->m_sts & STMSK;
	if (sts == STAVL || sts == STOFL)
		ra->flags &=~ ONLINE;	/* help! */
illcmd:
	switch (ep->m_opcd & 0377) {
	case OPEND:		/* eg invalid command */
		if (ep->m_crf == ra->cmdcrf) {
			ep->m_opcd = ra->cmdopc | OPEND;
			goto illcmd;
		}
		/* else check for pending read or write */
	case OPRD|OPEND:
	case OPWR|OPEND:
		for (bp = ra->actf, obp = NULL; bp; obp = bp, bp = bp->b_next)
			if (ep->m_crf == bp->b_crf)
				break;
		if (bp == NULL) {
			printf("ra%d stray end: crf %d sts x%x opcode 0%o\n",
				unit, ep->m_crf, ep->m_sts, ep->m_opcd & 0377);
			return;
		}
		if (((struct mscmd *)(bp->b_pkt))->m_crf != ep->m_crf)
			printf("ra%d sent %d got %d crf; flg %x dev %x\n",
				unit, ((struct mscmd *)(bp->b_pkt))->m_crf,
				ep->m_crf, bp->b_flags, bp->b_dev);
		if (obp)
			obp->b_next = bp->b_next;
		else
			ra->actf = bp->b_next;
		if (bp == ra->actl)
			ra->actl = obp;
		bp->b_resid = bp->b_bcount - ep->m_bcnt;
		if (sts != STSUC) {
			bp->b_flags |= B_ERROR;
			if (ep->m_sts == STBCK || ep->m_sts == STBK2)	/* optical disk blank check */
				bp->b_error = ENXIO;
			else
				printf("err on ra%d block %D: sts x%x\n", unit, bp->b_blkno, ep->m_sts);
		}
		(*ra->port->mp_unmap)(ctl, (struct mscmd *)bp->b_pkt);
		iodone(bp);
		return;

	case OPONL|OPEND:
		rasonl(ra, ep);
		return;

	case OPAVL|OPEND:
		ra->flags &=~ ONLINE;
		return;

	case OPGUS|OPEND:
		if (sts != STSUC) {
			printf("ra%d: can't get unit sts x%x\n", unit, ep->m_sts);
			return;
		}
		ra->di.medium = ep->m_medi;
		ra->di.tracksz = ep->m_trck;
		ra->di.groupsz = ep->m_grp;
		ra->di.cylsz = ep->m_cyl;
		ra->di.rctsize = ep->m_rcts;
		ra->di.rbns = ep->m_rbns;
		ra->di.copies = ep->m_rctc;
		ra->flags |= GOTDI;
		return;

	case OPSCC|OPEND:
		if (sts != STSUC)
			printf("ra ctl%d typ%d: bad init\n", ctl, type);
		return;

	case OPRPL|OPEND:
		ra->rplret = 0;
		if (sts != STSUC) {
			printf("ra%d: rpl sts x%x\n", unit, ep->m_sts);
			ra->rplret = EIO;
		}
		ra->flags |= RPDONE;
		wakeup((caddr_t)&ra->rplret);
		return;

	default:
		printf("ra%d ctl%d typ%d: stray mscp msg opcd 0%o sts x%x\n",
			ep->m_unit, ctl, type, ep->m_opcd&0377, ep->m_sts);
		return;
	}
}

/*
 * controller was reset
 * discard all pending io,
 * awake all sleepers,
 * mark everything offline
 */

rareset(ctl)
int ctl;
{
	register int unit;
	register struct radisk *ra;
	register struct buf *bp, *nbp;

	for (unit = 0; unit < racnt; unit++)  {
		if (raaddr[unit].ctl != ctl)
			continue;
		ra = &radisk[unit];
		for (bp = ra->actf; bp; bp = nbp) {
			nbp = bp->b_next;
			(*ra->port->mp_unmap)(ctl, (struct mscmd *)bp->b_pkt);
			bp->b_flags |= B_ERROR;
			iodone(bp);
		}
		ra->actf = ra->actl = NULL;
		ra->flags &=~ (ONLINE|WONLINE);
		wakeup((caddr_t)ra);
	}
}

/*
 * here with a datagram message
 * explanations really shouldn't be in the driver
 *
 * the hack for event 8 ignores error packets from
 * the US Design optical disk controller
 * that really mean `read a blank spot on the disk
 */

static char *raevents[] = {
	"ok",
	"inv cmd",
	"op aborted",
	"offline",
	"available",
	"med fmt",
	"write prot",
	"comp err",
	"data err",
	"host buf access err",
	"cntl err",
	"drive err",
};
#define	MAXEVT	0xb

radg(ctl, type, ep)
int ctl, type;
register struct mserl *ep;
{
	register u_short *sp;		/* for sdi crap */
	register int i;			/* for useless design crap */
	register unsigned char *cp;	/* for useless design crap */

	if (ep->l_fmt == FMDSK && ep->l_evnt == STBCK && ep->l_flgs == 0)
		return;
	printf("ra%d ctl%d typ%d seq %d: %s err; fmt x%x ev x%x fl x%x\n",
		ep->l_unit, ctl, type, ep->l_seq,	/* phys unit, not log */
		ep->l_flgs&(LFSUC|LFCON) ? "soft" : "hard",
		ep->l_fmt, ep->l_evnt, ep->l_flgs&0377);
	if ((ep->l_evnt & STMSK) <= MAXEVT)
		printf("%s; ", raevents[ep->l_evnt & STMSK]);
	switch (ep->l_fmt) {
	case FMCNT:
		/* now the thing should be marked disastrously bad */
		printf("oops\n");
		break;

	case FMBAD:
		printf("host mem access; addr x%x\n", ep->l_badr);
		break;

	case FMDSK:
		printf("%sbn %d; lev x%x, retry x%x\n",
			(ep->l_hdcd & 0xf0000000) == 0 ? "l" : "r",
			ep->l_hdcd & 0x0fffffff, ep->l_lvl, ep->l_rtry);
		break;

	case FMSDI:
		printf("%sbn %d;",
			(ep->l_hdcd & 0xf0000000) == 0 ? "l" : "r",
			ep->l_hdcd & 0x0fffffff);
		/*
		 * print the bytes in the same order used
		 * by the dec diagnostics
		 */
		sp = (u_short *)&ep->l_sdi[3];
		while (sp > (u_short *)ep->l_sdi)
			printf(" %x", *--sp);
		printf(" xx\n"); 
		break;

	case FMSMD:
		printf("cyl %d\n", ep->l_sdi[1]);
		break;

	case 0x40:		/* hack for useless design */
		printf("scsi:");
		cp = (unsigned char *)ep->l_sdi;
		for(i = 0; i < 10; i++)
			printf(" %x", *cp++);
		printf(" [%x %x]\n", cp[0], cp[1]); 
		break;

	default:
		printf("\n");
		break;
	}
}

/*
 * unit is believed offline
 * try to bring it on
 */

static
raonline(ra, rp)
register struct radisk *ra;
register struct msaddr *rp;
{
	register struct mscmd *mp;
	int s;

	s = spl6();
	if ((ra->flags & WONLINE) == 0) {
		ra->flags &=~ GOTDI;
		mp = (*ra->port->mp_get)(rp->ctl);
		bzero((caddr_t)mp, sizeof(struct mscmd)); /* clear reserved fields */
		mp->m_crf = ++rarefno;
		mp->m_unit = rp->unit;
		mp->m_opcd = OPONL;
		ra->cmdcrf = mp->m_crf;
		ra->cmdopc = OPONL;
		(*ra->port->mp_send)(rp->ctl, IDRA, mp);
		ra->flags |= WONLINE;
	}
	while (ra->flags & WONLINE)
		tsleep((caddr_t)ra, PRIONL, 60);
	if ((ra->flags & ONLINE) == 0)
		return (0);
	if ((ra->flags & GOTDI) == 0) {
		mp = (*ra->port->mp_get)(rp->ctl);
		mp->m_crf = ++rarefno;
		mp->m_unit = rp->unit;
		mp->m_opcd = OPGUS;
		mp->m_mod = 0;
		mp->m_unfl = 0;
		mp->m_dvpm = 0;	/* ? */
		(*ra->port->mp_send)(rp->ctl, IDRA, mp);
	}
	splx(s);
	return (1);
}

static
rasonl(ra, ep)
register struct radisk *ra;
register struct msend *ep;
{

	if (ra->flags & WONLINE) {
		ra->flags &=~ WONLINE;
		wakeup((caddr_t)ra);
	}
	if ((ep->m_sts & STMSK) != STSUC)
		return;
	ra->flags |= ONLINE;
	if (ra->di.radsize != HUGE && ra->di.radsize != ep->m_unsz)
		printf("ra%d: changed size %d to %d\n", ra-radisk, ra->di.radsize, ep->m_unsz);
	ra->di.radsize = ep->m_unsz;
}

/*
 * controller init
 * set characteristics to turn off host timeouts
 */

static
racinit(ra, rp)
struct radisk *ra;
struct msaddr *rp;
{
	register struct mscmd *mp;
	register int s;

	mp = (*ra->port->mp_get)(rp->ctl);
	mp->m_crf = ++rarefno;
	mp->m_unit = rp->unit;
	mp->m_opcd = OPSCC;
	mp->m_mod = 0;
	mp->m_cntf = CFMSC | CFTHS;
	mp->m_vrsn = MSCPVER;
	mp->m_htmo = 0;		/* no timeout */
	mp->m_time[0] = mp->m_time[1] = 0L;
	s = spl6();
	(*ra->port->mp_send)(rp->ctl, IDRA, mp);
	splx(s);
}
