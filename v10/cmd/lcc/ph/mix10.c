/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of the Suite corresponds to the official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/****************************************************************************
 ** Self-checking C source code generated by EGEN component of             **
 **    The Plum Hall Validation Suite for C.                               **
 **    (C) 1986-1988 Chiron Systems Inc and Plum Hall Inc                  **
 **    EGEN Version 1.09.000  88/08/21                                     **
 **                                                                        **
 **    EGEN -P -R10 -V -A -N5 -Dmixed.gen ( @ @ ) @ ( @ @ )                **
 ****************************************************************************/
#include "defs.h"


int main()
	{
	extern char *Filename;
	static float f1;
	auto float * pf1;
	static long stlong1;
	static float f2;
	auto float * pf2;
	static long stlong2;
	static float f3;
	static float * pf3;
	auto long stlong3;
	static float f4;
	static float * pf4;
	static long stlong4;
	static double d1;
	auto double * pd1;
	static long stlong5;
	static double d2;
	static double * pd2;
	static long stlong6;
	auto double d3;
	static double * pd3;
	static long stlong7;
	static double d4;
	static double * pd4;
	auto long stlong8;

	f1 = fvalue(1.100000E+000);
	pf1 = &f1;
	stlong1 = lvalue(1L);
	f2 = fvalue(2.200000E+000);
	pf2 = &f2;
	stlong2 = lvalue(2L);
	f3 = fvalue(3.300000E+000);
	pf3 = &f3;
	stlong3 = lvalue(3L);
	f4 = fvalue(4.400000E+000);
	pf4 = &f4;
	stlong4 = lvalue(4L);
	d1 = dvalue(1.100000000000000E+000);
	pd1 = &d1;
	stlong5 = lvalue(5L);
	d2 = dvalue(2.200000000000000E+000);
	pd2 = &d2;
	stlong6 = lvalue(6L);
	d3 = dvalue(3.300000000000000E+000);
	pd3 = &d3;
	stlong7 = lvalue(7L);
	d4 = dvalue(4.400000000000000E+000);
	pd4 = &d4;
	stlong8 = lvalue(8L);
	Filename = "mix10.c";

	/* 1L & 8L => 0L */
	/* 2L &= 0L => 0L */
	/* 3L ^ 3L => 0L */
	/* 0L ^ 7L => 7L */
	/* 0L > 7L => 0 */
	iequals(__LINE__, ((stlong2 &= (stlong1 & stlong8)) > ((stlong3 ^ stlong3) ^ stlong7)), 0);
	lequals(__LINE__, stlong2, 0L);

	/* post++ 2.10000000000000E+000 => 1.10000000000000E+000 */
	/* 1.10000000000000E+000 + 2.20000E+000 => 3.30000000000000E+000 */
	/* 4.40000000000000E+000 != 4.40000E+000 + .1 => 1 */
	/* 8L += 1 => 9L */
	/* 3.30000000000000E+000 + 9L => 1.23000000000000E+001 */
	fequals(__LINE__, ((((*pd1)++ ) + f2) + (stlong8 += (d4 != f4 + .1))), 1.23000000000000E+001);
	lequals(__LINE__, stlong8, 9L);
	fequals(__LINE__, d1, 2.10000000000000E+000);

	/* 3L += 6L => 9L */
	/* 5L |= 9L => 13L */
	/* 1L / 6L => 0L */
	/* 4L -= 0L => 4L */
	/* 13L << 4L => 208L */
	lequals(__LINE__, ((stlong5 |= (stlong3 += stlong6)) << (stlong4 -= (stlong1 / stlong6))), 208L);
	lequals(__LINE__, stlong4, 4L);
	lequals(__LINE__, stlong3, 9L);
	lequals(__LINE__, stlong5, 13L);

	/* ! 9L => 0 */
	/* 0 != 3.30000E+000 => 1 */
	/* 4L - 1L => 3L */
	/* 13L &= 3L => 1L */
	/* 1 >= 1L => 1 */
	iequals(__LINE__, ((( ! stlong3) != *pf3) >= (stlong5 &= (stlong4 - stlong1))), 1);
	lequals(__LINE__, stlong5, 1L);

	/* 1L + 1L => 2L */
	/* 2L != 9L => 1 */
	/* 1 & 1 => 1 */
	iequals(__LINE__, (((stlong5 + stlong1) != stlong3) & ((stlong3 || stlong8) || stlong7)), 1);

	return 	report(Filename);
	}
