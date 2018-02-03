/*	"@(#) sigcatch.c: V 1.19 6/27/83"	*/

/*	sigcatch.c: 	signal catching routines	*/

#include		<signal.h>
#include		<stdio.h>
#include		"bas.h"

char	*mktemp(),
    	template[] = "basic.XXXXXX";

extern	char	OLDAOUT[],
		NEWAOUT[],
		IFILE[],
		LDOUT[],
		filnam[];


static UCHAR	byebye = 0;

/*
*	issue message, save file, and exit BASIC
*/

bomb()
{
	FILE	*dmpfp;
	char	*getenv();

	setsig();
	if (byebye)
	{
		printf("Cannot Save Program\n");
		exit(-1);
	}
	else
		byebye = 1;
	printf("******   BASIC INTERNAL ERROR   ******\n");
	unlink(OLDAOUT);
	unlink(NEWAOUT);
	unlink(IFILE);
	unlink(LDOUT);
	strcpy(filnam, template);
	mktemp(filnam);
	strcat(filnam, ".b");
	expr = &nullstrng;
	if ((dmpfp = fopen(filnam, "w")) != NULL)
	{
		fclose(dmpfp);
		save();
	} else {
		strcpy(filnam, getenv("HOME"));
		strcat(filnam, "/");
		strcat(filnam, template);
		mktemp(filnam);
		strcat(filnam, ".b");
		if ((dmpfp=fopen(filnam, "w")) != NULL)
		{
			fclose(dmpfp);
			save();
		}
	}
	clall();
	exit(-1);
}


catch()
{
	FILE	*dmpfp;
	char	*getenv();

	setsig();
	if (byebye)
	{
		printf("Cannot Save Program\n");
		exit(-1);
	}
	else
		byebye = 1;
	printf("******   SIGNAL CAUGHT   ******\n");
	unlink(OLDAOUT);
	unlink(NEWAOUT);
	unlink(IFILE);
	strcpy(filnam, template);
	mktemp(filnam);
	strcat(filnam, ".b");
	expr = &nullstrng;
	if ((dmpfp = fopen(filnam, "w")) != NULL)
	{
		fclose(dmpfp);
		save();
	} else {
		strcpy(filnam, getenv("HOME"));
		strcat(filnam, "/");
		strcat(filnam, template);
		mktemp(filnam);
		strcat(filnam, ".b");
		if ((dmpfp=fopen(filnam, "w")) != NULL)
		{
			fclose(dmpfp);
			save();
		}
	}
	clall();
	exit(-1);
}

#include <setjmp.h>

extern jmp_buf	overenv;



overf()
{
	error(inst.thing.linno,inst.thing.stmtno, 60);			/* overflow */
	signal(SIGFPE, overf);
	longjmp(overenv,0);
}


badnum()
{
	error(inst.thing.linno,inst.thing.stmtno, 42);
	signal(SIGFPE, overf);
	longjmp(overenv,0);
}


getfloat(ptr, fnum)
char	*ptr;
float	*fnum;
{
	char	*fptr,
		*sptr;
	int	i;

	fptr = (char *) fnum;
	sptr = ptr;
	for (i=0; i<sizeof(float); i++)
		*fptr++ = *sptr++;
	signal(SIGFPE, badnum);
	if (setjmp(overenv) != 0)
	{
		signal(SIGFPE, overf);
		return(-1);
	}
	else
		*fnum;
	signal(SIGFPE, overf);
	return(0);
}



getdouble(ptr, dnum)
char	*ptr;
double	*dnum;
{
	char	*dptr,
		*sptr;
	int	i;

	dptr = (char *) dnum;
	sptr = ptr;
	for (i=0; i<sizeof(double); i++)
		*dptr++ = *sptr++;
	signal(SIGFPE, badnum);
	if (setjmp(overenv) != 0)
	{
		signal(SIGFPE, overf);
		return(-1);
	}
	else
		*dnum;
	signal(SIGFPE, overf);
	return(0);
}



/*
* 	change default signal handling routines
*/

setsig()
{
	int	(*oldsig)();

	if ((oldsig = signal(SIGHUP, catch)) != SIG_DFL)
		signal(SIGHUP, oldsig);
	if ((oldsig = signal(SIGTERM, catch)) != SIG_DFL)
		signal(SIGTERM, oldsig);
	if ((oldsig = signal(SIGQUIT, catch)) != SIG_DFL)
		signal(SIGQUIT, oldsig);
	if ((oldsig = signal(SIGILL, bomb)) != SIG_DFL)
		signal(SIGILL, oldsig);
	if ((oldsig = signal(SIGTRAP, bomb)) != SIG_DFL)
		signal(SIGTRAP, oldsig);
	if ((oldsig = signal(SIGIOT, bomb)) != SIG_DFL)
		signal(SIGIOT, oldsig);
	if ((oldsig = signal(SIGEMT, bomb)) != SIG_DFL)
		signal(SIGEMT, oldsig);
	if ((oldsig = signal(SIGFPE, overf)) != SIG_DFL)
		signal(SIGFPE, oldsig);
	if ((oldsig = signal(SIGBUS, bomb)) != SIG_DFL)
		signal(SIGBUS, oldsig);
	if ((oldsig = signal(SIGSEGV, bomb)) != SIG_DFL)
		signal(SIGSEGV, oldsig);
	if ((oldsig = signal(SIGSYS, bomb)) != SIG_DFL)
		signal(SIGSYS, oldsig);
	if ((oldsig = signal(SIGPIPE, bomb)) != SIG_DFL)
		signal(SIGPIPE, oldsig);
	if ((oldsig = signal(SIGALRM, bomb)) != SIG_DFL)
		signal(SIGALRM, oldsig);
/*
	if ((oldsig = signal(SIGUSR1, bomb)) != SIG_DFL)
		signal(SIGUSR1, oldsig);
	if ((oldsig = signal(SIGUSR2, bomb)) != SIG_DFL)
		signal(SIGUSR2, oldsig);
*/
}
