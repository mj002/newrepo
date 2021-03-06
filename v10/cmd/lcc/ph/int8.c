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
 **    EGEN -P -R8 -V -A -N5 -Dinteger.gen ( @ @ ) @ ( @ @ )               **
 ****************************************************************************/
#include "defs.h"


int main()
	{
	extern char *Filename;
	auto unsigned int ui;
	static unsigned int * pui;
	auto unsigned int ** ppui;
	auto int i;
	static int * pi;
	auto int ** ppi;
	auto unsigned short us;
	static unsigned short * pus;
	auto unsigned short ** ppus;
	auto short s;
	static short * ps;
	auto short ** pps;
	auto unsigned char uc;
	static unsigned char * puc;
	auto unsigned char ** ppuc;
	auto char c;
	static char * pc;
	auto char ** ppc;
	auto unsigned long ul;
	static unsigned long * pul;
	auto unsigned long ** ppul;
	auto long l;
	static long * pl;
	auto long ** ppl;
	register int rint1;
	register int rint2;
	register int rint3;
	register int rint4;
	register int rint5;

	ui = ivalue(100);
	pui = &ui;
	ppui = &pui;
	i = ivalue(10);
	pi = &i;
	ppi = &pi;
	us = lvalue(33000);
	pus = &us;
	ppus = &pus;
	s = ivalue(13);
	ps = &s;
	pps = &ps;
	uc = ivalue(128);
	puc = &uc;
	ppuc = &puc;
	c = ivalue(20);
	pc = &c;
	ppc = &pc;
	ul = lvalue(65000L);
	pul = &ul;
	ppul = &pul;
	l = lvalue(97L);
	pl = &l;
	ppl = &pl;
	rint1 = ivalue(1);
	rint2 = ivalue(2);
	rint3 = ivalue(3);
	rint4 = ivalue(4);
	rint5 = ivalue(5);
	Filename = "int8.c";

	/* 1 % 13 => 1 */
	/* 1 > 1 => 0 */
	/* post-- 1 => 2 */
	/* 97L %= 2 => 1L */
	/* 0 & 1L => 0L */
	lequals(__LINE__, (((rint1 % s) > rint1) & (**ppl %= ((rint2)-- ))), 0L);
	iequals(__LINE__, rint2, 1);
	lequals(__LINE__, l, 1L);

	/* 1L * 0x64 => 100L */
	/* 100L + 0x64 => 200L */
	/* 0x80 /= 20 => 0x6 */
	/* 10 += 0x6 => 16 */
	/* 200L - 16 => 184L */
	lequals(__LINE__, (((*pl * ui) + **ppui) - (**ppi += (**ppuc /= **ppc))), 184L);
	iequals(__LINE__, uc, (unsigned)6);
	iequals(__LINE__, i, 16);

	/* 1 >>= 1 => 0 */
	/* 1L >>= 0 => 1L */
	/* 1L + 1 => 2L */
	lequals(__LINE__, ((*pl >>= (rint1 >>= rint2)) + (**ppc || ( ! ui))), 2L);
	iequals(__LINE__, rint1, 0);
	lequals(__LINE__, l, 1L);

	/* post-- 2 => 3 */
	/* 0x64 = 3 => 0x3 */
	/* pre++ 0xfde8L => 0xfde9L */
	/* 4 ^ 0xfde9L => 0xfdedL */
	/* 0x3 | 0xfdedL => 0xfdefL */
	lequals(__LINE__, ((**ppui = ((rint3)-- )) | (rint4 ^ ( ++ ul))), 0xfdefL);
	lequals(__LINE__, ul, 65001L);
	iequals(__LINE__, rint3, 2);
	iequals(__LINE__, ui, (unsigned)3);

	/* pre-- 0x6 => 0x5 */
	/* 2 == 5 => 0 */
	/* 1L * 20 => 20L */
	/* 20L < 1L => 0 */
	/* 0 - 0 => 0 */
	iequals(__LINE__, ((rint3 == ( -- **ppuc)) - ((l * c) < **ppl)), 0);
	iequals(__LINE__, uc, (unsigned)5);

	return 	report(Filename);
	}
