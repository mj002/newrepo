/*
 * @(#)devio.c 1.1 86/02/03 Copyright (c) 1985 by Sun Microsystems, Inc.
 */

/*
 * Device interface code for standalone I/O system.
 *
 * Most simply indirect thru the table to call the "right" routine.
 */
#include <sys/param.h>
#include <sys/inode.h>
#include "saio.h"

/*
 * Strategy -- handles I/O in large blocks for drivers.
 * (This routine is private to this file.)
 *
 * If a devread() or devwrite() is attempted which is larger
 * than the max I/O size for this device, break it up into a
 * series of max-sized operations.
 *
 * Devices which do not declare a max size get the whole thing.
 */
static int
strategy(iob, rw)
	struct iob *iob;
	int rw;
{
	register struct saioreq *sip = &iob->i_si;
	register char *ma;
	register unsigned cc;
	register daddr_t bn;
	register int errs;
	register unsigned maxsize;
	register struct devinfo *dp;

	if (rw == READ && (sip->si_flgs & F_EOF)) {
		sip->si_flgs &= ~F_EOF;
		return (0);
	}
	ma = sip->si_ma;			/* Save for later */
	bn = sip->si_bn;
	cc = sip->si_cc;

	dp = sip->si_boottab->b_devinfo;
	if (dp && (maxsize = dp->d_maxiobytes) != 0 &&
	    ((maxsize & (BUFSIZE - 1)) == 0))
		;
	else
		maxsize = 0x7FFFFFFF;
	errs = 0;
	while (sip->si_cc > 0) {
		if (sip->si_cc > maxsize)
			sip->si_cc = maxsize;
		errs = (*sip->si_boottab->b_strategy)(sip, rw);
		/* short read is expected for 1/2" tape */
		if (errs <= 0)		/* error or EOF */
			break;
		sip->si_ma += errs;
		sip->si_bn += errs / 512;
		sip->si_cc = cc - (sip->si_ma - ma);
	}
	if (errs == 0)
		sip->si_flgs |= F_EOF;
	if (errs >= 0)
		errs = sip->si_ma - ma;		/* Add the part we did before */
	if (errs == 0)
		sip->si_flgs &= ~F_EOF;
	sip->si_ma = ma;			/* Restore */
	sip->si_bn = bn;
	sip->si_cc = cc;
	return errs;
}

int
devread(iob)
	struct iob *iob;
{

	return(strategy(iob, READ));
}

int
devwrite(iob)
	struct iob *iob;
{

	return(strategy(iob, WRITE));
}

int
devopen(iob)
	struct iob *iob;
{
	register struct saioreq *sip = &iob->i_si;
	register struct devinfo *dp;
	register char *a;
	register int result;

	sip->si_flgs &= ~F_EOF;
	sip->si_devaddr = sip->si_devdata = sip->si_dmaaddr = (char *)0;
	dp = sip->si_boottab->b_devinfo;
	if (dp) {
		/* Map controller number into controller address */
		if (sip->si_ctlr < dp->d_stdcount) {
			sip->si_ctlr = (int)((dp->d_stdaddrs)[sip->si_ctlr]);
		}
		/* Map in device itself */
		if (dp->d_devbytes) {
			a = devalloc(dp->d_devtype, sip->si_ctlr,
				dp->d_devbytes);
			if (!a)
				goto bad;
			sip->si_devaddr = a;
		}
		if (dp->d_dmabytes) {
			a = resalloc(RES_DMAMEM, dp->d_dmabytes);
			if (!a) 
				goto bad;
			sip->si_dmaaddr = a;
		}
		if (dp->d_localbytes) {
			a = resalloc(RES_MAINMEM, dp->d_localbytes);
			if (!a) 
				goto bad;
			sip->si_devdata = a;
		}
	}

	result = (sip->si_boottab->b_open)(sip);
	if (result != -1)
		return result;
bad:
	return -1;		/* Indicate failure */
}

devclose(iob)
	struct iob *iob;
{
	struct saioreq *sip = &iob->i_si;

	sip->si_flgs &= ~F_EOF;
	(sip->si_boottab->b_close)(sip);
}
