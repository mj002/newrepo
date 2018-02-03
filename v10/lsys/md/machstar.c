/*
 * machine-specific hardware routines,
 * these for VAX-11/780 & 785
 */

#include "sys/mtpr.h"

int delayfact = 2;		/* factor for DELAY macro */

/*
 * adjust physical top of memory to useful top of memory
 * (some vaxes put important console stuff there)
 * called with mapping off
 */
machmem(hi)
int hi;
{
	return (hi);
}

/*
 * miscellaneous machine-dependent initialization
 * called just after mapping turned on
 */

machinit()
{
	machreset();		/* clear error latches */
	mcrinit();
}

/*
 * stray interrupt handling:
 * just decrypt it and return
 */
strayintr(v)
int v;
{
	if (v < 0x200)
		printf("stray interrupt at 0x%x\n", v);
	else
		printf("stray intr unibus %d 0%o\n", (v >> 9)-1, v & 0777);
}

/*
 * fetch and set the time-of-year clock
 */
gettodr()
{
	return (mfpr(TODR));
}

settodr(t)
unsigned t;
{
	mtpr(TODR, t);
}
