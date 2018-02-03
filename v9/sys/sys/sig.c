/*	sig.c	4.12	81/07/03	*/

#include "../h/param.h"
#include "../h/systm.h"
#include "../h/dir.h"
#include "../h/user.h"
#include "../h/proc.h"
#include "../h/inode.h"
/*#include "../h/reg.h"*/
#include "../h/text.h"
#include "../h/seg.h"
/*#include "../h/mtpr.h"*/
#include "../machine/pte.h"
#include "../machine/psl.h"
#include "../h/vm.h"
#include "../h/buf.h"
#include "../h/conf.h"
#include "../h/vlimit.h"

/*
 * Send the specified signal to
 * all processes with 'pgrp' as
 * process group.
 * Called by tty.c for quits and
 * interrupts.
 */
gsignal(pgrp, sig)
register pgrp;
{
	register struct proc *p;

	if(pgrp == 0)
		return;
	for(p = proc; p < procNPROC; p++)
		if(p->p_pgrp == pgrp)
			psignal(p, sig);
}

/*
 * Send the specified signal to
 * the specified process.
 */
psignal(p, sig)
register struct proc *p;
register int sig;
{
	register s;
	register int (*action)();
	long sigmask;

	if((unsigned)sig >= NSIG)
		return;
	sigmask = (1L << (sig-1));

	/*
	 * If proc is traced, always give parent a chance.
	 * Otherwise get the signal action from the bits in the proc table.
	 */
	if (p->p_flag & STRC)
		action = SIG_DFL;
	else {
		s = (p->p_siga1&sigmask) != 0;
		s <<= 1;
		s |= (p->p_siga0&sigmask) != 0;
		action = (int(*)())s;
		/*
		 * If the signal is ignored, we forget about it immediately.
		 */
		if (action == SIG_IGN)
			return;
	}
#define mask(sig)	(1<<(sig-1))
#define	stops	(mask(SIGSTOP)|mask(SIGTSTP)|mask(SIGTTIN)|mask(SIGTTOU))
	if (sig) {
		p->p_sig |= sigmask;
		switch (sig) {

		case SIGTERM:
			if ((p->p_flag&STRC) != 0 || action != SIG_DFL)
				break;
			/* fall into ... */

		case SIGKILL:
			if (p->p_nice > NZERO)
				p->p_nice = NZERO;
			break;

		case SIGCONT:
			p->p_sig &= ~stops;
			break;

		case SIGSTOP:
		case SIGTSTP:
		case SIGTTIN:
		case SIGTTOU:
			p->p_sig &= ~mask(SIGCONT);
			break;
		}
	}
#undef mask
#undef stops
	/*
	 * Defer further processing for signals which are held.
	 */
	if (action == SIG_HOLD)
		return;
	s = spl6();
	switch (p->p_stat) {

	case SSLEEP:
		/*
		 * If process is sleeping at negative priority
		 * we can't interrupt the sleep... the signal will
		 * be noticed when the process returns through
		 * trap() or syscall().
		 */
		if (p->p_pri <= PZERO)
			goto out;
		/*
		 * Process is sleeping and traced... make it runnable
		 * so it can discover the signal in issig() and stop
		 * for the parent.
		 */
		if (p->p_flag&STRC)
			goto run;
		switch (sig) {

		case SIGSTOP:
		case SIGTSTP:
		case SIGTTIN:
		case SIGTTOU:
			/*
			 * These are the signals which by default
			 * stop a process.
			 */
			if (action != SIG_DFL)
				goto run;
			/*
			 * Don't clog system with children of init
			 * stopped from the keyboard.
			 */
			if (sig != SIGSTOP && p->p_pptr == &proc[1]) {
				psignal(p, SIGKILL);
				p->p_sig &= ~sigmask;
				splx(s);
				return;
			}
			/*
			 * If a child in vfork(), stopping could
			 * cause deadlock.
			 */
			if (p->p_flag&SVFORK)
				goto out;
			p->p_sig &= ~sigmask;
			p->p_cursig = sig;
			stop(p);
			goto out;

		case SIGTINT:
		case SIGCHLD:
			/*
			 * These signals are special in that they
			 * don't get propogated... if the process
			 * isn't interested, forget it.
			 */
			if (action != SIG_DFL)
				goto run;
			p->p_sig &= ~sigmask;		/* take it away */
			goto out;

		default:
			/*
			 * All other signals cause the process to run
			 */
			goto run;
		}
		/*NOTREACHED*/

	case SSTOP:
		/*
		 * If traced process is already stopped,
		 * then no further action is necessary,
		 * except to guarantee a sure SIGKILL and
		 * prevent multiple SIGSTOP's.
		 */
		if ((p->p_flag&STRC) && sig != SIGKILL && sig != SIGSTOP)
			goto out;
		switch (sig) {

		case SIGKILL:
			/*
			 * Kill signal always sets processes running.
			 */
			goto run;

		case SIGCONT:
			/*
			 * If the process catches SIGCONT, let it handle
			 * the signal itself.  If it isn't waiting on
			 * an event, then it goes back to run state.
			 * Otherwise, process goes back to sleep state.
			 */
			if (action != SIG_DFL || p->p_wchan == 0)
				goto run;
			p->p_stat = SSLEEP;
			goto out;

		case SIGSTOP:
		case SIGTSTP:
		case SIGTTIN:
		case SIGTTOU:
			/*
			 * Already stopped, don't need to stop again.
			 * (If we did the shell could get confused.)
			 */
			p->p_sig &= ~sigmask;		/* take it away */
			goto out;

		default:
			/*
			 * If process is sleeping interruptibly, then
			 * unstick it so that when it is continued
			 * it can look at the signal.
			 * But don't setrun the process as its not to
			 * be unstopped by the signal alone.
			 */
			if (p->p_wchan && p->p_pri > PZERO)
				unsleep(p);
			goto out;
		}
		/*NOTREACHED*/

	default:
		/*
		 * SRUN, SIDL, SZOMB do nothing with the signal,
		 * other than kicking ourselves if we are running.
		 * It will either never be noticed, or noticed very soon.
		 */
		if (p == u.u_procp && !noproc)
			aston();
		goto out;
	}
	/*NOTREACHED*/
run:
	/*
	 * Raise priority to at least PUSER.
	 */
	if (p->p_pri > PUSER)
		if ((p != u.u_procp || noproc) && p->p_stat == SRUN &&
		    (p->p_flag & SLOAD)) {
			remrq(p);
			p->p_pri = PUSER;
			setrq(p);
		} else
			p->p_pri = PUSER;
	setrun(p);
out:
	splx(s);
}

/*
 * Returns true if the current
 * process has a signal to process.
 * The signal to process is put in p_cursig.
 * This is asked at least once each time a process enters the
 * system (though this can usually be done without actually
 * calling issig by checking the pending signal masks.)
 * A signal does not do anything
 * directly to a process; it sets
 * a flag that asks the process to
 * do something to itself.
 */
#define bit(a) (1<<(a-1))
issig()
{
	register struct proc *p = u.u_procp;
	register int sig;
	long sigbits, sigmask, trmask;

	for (;;) {
		sigbits = p->p_sig;
		if ((p->p_flag&STRC) == 0)
			sigbits &= ~p->p_ignsig;
		if (p->p_flag&SVFORK)
			sigbits &= ~(bit(SIGSTOP)|bit(SIGTSTP)|bit(SIGTTIN)|bit(SIGTTOU));
		if (sigbits == 0)
			break;
		sig = (sigbits & bit(SIGKILL)) ? SIGKILL : ffs(sigbits);

		sigmask = 1L << (sig-1);
		p->p_sig &= ~sigmask;		/* take the signal! */
		p->p_cursig = sig;

		trmask = bit(SIGSTOP);	/* SIGSTOP always traced */
		if (p->p_flag&STRC) {
			register struct proc *pp = p;
			do if (pp->p_trace) {
					trmask |= pp->p_trace->i_un.i_sigmask;
					break;
			} while ((pp = pp->p_pptr) && pp->p_flag&STRC);
		}
		trmask &= sigmask & (~bit(SIGKILL));

		if (trmask) {
			/*
			 * If traced, always stop.
			 */
			stop(p);
			swtch();
			/*
			 * If debugger wants us to take the signal,
			 * then it will leave it in p->p_cursig;
			 * otherwise we just look for signals again.
			 */
			if ((sig = p->p_cursig) == 0)
				continue;
		}
		switch ((int)u.u_signal[sig]) {

		case (int)SIG_DFL:
			/*
			 * Don't take default actions on system processes.
			 */
			if (p <= &proc[2])
				break;
			switch (sig) {

			case SIGTSTP:
			case SIGTTIN:
			case SIGTTOU:
				/*
				 * Children of init aren't allowed to stop
				 * on signals from the keyboard.
				 */
				if (p->p_pptr == &proc[1]) {
					psignal(p, SIGKILL);
					continue;
				}
				/* fall into ... */

			case SIGSTOP:
				if (trmask)
					continue;
				stop(p);
				swtch();
				continue;

			case SIGTINT:
			case SIGCONT:
			case SIGCHLD:
				/*
				 * These signals are normally not
				 * sent if the action is the default.
				 * This can happen only if you reset the
				 * signal action from an action which was
				 * not deferred to SIG_DFL before the
				 * system gets a chance to post the signal.
				 */
				continue;		/* == ignore */

			default:
				goto send;
			}
			/*NOTREACHED*/

		case (int)SIG_HOLD:
		case (int)SIG_IGN:
			/*
			 * Masking above should prevent us
			 * ever trying to take action on a held
			 * or ignored signal, unless process is traced.
			 */
			if ((p->p_flag&STRC) == 0)
				printf("issig\n");
			continue;

		default:
			/*
			 * This signal has an action, let
			 * psig process it.
			 */
			goto send;
		}
		/*NOTREACHED*/
	}
	/*
	 * Didn't find a signal to send.
	 */
	p->p_cursig = 0;
	return (0);

send:
	/*
	 * Let psig process the signal.
	 */
	return (sig);
}

#ifndef vax
ffs(mask)
register long mask;
{
	register int i;

	for(i=1; i<NSIG; i++) {
		if(mask & 1)
			return(i);
		mask >>= 1;
	}
	return(0);
}
#endif

/*
 * Put the argument process into the stopped
 * state and notify the parent via wakeup and/or signal.
 */
stop(p)
	register struct proc *p;
{

	p->p_stat = SSTOP;
	p->p_flag &= ~SWTED;
	wakeup((caddr_t)p->p_pptr);
	wakeup((caddr_t)p->p_trace);
	/*
	 * Avoid sending signal to parent if process is traced
	 */
	if (p->p_flag&STRC)
		return;
	psignal(p->p_pptr, SIGCHLD);
}

/*
 * Perform the action specified by
 * the current signal.
 * The usual sequence is:
 *	if(issig())
 *		psig();
 * The signal bit has already been cleared by issig,
 * and the current signal number stored in p->p_cursig.
 */
psig()
{
	register struct proc *rp = u.u_procp;
	register int n = rp->p_cursig;
	long sigmask = 1L << (n-1);
	register int (*action)();

	if (rp->p_cursig == 0)
		panic("psig");
	action = u.u_signal[n];
	if (action != SIG_DFL) {
		if (action == SIG_IGN || action == SIG_HOLD)
			panic("psig action");
		u.u_error = 0;
		if(n != SIGILL && n != SIGTRAP)
			u.u_signal[n] = 0;
		/*
		 * If this catch value indicates automatic holding of
		 * subsequent signals, set the hold value.
		 */
		if (SIGISDEFER(action)) {
			(void) spl6();
			if ((int)SIG_HOLD & 1)
				rp->p_siga0 |= sigmask;
			else
				rp->p_siga0 &= ~sigmask;
			if ((int)SIG_HOLD & 2)
				rp->p_siga1 |= sigmask;
			else
				rp->p_siga1 &= ~sigmask;
			u.u_signal[n] = SIG_HOLD;
			(void) spl0();
			action = SIGUNDEFER(action);
		}
		sendsig(action, n);
		rp->p_cursig = 0;
		return;
	}
	switch (n) {

	case SIGILL:
	case SIGIOT:
	case SIGBUS:
	case SIGQUIT:
	case SIGTRAP:
	case SIGEMT:
	case SIGFPE:
	case SIGSEGV:
	case SIGSYS:
		u.u_arg[0] = n;
		if(core())
			n += 0200;
	}
	exit(n);
}

#ifdef unneeded
int	corestop = 0;
#endif
/*
 * Create a core image on the file "core"
 * If you are looking for protection glitches,
 * there are probably a wealth of them here
 * when this occurs to a suid command.
 *
 * It writes UPAGES block of the
 * user.h area followed by the entire
 * data+stack segments.
 */
core()
{
	register struct inode *ip;
	extern schar();
	struct argnamei nmarg; struct proc coreproc;
	int fd;

	if (ctob(UPAGES+u.u_dsize+u.u_ssize) >= u.u_limit[LIM_CORE])
		return (0);
	coreproc = *u.u_procp;
	*(int *)u.u_stack = (int)&coreproc;
	u.u_error = 0;
	u.u_uid = u.u_ruid;
	u.u_gid = u.u_rgid;
	u.u_dirp = "core";
	nmarg = nilargnamei;
	nmarg.flag = NI_CREAT;
	nmarg.mode = 0666;
	ip = namei(schar, &nmarg, 1);
	if(ip == NULL)
		return(0);
	if(!access(ip, IWRITE) &&
	   (ip->i_mode&IFMT) == IFREG && ip->i_nlink==1) {
		/* unmap funky devices in the user address space */
		for (fd = 0; fd < NOFILE; fd++) 
			if (u.u_ofile[fd] && (u.u_pofile[fd] & MMAPPED))
				munmapfd(fd);
#ifdef sun
		u.u_fp_status.fps_flags = EXT_FPS_FLAGS(&u.u_fp_istate);
		u.u_fp_status.fps_code = u.u_code;
#endif
		(*fstypsw[ip->i_fstyp].t_trunc)(ip);
		u.u_offset = 0;
		u.u_base = (caddr_t)&u;
		u.u_count = ctob(UPAGES);
		u.u_segflg = 1;
		writei(ip);
		u.u_base = (caddr_t)ctob(dptov(u.u_procp, 0));
		u.u_count = ctob(u.u_dsize);
		u.u_segflg = 0;
		writei(ip);
		u.u_base = (caddr_t)ctob(sptov(u.u_procp, u.u_ssize - 1));
		u.u_count = ctob(u.u_ssize);
		writei(ip);
	} else
		u.u_error = EFAULT;
	iput(ip);
	return(u.u_error==0);
}

/*
 * grow the stack to include the SP
 * true return if successful.
 */
grow(sp)
unsigned sp;
{
	register si;

	if(sp >= USRSTACK-ctob(u.u_ssize))
		return(0);
	si = clrnd(btoc((USRSTACK-sp)) - u.u_ssize + SINCR);
	if (ctob(u.u_ssize+si) > u.u_limit[LIM_STACK])
		return(0);
	if (chksize(u.u_tsize, u.u_dsize, u.u_ssize+si))
		return(0);
	if (swpexpand(u.u_dsize, u.u_ssize+si, &u.u_dmap, &u.u_smap)==0)
		return(0);
	
	expand(si, 1);
	return(1);
}
