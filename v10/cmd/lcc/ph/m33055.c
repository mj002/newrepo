/* The Plum Hall Validation Suite for C
 * Unpublished copyright (c) 1986-1991, Chiron Systems Inc and Plum Hall Inc.
 * VERSION: 4
 * DATE: 1993-01-01
 * The "ANSI" mode of this suite corresponds to official ANSI C, X3.159-1989.
 * As per your license agreement, your distribution is not to be moved or copied outside the Designated Site
 * without specific permission from Plum Hall Inc.
 */

/* 3.3.16	ConP1S1	CONSTRAINT-MANDATORY */
/* 	left opnd of asst shall be modifiable lvalue */
int main() {
	typedef const struct ci { int i; } CI;
	CI a = {0};
	a.i = 1;
	return 0; }
