/* Copyright (c) 1979 Regents of the University of California */

static char sccsid[] = "@(#)BUFF.c 1.2 3/7/81";

#include "h00vars.h"

extern char	_sobuf[];

BUFF(amount)

	long		amount;
{
	struct iorec	*curfile;

	curfile = OUTPUT;
	if (amount == 0)
		setbuf(ACTFILE(curfile), (char *) 0);
	else if (amount == 2)
		setbuf(ACTFILE(curfile), _sobuf);
}
