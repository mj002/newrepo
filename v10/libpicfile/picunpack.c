/*
 * picunpack -- non-ansi version, uses varargs instead of stdarg
 * Bugs:
 *	f and d are non-portable
 *	could be faster
 */
#include <picfile.h>
#include <stdio.h>
#include <varargs.h>
void picunpack(va_alist)
va_dcl
{
	register char *bp, *cp;
	register short *sp;
	register long *lp;
	register float *fp;
	register double *dp;
	register char *buf, *ebuf;
	PICFILE *f;
	char *fmt;
	int nchan;
	va_list pvar;
	va_start(pvar);
	f=va_arg(pvar, PICFILE *);
	buf=va_arg(pvar, char *);
	nchan=f->nchan;
	ebuf=buf+f->width*nchan;
	for(fmt=va_arg(pvar, char *);*fmt;fmt++) switch(*fmt){
	case 'c':
		cp=va_arg(pvar, char *);
		for(bp=buf;bp!=ebuf;bp+=nchan)
			*cp++=*bp;
		buf++;
		ebuf++;
		break;
	case 's':
		sp=va_arg(pvar, short *);
		for(bp=buf;bp!=ebuf;bp+=nchan)
			*sp++=(bp[0]&255)|(bp[1]<<8);
		buf+=2;
		ebuf+=2;
		break;
	case 'l':
		lp=va_arg(pvar, long *);
		for(bp=buf;bp!=ebuf;bp+=nchan)
			*lp++=(bp[0]&255)|((bp[1]&255)<<8)|((bp[2]&255)<<16)|(bp[3]<<24);
		buf+=4;
		ebuf+=4;
		break;
	case 'f':
		fp=va_arg(pvar, float *);
		for(bp=buf;bp!=ebuf;bp+=nchan){
			cp=(char *)fp++;
			cp[0]=bp[0];
			cp[1]=bp[1];
			cp[2]=bp[2];
			cp[3]=bp[3];
		}
		buf+=4;
		ebuf+=4;
		break;
	case 'd':
		dp=va_arg(pvar, double *);
		for(bp=buf;bp!=ebuf;bp+=nchan){
			cp=(char *)dp++;
			cp[0]=bp[0];
			cp[1]=bp[1];
			cp[2]=bp[2];
			cp[3]=bp[3];
			cp[4]=bp[4];
			cp[5]=bp[5];
			cp[6]=bp[6];
			cp[7]=bp[7];
		}
		buf+=8;
		ebuf+=8;
		break;
	case '_':
		buf++;
		ebuf++;
		break;
	}
	va_end(pvar);
}
