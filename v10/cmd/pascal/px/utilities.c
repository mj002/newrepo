/* Copyright (c) 1979 Regents of the University of California */

static char sccsid[] = "@(#)utilities.c 1.4 3/6/81";

#include	"whoami.h"
#include	"vars.h"
#include	"panics.h"
#include	"h02opcs.h"

stats()
{
	struct	{
		long	usr_time;
		long	sys_time;
		long	child_usr_time;
		long	child_sys_time;
	} tbuf;
	register double l;
	register long count;
#	ifdef PROFILE
#	define	proffile	"/vb/grad/mckusick/px/profile/pcnt.out"
	struct cntrec {
		double	counts[NUMOPS];	/* instruction counts */
		long	runs;		/* number of interpreter runs */
		long	startdate;	/* date profile started */
		long	usrtime;	/* total user time consumed */
		long	systime;	/* total system time consumed */
		double	stmts;		/* number of pascal stmts executed */
	} profdata;
	FILE *datafile;
#	endif PROFILE

	if (_nodump)
		return(0);
	times(&tbuf);
#	ifdef PROFILE
	datafile = fopen(proffile,"r");
	if (datafile == NULL)
		goto skipprof;
	count = fread(&profdata,1,sizeof(profdata),datafile);
	if (count != sizeof(profdata))
		goto skipprof;
	for (count = 0;  count < NUMOPS;  count++)
		profdata.counts[count] += _profcnts[count];
	profdata.runs += 1;
	profdata.stmts += _stcnt;
	profdata.usrtime += tbuf.usr_time;
	profdata.systime += tbuf.sys_time;
	datafile = freopen(proffile,"w",datafile);
	if (datafile == NULL)
		goto skipprof;
	count = fwrite(&profdata,1,sizeof(profdata),datafile);
	if (count != sizeof(profdata))
		goto skipprof;
	fclose(datafile);
skipprof:
#	endif PROFILE
	l = tbuf.usr_time;
	l = l / HZ;
	fprintf(stderr,
		"\n%1ld statements executed in %04.2f seconds cpu time.\n",
		_stcnt,l);
}

backtrace(errnum)
	int	errnum;
{
	register struct disp *mydp;
	register struct stack *ap;
	register char *cp;
	register long i, linum;
	struct disply disp;

	if (_lino <= 0) {
		fprintf(stderr,"Program was not executed.\n");
		return;
	}
	disp = _display;
	if (errnum == PINTR)
		fprintf(stderr,"\n\tInterrupted in \"");
	else if (errnum == PHALT)
		fprintf(stderr,"\n\tHalted in \"");
	else
		fprintf(stderr,"\n\tError in \"");
	mydp = _dp;
	linum = _lino;
	for (;;) {
		ap = mydp->stp;
		i = linum - (((ap)->entry)->offset & 0177777);
		fprintf(stderr,"%s\"",(ap->entry)->name);
		if (_nodump == FALSE)
			fprintf(stderr,"+%D near line %D.",i,linum);
		fputc('\n',stderr);
		*mydp = (ap)->odisp;
		if (mydp <= &_display.frame[1]){
			_display = disp;
			psexit(errnum);
		}
		mydp = (ap)->dp;
		linum = (ap)->lino;
		fprintf(stderr,"\tCalled by \"");
	}
}

psexit(code)

	int	code;
{
	if (_pcpcount != 0)
		PMFLUSH(_cntrs, _rtns, _pcpcount);
	if (_mode == PIX) {
		fprintf(stderr,"Execution terminated");
		if (code)
			fprintf(stderr," abnormally");
		fputc('.',stderr);
		fputc('\n',stderr);
	}
	stats();
	exit(code);
}
