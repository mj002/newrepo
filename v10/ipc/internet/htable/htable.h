/*
 * common definitions for htable
 */

typedef unsigned long in_addr;

struct addr {
	in_addr addr_val;
	struct	addr *addr_link;
};

struct name {
	char	*name_val;
	struct	name *name_link;
};

struct gateway {
	struct	gateway *g_link;
	struct	gateway *g_dst;		/* connected gateway if metric > 0 */
	struct	gateway *g_firstent;	/* first entry for this gateway */
	struct	name	*g_name;
	int	g_net;
	in_addr g_addr;		/* address on g_net */
	int	g_metric;		/* hops to this net */
};

#define	NOADDR			((struct addr *)0)
#define	NONAME			((struct name *)0)

#define	KW_NET		1
#define	KW_GATEWAY	2
#define	KW_HOST		3

struct name *newname();
char *malloc();

char *infile;			/* Input file name */
