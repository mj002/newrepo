/* Copyright (c) 1979 Regents of the University of California */

static char sccsid[] = "@(#)rec.c 1.4 4/1/81";

#include "whoami.h"
#include "0.h"
#include "tree.h"
#include "opcode.h"

/*
 * Build a record namelist entry.
 * Some of the processing here is somewhat involved.
 * The basic structure we are building is as follows.
 *
 * Each record has a main RECORD entry, with an attached
 * chain of fields as ->chain;  these include all the fields in all
 * the variants of this record.
 *
 * Attached to NL_VARNT is a chain of VARNT structures
 * describing each of the variants.  These are further linked
 * through ->chain.  Each VARNT has, in ->range[0] the value of
 * the associated constant, and each points at a RECORD describing
 * the subrecord through NL_VTOREC.  These pointers are not unique,
 * more than one VARNT may reference the same RECORD.
 *
 * The involved processing here is in computing the NL_OFFS entry
 * by maxing over the variants.  This works as follows.
 *
 * Each RECORD has two size counters.  NL_OFFS is the maximum size
 * so far of any variant of this record;  NL_FLDSZ gives the size
 * of just the FIELDs to this point as a base for further variants.
 *
 * As we process each variant record, we start its size with the
 * NL_FLDSZ we have so far.  After processing it, if its NL_OFFS
 * is the largest so far, we update the NL_OFFS of this subrecord.
 * This will eventually propagate back and update the NL_OFFS of the
 * entire record.
 */

/*
 * P0 points to the outermost RECORD for name searches.
 */
struct	nl *P0;

tyrec(r, off)
	int *r, off;
{

	    return (int)tyrec1(r, off, 1);
}

/*
 * Define a record namelist entry.
 * R is the tree for the record to be built.
 * Off is the offset for the first item in this (sub)record.
 */
struct nl *
tyrec1(r, off, first)
	register int *r;
	int off;
	char first;
{
	register struct nl *p, *P0was;

	p = defnl(0, RECORD, 0, 0);
	P0was = P0;
	if (first)
		P0 = p;
#ifndef PI0
	p->value[NL_FLDSZ] = p->value[NL_OFFS] = off;
#endif
	if (r != NIL) {
		fields(p, r[2]);
		variants(p, r[3]);
	}
	    /*
	     *	round the lengths of records up to their alignments
	     */
	p->value[NL_OFFS] = roundup(p->value[NL_OFFS], (long)align(p));
	P0 = P0was;
	return (p);
}

/*
 * Define the fixed part fields for p.
 */
struct nl *
fields(p, r)
	struct nl *p;
	int *r;
{
	register int *fp, *tp, *ip;
	struct nl *jp;

	for (fp = r; fp != NIL; fp = (int*)fp[2]) {
		tp = (int*)fp[1];
		if (tp == NIL)
			continue;
		jp = gtype(tp[3]);
		line = tp[1];
		for (ip = (int*)tp[2]; ip != NIL; ip = (int*)ip[2])
			deffld(p, ip[1], jp);
	}
}

/*
 * Define the variants for RECORD p.
 */
struct nl *
variants(p, r)
	struct nl *p;
	register int *r;
{
	register int *vc, *v;
	int *vr;
	struct nl *ct;

	if (r == NIL)
		return;
	ct = gtype(r[3]);
	if ( ( ct != NIL ) && ( isnta( ct , "bcsi" ) ) ) {
	    error("Tag fields cannot be %ss" , nameof( ct ) );
	}
	line = r[1];
	/*
	 * Want it even if r[2] is NIL so
	 * we check its type in "new" and "dispose"
	 * calls -- link it to NL_TAG.
	 */
	p->ptr[NL_TAG] = (int*) deffld(p, r[2], ct);
	for (vc = (int*)r[4]; vc != NIL; vc = (int*)vc[2]) {
		v = (int*)vc[1];
		if (v == NIL)
			continue;
		vr = (int*)tyrec1(v[3], p->value[NL_FLDSZ], 0);
#ifndef PI0
		if (((struct nl*)vr)->value[NL_OFFS] > ((struct nl*)p)->value[NL_OFFS])
			p->value[NL_OFFS] = ((struct nl*)vr)->value[NL_OFFS];
#endif
		line = v[1];
		for (v = (int*)v[2]; v != NIL; v = (int*)v[2])
			defvnt(p, v[1], vr, ct);
	}
}

/*
 * Define a field in subrecord p of record P0
 * with name s and type t.
 */
struct nl *
deffld(p, s, t)
	struct nl *p;
	register char *s;
	register struct nl *t;
{
	register struct nl *fp;

	if (reclook(P0, s) != NIL) {
#ifndef PI1
		error("%s is a duplicate field name in this record", s);
#endif
		s = NIL;
	}
#ifndef PI0
	    /*
	     * it used to be easy to keep track of offsets of fields
	     * and total sizes of records.
	     * but now, the offset of the field is aligned
	     * so only it knows it's offset, and calculating 
	     * the total size of the record is based on it,
	     * rather than just the width of the field.
	     */
	fp = enter(defnl(s, FIELD, t, (int)roundup(p->value[NL_OFFS],
			(long)align(t))));
#else
	fp = enter(defnl(s, FIELD, t, 0));
#endif
	if (s != NIL) {
		fp->chain = P0->chain;
		P0->chain = fp;
#ifndef PI0
		    /*
		     * and the size of the record is incremented.
		     */
		p -> value[ NL_OFFS ] = fp -> value[ NL_OFFS ] + width( t );
		p -> value[ NL_FLDSZ ] = p -> value[ NL_OFFS ];
#endif
		if (t != NIL) {
			P0->nl_flags |= t->nl_flags & NFILES;
			p->nl_flags |= t->nl_flags & NFILES;
		}
#		ifdef PC
		    stabfield( s , p2type( t ) , fp -> value[ NL_OFFS ]
				, lwidth( t ) );
#		endif PC
	}
	return (fp);
}

/*
 * Define a variant from the constant tree of t
 * in subrecord p of record P0 where the casetype
 * is ct and the variant record to be associated is vr.
 */
struct nl *
defvnt(p, t, vr, ct)
	struct nl *p, *vr;
	int *t;
	register struct nl *ct;
{
	register struct nl *av;

	gconst(t);
	if (ct != NIL && incompat(con.ctype, ct , t )) {
#ifndef PI1
		cerror("Variant label type incompatible with selector type");
#endif
		ct = NIL;
	}
	av = defnl(0, VARNT, ct, 0);
#ifndef PI1
	if (ct != NIL)
		uniqv(p);
#endif
	av->chain = (struct nl*)p->ptr[NL_VARNT];
	p->ptr[NL_VARNT] = (int*)av;
	av->ptr[NL_VTOREC] = (int*)vr;
	av->range[0] = con.crval;
	return (av);
}

#ifndef PI1
/*
 * Check that the constant label value
 * is unique among the labels in this variant.
 */
uniqv(p)
	struct nl *p;
{
	register struct nl *vt;

	for (vt = (struct nl*)p->ptr[NL_VARNT]; vt != NIL; vt = vt->chain)
		if (vt->range[0] == con.crval) {
			error("Duplicate variant case label in record");
			return;
		}
}
#endif

/*
 * See if the field name s is defined
 * in the record p, returning a pointer
 * to it namelist entry if it is.
 */
struct nl *
reclook(p, s)
	register struct nl *p;
	char *s;
{

	if (p == NIL || s == NIL)
		return (NIL);
	for (p = p->chain; p != NIL; p = p->chain)
		if (p->symbol == s)
			return (p);
	return (NIL);
}
