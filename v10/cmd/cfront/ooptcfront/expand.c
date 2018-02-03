/*ident	"@(#)ctrans:src/expand.c	1.3.4.20" */
/*****************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

expand.c:

	expand inline functions

******************************************************************/

#include "cfront.h"
Ptable Scope;
// extern void display_expr(Pexpr);

static int inline_name_count = 1;
char* temp(char* vn, Pname fn)
//
//	type.inline_temp_index is used to construct the name
//	in place of the address of fn. That way, cfront
//	output dosen't change as nodes move around. 
//	AT&T uses the address. --benson@odi
//	 - anything unique will do
//	add two bytes to make the name sensitive to the scope
//	to avoid re-using temps when an inline is expanded in
//	more than one other inline that is then called in the
//	same expression
//
{
    char temp[50];		/* Big enough for any purpose */

	if (vn[0]!='_' || vn[1]!='_' || vn[2]!='X') {
 		if(fn->tp->inline_temp_index == 0)
 		    fn->tp->inline_temp_index = inline_name_count ++;
 		// this used to be the low 3 bytes of fn->tp.
		// this may be slower, but it works.
		// tune later.
 		long a = fn->tp->inline_temp_index;
		sprintf(temp, "%lx", a);
		int al = strlen(temp);
		// allocate memory for the result
		int ll = strlen(vn);
		char* s = new char[ll+al+8];
		register char* p = s;

		// append _ _ X vn
		*p++ = '_';
		*p++ = '_';
		*p++ = 'X';
		strcpy(p,vn);
		p += ll;
		strcat(p,temp);
		p += al;
		// append scope representation and trailing null
		*p++ = 'a' + ((int(Scope)>>4)&15);
		*p++ = 'a' + ((int(Scope)>>8)&15);
		*p = '\0';
		return s;
	}
	else
		return vn;

}

Pname dcl_local(Ptable scope, Pname an, Pname fn)
{
	if (scope == 0) {
		if (sti_tbl == 0) sti_tbl = new table(8,gtbl,0);
		scope = sti_tbl;
	//	error('s',"cannot expand inlineF needing temporaryV in nonF context");
	//	return an;
	}

	if (an->n_stclass == STATIC) {
		if (an->tp->base!=FCT) error('s',"cannot expand inlineF with static%n",an);
		return an;
	}

	Pname cn = fn->n_table->t_name;
	char* s = temp(an->string,fn);
	Pname nx = new name(s);
	Ptype atp = an->tp;
//error('d',"dcl_local(%d,%n,%n) -> %s",scope,an,fn,s);
	while (atp->base == TYPE) atp = Pbase(atp)->b_name->tp;
	if (atp->base == VEC) {
		if (an->n_scope == ARG) {
			Pptr t = new ptr(PTR,Pvec(atp)->typ);
			nx->tp = t;
		}
		else {
			error('s',"cannot expand inlineF needing temporaryV of vectorT");
			return an;
		}
	}
	else
		nx->tp = atp;
	PERM(nx->tp);

	nx->n_used = an->n_used;
	nx->n_assigned_to = an->n_assigned_to;
	nx->n_addr_taken = an->n_addr_taken;
	nx->n_xref = an->n_xref;
//error('d',"nx %n %t,",nx,nx->tp);
	Pname r = scope->look(nx->string,0);
	if (r) {
//error('d',"ll %n %t",ll,ll->tp);
		if (r->tp->check(nx->tp,0))
			error('s',"cannot expand inlineF %n with two local variables with the sameN (%s)",fn,an->string);
		else
			r = 0;
	}
	if (r==0) {
		r = scope->insert(nx,0);
//error('d',"%d %d %d %d",r->n_stclass,an->n_stclass,r->lex_level,an->lex_level);
		r->n_stclass = an->n_stclass;
		r->lex_level = an->lex_level;
		delete nx;
		r->where.line = 0;
	}
	return r;
}

int ck_cast(Ptype t1, Ptype t2)
/*
	return a value of type t2 from a function returning a t1
	return 1 if cast is needed
*/
{
st:
	while (t1->base == TYPE) t1 = Pbase(t1)->b_name->tp;
	while (t2->base == TYPE) t2 = Pbase(t2)->b_name->tp;

	if (t1 == t2) return 0;

	if (t1->base != t2->base) return 1;

	switch (t1->base) {
	case RPTR:
	case PTR:
	{	t1 = Pptr(t1)->typ;
		if (t1->base == VOID) return 1;
		t2 = Pptr(t2)->typ;
		goto st;
	}
	case CHAR:
	case SHORT:
	case INT:
	case LONG:
		if (Pbase(t1)->b_unsigned != Pbase(t2)->b_unsigned) return 1;
		break;
	case COBJ:
	{
		Pname nn = Pbase(t1)->b_name;

		if (Pclass(nn->tp)->csu==UNION ) return 0; 

		if (t2->base==COBJ && nn->tp==Pbase(t2)->b_name->tp) return 0;

		return 1;
	}
	}

	return 0;
}

int makeCM( Ptype t )
/* return 1 if the type is not a primitive type
 * this will cause the generation of (t,0) 
 * note: ``t'' is guaranteed not to be 0
 */
{
    while ( t->base == TYPE )
	t = Pbase(t)->b_name->tp;

    switch (t->base) {
	case FLOAT:
	case DOUBLE:
	case LDOUBLE:
	case CHAR:
	case SHORT:
	case INT:
	case LONG:
	case EOBJ:
		return 0;
	default: 
		return 1;
	}
}

static ret_seen = 0;

Pstmt stmt::expand()
/*
	copy the statements with the formal arguments replaced by ANAMES 

	called once only per inline function
	expand_tbl!=0 if the function should be transformed into an expression
	and expand_tbl is the table for local variables
*/
{
	if (this == 0) error('i',"0->S::expand() for%n",expand_fn);

	if (memtbl) {	/* check for static variables */
		register Ptable t = memtbl;
		register int i;
		for (register Pname n = t->get_mem(i=1); n; n=t->get_mem(++i)) {
			if (n->n_stclass == STATIC) {
				if (n->tp->base == FCT) continue;
				error('s',"cannot expand inlineF with static%n",n);
				n->n_stclass = AUTO;
			}
			n->where.line = 0;
		}
	}

	if (expand_tbl) {	/* make expression */
		Pexpr ee;

		if (memtbl) { // temporaries
			int i;
			for (Pname n=memtbl->get_mem(i=1); n; n=memtbl->get_mem(++i)) {
//error('d',"block %n %k %d %d",n,base,memtbl->real_block == this,n->lex_level);
				if (n->base!=NAME || n->tp==any_type) continue;

				if (base==BLOCK 
				&& memtbl->real_block == this 
				&& n->lex_level < 2
				&& (n->string[0]!='_'	// promoted from called
							// inlines
					|| n->string[1]!='_'
					|| n->string[2]!='X')
				)
					continue;

 				if (memtbl != scope)
 				{
 			 		Pname nn = dcl_local(scope,n,expand_fn);
					nn->base = NAME;
					n->string = nn->string;
				}
				else if (n->tp->base!=FCT && n->tp->base!=OVERLOAD){
 					n->string = temp(n->string, expand_fn);
 					n->where.line = 0;
 				}
			}
		}

		switch (base) {
		default:
			error('s',"cannot expand inlineF%n with %kS in inline",expand_fn,base);
			return Pstmt(dummy);

		case BLOCK:
			if (s_list) {
				ee = Pexpr(s_list->expand());
				if (s) {
					ee = new expr(CM, Pexpr(s->expand()), ee);
					ee->tp = ee->e2->tp;
					PERM(ee);
				}
				return Pstmt(ee);
			}

			if (s) return s->expand();

			return Pstmt(zero);

		case PAIR:
			ee = s2 ? Pexpr(s2->expand()) : 0;
			ee = new expr(CM, s?Pexpr(s->expand()):0, ee);
			ee->tp = ee->e2->tp;
			if (s_list) {
				ee = new expr(CM, ee, Pexpr(s_list->expand()));
				ee->tp = ee->e2->tp;
			}
			PERM(ee);
			return Pstmt(ee);

		case RETURN:
			ret_seen = 1;
			s_list = 0;
		//	return Pstmt(e ? e->expand() : zero);

			if (e == 0) 
				ee = zero;
			else {
				ee = e->expand();
				Ptype tt = Pfct(expand_fn->tp)->returns;
				if (tt == 0) tt = Pfct(expand_fn->tp)->returns;
				if (tt!=ee->tp && ck_cast(tt,ee->tp)) ee = new cast(tt,ee);
			}
	
			return Pstmt(ee);

		case SM:
			if (e== 0 || e==dummy)
				ee = zero;
			else {
				if (e->base == DEREF) e = e->e1;
				ee = e->expand();
			}
			// no break;

		case ASM:
			if (s_list) {
				ee = new expr(CM, ee, (Pexpr)s_list->expand());
				ee->tp = ee->e2->tp;
				PERM(ee);
			}
			return (Pstmt)ee;

		case IF:
		{
			int ors = ret_seen;
			ret_seen = 0;
			Pexpr qq = new expr(QUEST,Pexpr(s->expand()),0);
			int ret1 = ret_seen;
			ret_seen = 0;

			qq->cond = e->expand();
			qq->e2 = else_stmt ? Pexpr(else_stmt->expand()) : zero;
			int ret2 = ret_seen; 
			if (ret1+ret2 && s_list) {
				error('s',"cannot expand inlineF%n with S after \"return\"",expand_fn);
				ret_seen = 0;
			}
			ret_seen += ors;

			// handle ``if (x) class_object_valued_expression;''

			Ptype t1 = qq->e1->tp;
			Ptype t2 = qq->e2->tp;

	if (t1 && t1->base==FCT)
		t1 = Pfct(t1)->s_returns ? Pfct(t1)->s_returns : Pfct(t1)->returns;

	if (t2 && t2->base==FCT)
		t2 = Pfct(t2)->s_returns ? Pfct(t2)->s_returns : Pfct(t2)->returns;

			Pname c1 = t1?t1->is_cl_obj():0;
			Pname c2 = t2?t2->is_cl_obj():0;

			int z1 = c1 && c1!=c2;
			int z2 = c2 && c1!=c2;

//error('d',"t1 %t t2 %t",t1,t2);
			if (c1==0
			&& c2==0
			&& t1
			&& t2
			&& t2->check(t1,ASSIGN)
			&& t1->check(t2,ASSIGN)) {
				z1 = makeCM( t1 );
				z2 = makeCM( t2 );
				if ((z1 && ret1) || (z2 && ret2))
					error('s',"cannot expand inlineF with return in ifS");
			}

			if (t1 && z1==0) {
				// since zero is acceptable to all pointer types
				// we need only ``fix'' z1 and z2 will take care
				// of itself
				z1 = t1->is_ptr_or_ref()
					&& t2
					&& t2->is_ptr_or_ref()
					&& t1->check(t2,0);
			}

			if (z1) {
				Pexpr v = (z2==0 && (t2->is_ptr_or_ref())) ? new cast(t2,zero) : zero;
				qq->e1 = new expr(CM,qq->e1,v);
				qq->e1->tp = qq->e1->e1->tp;
			}

			if (z2) {
				Pexpr v = (z1==0 && (t1->is_ptr_or_ref())) ? new cast(t1,zero) : zero;
				qq->e2 = new expr(CM,qq->e2,v);
				qq->e2->tp = qq->e2->e1->tp;
			}

			if (s_list) {
				qq = new expr(CM,qq,Pexpr(s_list->expand()));
				qq->tp = qq->e2->tp;
			}
			else
				qq->tp = qq->e1->tp;
			PERM(qq);
			return Pstmt(qq);
		}
		}
	}

	where.line = 0;

	switch (base) {
	default:
		if (e) e = e->expand();
		break;
	case PAIR:
		if (s2) s2 = s2->expand();
		break;
	case BLOCK:
		break;
	case FOR:
		if (for_init) for_init = for_init->expand();
		if (e2) e2 = e2->expand();
	case ASM:
		break;
//	case LABEL:
//	case GOTO:
	case RETURN:
//	case BREAK:
//	case CONTINUE:
		error('s',"cannot expand inlineF%n with %kS",expand_fn,base);
	}

	if (s) s = s->expand();
	if (s_list) s_list = s_list->expand();
	PERM(this);
	return this;
}

Pexpr expr::expand()
{
	if (this == 0) error('i',"E::expand(0)");

	switch (base) {
	case NAME:
		if (expand_tbl && Pname(this)->n_scope==FCT) {
			Pname n = Pname(this);
			char* s = n->string;
			if (s[0]=='_' && s[1]=='_' && s[2]=='X') break;
			Pname cn = expand_fn->n_table->t_name;
		//	n->string = temp(s,expand_fn->string,(cn)?cn->string:0);
			n->string = temp(s,expand_fn);
		}
	case DUMMY:
	case ICON:
	case FCON:
	case CCON:
	case IVAL:
	case FVAL:
	case LVAL:
	case STRING:
	case ZERO:
	case TEXT:
	case ANAME:
	case MDOT:
		break;
	case ICALL:
		if (expand_tbl && e1==0) {
			int ors = ret_seen;
			ret_seen = 0;
			Pname fn = il->fct_name;
			Pfct f = Pfct(fn->tp);
			if (f->f_expr==0
		// f->returns==void_type
		//	&& f->s_returns!=int_type
		//	&& fn->n_oper!=CTOR
			)
				error('s',"cannot expand value-returning inline%n with call of non-value-returning inline%n",expand_fn,fn);
		//	else
		//		error("inline%n called before defined",fn);
ret_seen = ors;
		}
		break;
	case SIZEOF:
	case CAST:
		if (tp2) PERM(tp2);
		goto rrr;
	case QUEST:
		cond = cond->expand();
	default:
		if (e2) e2 = e2->expand();
	case REF:
	case DOT:
	rrr:
		if (e1) e1 = e1->expand();
		break;
	}

	switch (base) {
	case CM:
	case G_CM:
		if (tp==0) tp=e2->tp;
	}

	PERM(this);
	return this;
}

int nin;

bit expr::not_simple()
/*
	is a temporary variable needed to hold the value of this expression
	as an argument for an inline expansion?
	return 1; if side effect
	return 2; if modifies expression
*/
{
	int s;
//error('d',"not_simple%k",base);
	switch (base) {
	default:
		return 2;
	case NAME:
		if (nin==0 && Pname(this)->n_table==gtbl) return 2;	// unsafe: aliasing
	case ZERO:
	case IVAL:
	case FVAL:
	case ICON:
	case CCON:
	case FCON:
	case STRING:
		return 0;
	case MDOT:
		return mem->not_simple();
	case SIZEOF:
		return (e1==0 || e1==dummy) ? 0 : e1->not_simple();
	case G_ADDROF:
	case ADDROF:
		return e2->not_simple();
	case CAST:
	case DOT:
	case REF:
		return e1->not_simple();
	case UMINUS:
	case NOT:
	case COMPL:
		return e2->not_simple();
	case DEREF:
		s = e1->not_simple();
		if (1<s) return 2;
		if (e2==0) return s;
		return s |= e2->not_simple();
	case MUL:
	case DIV:
	case MOD:
	case PLUS:
	case MINUS:
	case LS:
	case RS:
	case AND:
	case OR:
	case ER:
	case LT:
	case LE:
	case GT:
	case GE:
	case EQ:
	case NE:
	case ANDAND:
	case OROR:
	case CM:
		s = e1->not_simple();
		if (1<s) return 2;
		return s |= e2->not_simple();
	case QUEST:
		s = cond->not_simple();
		if (1<s) return 2;
		s |= e1->not_simple();
		if (1<s) return 2;
		return s |= e2->not_simple();
	case ANAME:
		if (curr_icall) {
			Pname n = (Pname)this;
			int argno = int(n->n_val);
			for (Pin il=curr_icall; il; il=il->i_next)
				if (n->n_table == il->i_table) goto aok;
			goto bok;
		aok:
			return (il->i_args[argno].local) ? 0 : il->i_args[argno].arg->not_simple();
		}
	bok:	error('i',"expand aname%n",this);
	case G_CM:
	case VALUE:
	case NEW:
	case GNEW:
	case CALL:
	case G_CALL:
	case ICALL:
	case ASSIGN:
	case INCR:
	case DECR:
	case ASPLUS:
	case ASMINUS:
	case ASMUL:
	case ASDIV:
	case ASMOD:
	case ASAND:
	case ASOR:
	case ASER:
	case ASLS:
	case ASRS:
		return 2;
	}
}

extern void uninline(Pname fn);
//extern Pname new_fct;	
//extern Pname del_fct;
extern Pstmt del_list;
extern Pstmt break_del_list;
extern Pstmt continue_del_list;
extern Pname curr_fct;
extern Pexpr init_list;

extern Pname make_default_ctor(Pclass cl);
void expand_itor(Pclass cl)
{
//error('d',"expand_itor(%t)",cl);
	Pexpr s2 = curr_expr;
	Pstmt s5 = del_list;
	Pstmt s6 = break_del_list;
	Pstmt s7 = continue_del_list;
	Pname s8 = curr_fct;
	Pexpr s9 = init_list;
	(void) cl->make_itor(1);
	curr_expr = s2;
	del_list = s5;
	break_del_list = s6;
	continue_del_list = s7;
	curr_fct = s8;
	init_list = s9;
}
/*
extern Pname make_default_dtor(Pclass cl);
void expand_dtor(Pclass cl)
{
//error('d',"expand_itor(%t)",cl);
	Pexpr s2 = curr_expr;
	Pstmt s5 = del_list;
	Pstmt s6 = break_del_list;
	Pstmt s7 = continue_del_list;
	Pname s8 = curr_fct;
	Pexpr s9 = init_list;
	(void) make_default_dtor(cl);
	curr_expr = s2;
	del_list = s5;
	break_del_list = s6;
	continue_del_list = s7;
	curr_fct = s8;
	init_list = s9;
}


void expand_ictor(Pclass cl)
{
//error('d',"expand_itor(%t)",cl);
	Pexpr s2 = curr_expr;
	Pstmt s5 = del_list;
	Pstmt s6 = break_del_list;
	Pstmt s7 = continue_del_list;
	Pname s8 = curr_fct;
	Pexpr s9 = init_list;
	(void) make_default_ctor(cl);
	curr_expr = s2;
	del_list = s5;
	break_del_list = s6;
	continue_del_list = s7;
	curr_fct = s8;
	init_list = s9;
}
*/
Pexpr fct::expand(Pname fn, Ptable scope, Pexpr ll)
/*
	expand call to (previously defined) inline function in "scope"
	with the argument list "ll"
	(1) declare variables in "scope"
	(2) initialize argument variables
	(3) link to body
*/
{
	Scope = scope;
//error('d',"expand%s() body %d f_expr %d inline %d",fn->string,body,f_expr,f_inline);
	if (f_inline == ITOR) {
		Pexpr s1 = last_expanded;
		expand_itor(memof);
		last_expanded = s1;
		this = Pfct(fn->tp);
		if (f_inline==0 && f_imeasure) {
			uninline(fn);
			return 0;
		}
	}
/*
	if (f_inline == ICTOR) {
		Pexpr s1 = last_expanded;
		expand_ictor(memof);
		last_expanded = s1;
		this = Pfct(fn->tp);
		if (f_inline==0 && f_imeasure) {
			uninline(fn);
			return 0;
		}
	}
	if (f_inline == IDTOR) {
		Pexpr s1 = last_expanded;
		expand_dtor(memof);
		last_expanded = s1;
		this = Pfct(fn->tp);
		if (f_inline==0 && f_imeasure) {
			uninline(fn);
			return 0;
		}
	}
*/
//error('d',"expand %n body %d f_expr %d defined %d",fn,body,f_expr,defined);
//error('d',"inline %d scope %d %d",f_inline,Pfct(fn->tp)->body->memtbl,scope);
	if ((body==0 && f_expr==0)			// before defined
	||  ((defined&SIMPLIFIED)==0)			// before simplified
	||  (Pfct(fn->tp)->body->memtbl==scope)		// while defining
	||  (f_inline>1)				// recursive call
//	||  (last_expanded && last_expanded==curr_expr)	// twice in an expression
	) {						// so don't expand
		if (warning_opt) error('w',"cannot inline%n in thisE",fn);
		if (fn->n_addr_taken++==0) fn->dcl_print(0);
		return 0;
	}

	if (fn->n_oper==CTOR) {
		Pclass cl = Pclass(fn->n_table->t_name->tp);
		if (cl->c_body == 3) cl->print_all_vtbls(cl);
	}

	Pin il = new iline;
	Pexpr ic = new texpr(ICALL,0,0);
	int ns = 0;
	for (Pname an = f_args; an; an=an->n_list) ns++;
	il->fct_name = fn;
	il->i_args = new ia[il->i_slots = ns];
	ic->il = il;
	ic->tp = s_returns ? s_returns : returns;
//	Pname at = (f_this) ? f_this : (f_result) ? f_result : argtype;
	Pname at = f_args;

	f_inline++;

	if (at) il->i_table = at->n_table;
	int i = 0;
	int not_simple = 0;	/* is a temporary argument needed? */

	for (Pname n=at; n; n=n->n_list, i++) {
		/*	check formal/actual argument pairs
			and generate temporaries as necessary
		*/
		if (ll == 0) error('i',"F::expand(%n):AX",fn);
		Pexpr ee;

		if (ll->base == ELIST) {
			ee = ll->e1;
			ll = ll->e2;
		}
		else {
			ee = ll;
			ll = 0;
		}

		/* could be avoided when expanding into a block */
		il->i_args[i].local = 0;
		int s ;

 	// try to protect agains aliasing through pointers and references
	for (Pname m=at; m; m=m->n_list) {
		Pptr p;
		if (n!=m) {
		//	if ((p=m->tp->is_ptr()) || (p=m->tp->is_ref()))
			if (p=m->tp->is_ptr_or_ref())
				if (p->check(n->tp,0)==0 || p->typ->check(n->tp,0)==0) goto zxc;

		}
	}

		if (n->n_assigned_to==FUDGE111
			&& ee!=zero
			&& ee->not_simple()==0)
			;
		else if (n->n_addr_taken || n->n_assigned_to) 
			goto zxc;
		else if (s=ee->not_simple()) {
			if (/*n->n_used==0	// n_used not set for ``this''
			|| */1<s
			|| 1<n->n_used ) {	// not safe
		zxc:
if (last_expanded && last_expanded==curr_expr) {
	f_inline--;
	delete il->i_args;
	delete il;
	if (fn->n_addr_taken++==0) fn->dcl_print(0);
	return 0;
}
//error('d',"zxc %n %t ee %d %t",n,n->tp,ee->base,ee->tp);
				Pname nn = dcl_local(scope,n,fn);
				nn->base = NAME;
				il->i_args[i].local = nn;
				++not_simple;
			//	if (nn->tp->is_ref()
			//	&& ee->tp
			//	&& ee->tp->is_ptr_or_ref()==0) ee = ee->address();
			}
		}

		il->i_args[i].arg = ee;
		il->i_args[i].tp = n->tp;
	}
	
	Ptable tbl = body->memtbl;
	if (f_expr) {		// generate comma expression
		
		char loc_var = 0;

		/* look for local variables needing declaration: */
		for (n=tbl->get_mem(i=1); n; n=tbl->get_mem(++i) ) {
//error('d',"n %n %d",n,n->base);
//error('d',"loc %n %d %d %d",n,n->n_used,n->n_assigned_to,n->n_addr_taken);
			if (n->base==NAME	// don't re-declare the args
			&& (n->tp->base!=FCT && n->tp->base!=OVERLOAD)
			&& (n->n_used || n->n_assigned_to || n->n_addr_taken)) {
if (last_expanded && last_expanded==curr_expr) {
	f_inline--;
	delete il->i_args;
	delete il;
	if (fn->n_addr_taken++==0) fn->dcl_print(0);
	return 0;
}
				Pname nn = dcl_local(scope,n,fn);
				nn->base = NAME;
				n->string = nn->string;

				loc_var++;
			}
		}

		if (i /*not_simple*/ || loc_var) last_expanded = curr_expr;

		Pexpr ex;
		if (not_simple) {
			Pexpr etail = ex = new expr(CM,0,0);
			for (i=0; i<il->i_slots; i++) {
				Pname n = il->i_args[i].local;
				if (n == 0) continue;
				Pexpr e = il->i_args[i].arg;
					// if used assign
					// otherwise (e.g. unused argument)
					// simply evaluate for sideeffects
				if (n->n_used
				|| n->n_assigned_to
				|| n->n_addr_taken) {
Pexpr mptr_assign(Pexpr n, Pexpr in);
				Pptr p1 = n->tp->is_ptr();
                                Pptr p2 = e->tp ? e->tp->is_ptr() : 0;
				if (p1 && p1->memof && p1!=p2) {
					switch (e->base) {
					case CM:
					case G_CM:
						e->e2 = mptr_assign(n,e->e2);
						break;
					default:
						e = mptr_assign(n,e);
					}
					etail->e1 = e;
				}
				else
					etail->e1 = new expr(ASSIGN,n,e);
				}
				else
					etail->e1 = e;
				if (--not_simple)
					etail = etail->e2 = new expr(CM,0,0);
				else
					break;
			}
			etail->e2 = f_expr;
		}
		else
			ex = f_expr;
		ic->e1 = ex;
	}
	else {		// generate block:
		for (n=tbl->get_mem(i=1); n; n=tbl->get_mem(++i) ) {
			// mangle local names
			if (n->base==NAME
			&& (n->n_used || n->n_assigned_to || n->n_addr_taken)) {
			//	Pname cn = fn->n_table->t_name;
			//	n->string = temp(n->string,fn->string,(cn)?cn->string:0);
				n->string = temp(n->string,fn);
			}
		}
		Pstmt ss;
		if (not_simple) {
			last_expanded = curr_expr;
			Pstmt st = new estmt(SM,curloc,0,0);
			st->where.line = 0;
			Pstmt stail = st;
			for (i=0; i<il->i_slots; i++) {
				Pname n = il->i_args[i].local;
				if (n == 0) continue;
				Pexpr e = il->i_args[i].arg;
				Pptr p1 = n->tp->is_ptr();
				Pptr p2 = e->tp->is_ptr();
				Pexpr mptr_assign(Pexpr n, Pexpr in);
				if (p1 && p1->memof && p1!=p2) {
					switch (e->base) {
					case CM:
					case G_CM:
						e->e2 = mptr_assign(n,e->e2);
						break;
					default:
						e = mptr_assign(n,e);
					}
					stail->e = e;
				}
				else
					stail->e = new expr(ASSIGN,n,e);
				if (--not_simple) {
					stail = stail->s_list = new estmt(SM,curloc,0,0);
					stail->where.line = 0;
				}
				else
					break;
			}
			stail->s_list = body;
			ss = new block(curloc,0,st);
			ss->where.line = 0;
		}
		else
			ss = body;
		ic->e2 = Pexpr(ss);
	}

	f_inline--;
	return ic;
}

/* ODI Notes

   change address sensitive name to be address-insensitive
   that way cfront output can be diffed to regress the compiler.
*/
