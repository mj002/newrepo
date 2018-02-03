/*
	This is version 4.  The input language is:

Cmd	Abbrev	Operands / Comments
comment	#		only at start of line
blank	b	s <vnum>	start blanking; erase commands follow
		e <vnum>	end blanking current view
click	c	<clicknum>
define	d	v <number> <viewname> xmin ymin xmax ymax
		c <number> <clickname>
		p e	pragma -- end of defs at front of file
			Other possible ``pragmas'':			???
				Number of views, clicks
				Number of (lines, bytes) in file
erase	e	<line repeated here, except for leading g>
			Object is deletable
geom	g	<slotn>	l <vnum> <opts> <x1> <y1> <x2> <y2>	line
			b <vnum> <opts> <x1> <y1> <x2> <y2>	box
			c <vnum> <opts> <x> <y> <rad>		circle
			t <vnum> <opts> <x> <y> <text string>	text
				one separating blank; string unquoted
			    <opts> format: string of characters, in order
			    <slotn> == 0 => line never erased

At beginning of file:
	d v <number> <view name>
	d c <number> <click name>
	d p <various pragmas here>
	d p e

 */

#include <stdio.h>
#include "anim.h"

#define	sendpoint(p)	{ sendint(p.x); sendint(p.y); }
#define	sendpair(x,y)	{ sendint(x); sendint(y); }
#define	eq(s, t)	(strcmp(s,t) == 0)
#define	FATAL	1

char	*cmdname;
char	*emalloc();

long	memsize	= 80000;	/* jerq memory allocation */

int	dbg	= 0;
int	lineno	= 1;	/* ? */

main(argc, argv)
	char *argv[];
{
	FILE *fp;
	char *term_process = ANIMTERM;
	char *zload = "";
	char buf[100];
	int i, c;

	cmdname = argv[0];
	while (argc > 1 && argv[1][0] == '-') {
		switch (argv[1][1]) {
		case 'd':
			dbg = !dbg;
			break;
		case 'z':
			zload = "-z";
			break;
		case 't':
			term_process = argv[2];
			argc--;
			argv++;
			break;
		case 'm':
			memsize = atoi(&argv[1][2]);
			break;
		}
		argc--;
		argv++;
	}
	if (argc <= 1)
		error(FATAL, "no input file");
	if ((fp = fopen(argv[1], "r")) == NULL)
		error(FATAL, "can't open %s\n", argv[1]);
	if (!dbg) {
		sprintf(buf, "32ld %s %s </dev/tty", zload, term_process);
		system(buf);
		set_tty();
	}
	send_data(fp);
	if (dbg)
		exit(0);
	while ((c = readchar()) != P_QUIT) {
		if (c == P_FILE) {	/* rest of line is a filename */
			readstring(buf);
			if ((fp = fopen(buf, "r")) == NULL)
				send1char(P_ERROR);
			else {
				send1char(P_FILE);	/* ack */
				send_data(fp);
			}
		} else
			send1char(P_ERROR);
	}
	reset_tty();
}

send_data(fp)
	FILE *fp;
{
	char text[100], buf[100], opts[100], *p;
	int c, x1, y1, x2, y2, i, v, slot;

	sendint(memsize);
	send1char(P_INIT);
	send1char(P_PRINT);
	sendstring("here comes the data!");
	while ((c = getc(fp)) != EOF) {
		switch (c) {
		case ' ':
		case '\t':
			break;
		case '\n':
			lineno++;	/* this should be the only increment */
			break;
		case '#':		/* comments */
			skipline(fp);
			break;
		case 'b':	/* blank.  ignore for now */
			skipline(fp);
			break;
		case 'c':		/* click */
			fscanf(fp, "%d", &i);
			send1char(P_OBJECT);
			send1char(c);
			sendint(i);
			skipline(fp);
			break;
		case 'e':		/* erase */
			fscanf(fp, "%d", &i);
			send1char(P_OBJECT);
			send1char(c);
			sendint(i);
			skipline(fp);
			break;
		case 'g':		/* geom:  draw line, box, circle, ... */
			fscanf(fp, "%d", &slot);
			switch (c = skipbl(fp)) {
			case 'l':
			case 'b':
				fscanf(fp, "%d %s %d %d %d %d", &v, opts, &x1, &y1, &x2, &y2);
				send1char(P_OBJECT);
				send1char(c);
				sendint(slot);
				sendint(v);
				sendopt(c=='b' ? boxops : lineops, opts);
				sendpair(x1, y1);
				sendpair(x2, y2);
				break;
			case 'c':	/* circle */
				fscanf(fp, "%d %s %d %d %d", &v, opts, &x1, &y1, &x2);
				send1char(P_OBJECT);
				send1char('o');	/* 'o' is for circle */
				sendint(slot);
				sendint(v);
				sendopt(circops, opts);
				sendpair(x1, y1);
				sendint(x2);
				break;
			case 't':	/* text */
				fscanf(fp, "%d %s %d %d", &v, opts, &x1, &y1);
				send1char(P_OBJECT);
				send1char('t');
				sendint(slot);
				sendint(v);
				sendopt(textops, opts);
				sendpair(x1, y1);
				getc(fp);	/* skip 1 separator; no quotes */
				for (p = text; (c = getc(fp)) != '\n'; )
					*p++ = c;
				*p = 0;
				ungetc('\n', fp);
				/* slow... */
				if (eq(text, "bullet"))
					sendstring("*");
				else if (eq(text, "dot"))
					sendstring(".");
				else if (eq(text, "circle"))
					sendstring("o");
				else if (eq(text, "times"))
					sendstring("x");
				else
					sendstring(text);
				break;
			default:
				error(FATAL, "illegal g command: g %c", c);
				break;
			}
			break;
		case 'd':	/* definition of some sort */
			switch (c = skipbl(fp)) {
			case 'v':	/* view */
				fscanf(fp, "%d %s", &i, text);
				send1char(P_DEFINE);
				send1char('v');
				sendint(i);
				sendstring(text);
				skipline(fp);	/* might be a title there */
				break;
			case 'c':	/* click */
				fscanf(fp, "%d %s", &i, text);
				send1char(P_DEFINE);
				send1char('c');
				sendint(i);
				sendstring(text);
				break;
			case 'p':
				skipline(fp);
				break;
			default:
				error(FATAL, "unknown define command d %c", c);
				break;
			}
			break;
		default:
			error(FATAL, "unknown command %c", c);
			break;
		}
	}
	send1char(P_ENDFILE);
	flushproto();
	fclose(fp);
}

sendopt(optvals, opts)
	int optvals[];
	char *opts;
{
	int i, n;

	n = 0;
	for (i = 0; optvals[i] && opts; i += 2)
		if (*opts == optvals[i]) {
			n += optvals[i+1];
			opts++;
		}
	sendint(n);
}

skipbl(fp)
	FILE *fp;
{
	int c;

	while ((c = getc(fp)) == ' ' || c == '\t')
		;
	return c;
}

skipline(fp)
	FILE *fp;
{
	int c;

	while ((c = getc(fp)) != '\n')
		;
	ungetc('\n', fp);
}

error(f, s, a1, a2, a3, a4, a5, a6, a7)
{
	extern char *cmdname;
	extern int dbg;

	fprintf(stderr, "%s: ", cmdname);
	fprintf(stderr, s, a1, a2, a3, a4, a5, a6, a7);
	fprintf(stderr, "\n");
	if (lineno)
		fprintf(stderr, " source line number %d\n", lineno);
	if (f) {
		if (dbg)
			abort();
		exit(2);
	}
}
