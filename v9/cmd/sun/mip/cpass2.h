/*	@(#)cpass2.h 1.1 86/02/03 SMI	*/

# include "machdep.h"
# include "machdep2.h"
# include "mip.h"

# ifdef ONEPASS

/*	bunch of stuff for putting the passes together... */
# define crslab crs2lab
# define where where2
# define xdebug x2debug
# define tdebug t2debug
# define deflab def2lab
# define edebug e2debug
# define eprint e2print
# define getlab get2lab
# define filename ftitle
# endif

/*	cookies, used as arguments to codgen */

# define FOREFF       01 /* compute for effects only */
# define INAREG       02 /* compute into a register */
# define INTAREG      04 /* compute into a scratch register */
# define INBREG      010 /* compute into a lvalue register */
# define INTBREG     020 /* compute into a scratch lvalue register */
# define INCREG      040 /* compute into a floating register */
# define INTCREG    0100 /* compute into a scratch floating register */
# define FORCC      0200 /* compute for condition codes only */
# define INTEMP  0200000 /* compute into a temporary location */
# define FORARG  0400000 /* compute for an argument of a function */
# define FORREW 01000000 /* search the table, looking for a rewrite rule */

	/* OP descriptors */
	/* the ASG operator may be used on some of these */

# define OPSIMP  010000 /* +, -, &, |, ^ */
# define OPCOMM  010002 /* +, &, |, ^    */
# define OPMUL   010004 /* *, /          */
# define OPDIV   010006 /* /, %          */
# define OPUNARY 010010 /* unary ops     */
# define OPLEAF  010012 /* leaves        */
# define OPANY   010014 /* any op...     */
# define OPLOG   010016 /* logical ops   */
# define OPFLOAT 010020 /* +, -, *, or / (for floats) */
# define OPSHFT  010022 /* <<, >>        */
# define OPLTYPE 010024 /* leaf type nodes (e.g, NAME, ICON, etc. ) */
# define OPINTR  010026 /* fortran intrinsics (sin, cos, exp, ...) */

	/* match returns */

# define MNOPE 010000
# define MDONE 010001

	/* shapes */

# define SANY     01	/* same as FOREFF   */
# define SAREG    02	/* same as INAREG   */
# define STAREG   04	/* same as INTAREG  */
# define SBREG   010	/* same as INBREG   */
# define STBREG  020	/* same as INTBREG  */
# define SCREG   040	/* same as INCREG   */
# define STCREG 0100	/* same as INTCREG  */
# define SCC    0200	/* same as FORCC    */
# define SNAME  0400
# define SCON  01000
# define SFLD  02000
# define SOREG 04000
/* indirection or wild card shapes */
# ifndef WCARD1
# define STARNM  010000
# endif
# ifndef WCARD2
# define STARREG 020000
# endif
# define SWADD   040000
# define SPECIAL 0100000
# define SZERO   SPECIAL
# define SONE   (SPECIAL|1)
# define SMONE  (SPECIAL|2)
# define SCCON  (SPECIAL|3)	/* -256 <= constant < 256 */
# define SSCON  (SPECIAL|4)	/* -32768 <= constant < 32768 */
# define SSOREG (SPECIAL|5)	/* non-indexed OREG */

	/* FORARG and INTEMP are carefully not conflicting with shapes */

	/* types */

# define TCHAR 01
# define TSHORT 02
# define TINT 04
# define TLONG 010
# define TFLOAT 020
# define TDOUBLE 040
# define TPOINT 0100
# define TUCHAR 0200
# define TUSHORT 0400
# define TUNSIGNED 01000
# define TULONG 02000
# define TPTRTO 04000  /* pointer to one of the above */
# define TANY 010000  /* matches anything within reason */
# define TSTRUCT 020000   /* structure or union */

	/* reclamation cookies */

# define RNULL 0    /* clobber result */
# define RLEFT 01
# define RRIGHT 02
# define RESC1 04
# define RESC2 010
# define RESC3 020
# define RESCC 04000
# define RESFCC 010000 /* floating coprocessor condition code */
# define RNOP 020000   /* DANGER: can cause loops.. */

	/* needs */

# define NAREG       01
# define NACOUNT     03
# define NAMASK     017
# define NASL        04  /* share left register */
# define NASR       010 /* share right register */
# define NBREG      020
# define NBCOUNT    060
# define NBMASK    0360
# define NBSL      0100
# define NBSR      0200
# define NCREG     0400
# define NCCOUNT  01400
# define NCMASK   07400
# define NCSL     02000
# define NCSR     04000
# define NTEMP    010000
# define NTMASK  0170000
# define REWRITE 0200000
# define EITHER  0400000 /* "either" modifier for needs */


# define MUSTDO 010000   /* force register requirements */
# define NOPREF 020000  /* no preference for register assignment */


	/* register allocation */

extern int rstatus[];
extern int busy[];

extern struct respref { int cform; int mform; } respref[];

# define isareg(r) (rstatus[r]&SAREG)
# define isbreg(r) (rstatus[r]&SBREG)
# define iscreg(r) (rstatus[r]&SCREG)
# define istreg(r) (rstatus[r]&(STBREG|STAREG|STCREG))
# define istnode(p) (p->in.op==REG && istreg(p->tn.rval))

# define TBUSY 01000
# define REGLOOP(i) for(i=0;i<REGSZ;++i)

# define SETSTO(x,y) (stotree=(x),stocook=(y))
extern int stocook;
# define DELAYS 20
extern NODE *deltrees[DELAYS];
extern int deli;   /* mmmmm */

extern NODE *stotree;
extern int callflag;

# ifndef ONEPASS
union ndu {

	struct {
		int op;
		int rall;
		TWORD type;
		SUTYPE su;
		int stalign;  /* alignment of structure objects */
#ifndef FLEXNAMES
		char name[NCHNAM];
#else
		char *name;
#endif
		NODE *left;
		NODE *right;
		}in; /* interior node */
	
	struct {
		int op;
		int rall;
		TWORD type;
		SUTYPE su;
		int stalign;  /* alignment of structure objects */
#ifndef FLEXNAMES
		char name[NCHNAM];
#else
		char *name;
#endif
		CONSZ lval;
		int rval;
		}tn; /* terminal node */
	
	struct {
		int op, rall;
		TWORD type;
		SUTYPE su;
		int label;  /* for use with branching */
		int reversed;
		}bn; /* branch node */

	struct {
		int op, rall;
		TWORD type;
		SUTYPE su;
		int stalign;  /* alignment of structure objects */
		int stsize;  /* sizes of structure objects */
		}stn; /* structure node */

	struct {
		/* this structure is used when a floating point constant
		   is being computed */
		int op;
		int rall;
		TWORD type;
		SUTYPE su;
		double dval;
		}fpn; /* floating point node */

	};
#endif

extern struct optab {
	int op;
	int visit;
	int lshape;
	int ltype;
	int rshape;
	int rtype;
	int needs;
	int rewrite;
	char * cstring;
	}
	table[];

extern NODE resc[];

extern OFFSZ tmpoff;
extern OFFSZ maxoff;
extern OFFSZ baseoff;
extern OFFSZ maxtemp;
extern int maxtreg;
extern int ftnno;
extern int rtyflg;

extern int nrecur;  /* flag to keep track of recursions */

# define NRECUR (10*TREESZ)

#define ncopy(dest,srce) (*(dest) = *(srce))

extern NODE
	*talloc(),
	*eread(),
	*tcopy(),
	*getlr();

extern CONSZ rdin();

extern int eprint();

extern char *rnames[];

extern int lineno;
extern char filename[];
extern int fldshf, fldsz;
extern int lflag, xdebug, udebug, edebug, odebug, rdebug, radebug, tdebug, sdebug;

#ifndef callchk
#define callchk(x) allchk()
#endif

#ifndef PUTCHAR
# define PUTCHAR(x) putchar(x)
#endif

	/* macros for doing double indexing */
# define R2PACK(x,y,z) (0200*((x)+1)+y+040000*z)
# define R2UPK1(x) ((((x)>>7)-1)&0177)
# define R2UPK2(x) ((x)&0177)
# define R2UPK3(x) (x>>14)
# define R2TEST(x) ((x)>=0200)

# ifdef MULTILEVEL

union mltemplate{
	struct ml_head{
		int tag; /* identifies class of tree */
		int subtag; /* subclass of tree */
		union mltemplate * nexthead; /* linked by mlinit() */
		} mlhead;
	struct ml_node{
		int op; /* either an operator or op description */
		int nshape; /* shape of node */
		/* both op and nshape must match the node.
		 * where the work is to be done entirely by
		 * op, nshape can be SANY, visa versa, op can
		 * be OPANY.
		 */
		int ntype; /* type descriptor from mfile2 */
		} mlnode;
	};

# endif
