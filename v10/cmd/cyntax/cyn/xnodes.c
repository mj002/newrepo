/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

#include	"defs.h"

xdopes		xdope[]	=
{
/* xt_and */		{"&",		pack(su_and)},
/* xt_andand */		{"&&",		pack(su_null)},
/* xt_andeq */		{"&=",		pack(su_null)},
/* xt_argident */	{"",		pack(su_null)},
/* xt_argname */	{"",		pack(su_null)},
/* xt_arrayof */	{"",		pack(su_arrayof)},
/* xt_bar */		{"|",		pack(su_bar)},
/* xt_barbar */		{"||",		pack(su_null)},
/* xt_bareq */		{"|=",		pack(su_null)},
/* xt_basetype */	{"",		pack(su_basetype)},
/* xt_bitfield */	{"",		pack(su_bitfield)},
/* xt_block */		{"",		pack(su_null)},
/* xt_caddr */		{"",		pack(su_null)},
/* xt_call */		{"",		pack(su_null)},
/* xt_cast */		{"cast",	pack(su_null)},
/* xt_cchar */		{"",		pack(su_null)},
/* xt_cdouble */	{"",		pack(su_null)},
/* xt_cfloat */		{"",		pack(su_null)},
/* xt_cint */		{"",		pack(su_null)},
/* xt_class */		{"",		pack(su_null)},
/* xt_clong */		{"",		pack(su_null)},
/* xt_cnull */		{"",		pack(su_null)},
/* xt_colon */		{":",		pack(su_null)},
/* xt_comma */		{",",		pack(su_null)},
/* xt_cuint */		{"",		pack(su_null)},
/* xt_culong */		{"",		pack(su_null)},
/* xt_do */		{"",		pack(su_null)},
/* xt_dot */		{".",		pack(su_null)},
/* xt_enum */		{"",		pack(su_enum)},
/* xt_eq */		{"=",		pack(su_null)},
/* xt_eqeq */		{"==",		pack(su_null)},
/* xt_expr */		{"",		pack(su_null)},
/* xt_extf */		{"",		pack(su_null)},
/* xt_ftnreturning */	{"",		pack(su_ftnreturning)},
/* xt_grgreq */		{">>=",		pack(su_null)},
/* xt_grt */		{">",		pack(su_null)},
/* xt_grteq */		{">=",		pack(su_null)},
/* xt_grtgrt */		{">>",		pack(su_null)},
/* xt_if */		{"",		pack(su_null)},
/* xt_insf */		{"",		pack(su_null)},
/* xt_less */		{"<",		pack(su_null)},
/* xt_lesseq */		{"<=",		pack(su_null)},
/* xt_lessless */	{"<<",		pack(su_null)},
/* xt_literal */	{"",		pack(su_null)},
/* xt_lslseq */		{"<<=",		pack(su_null)},
/* xt_maxof */		{"",		pack(su_null)},
/* xt_memblist */	{"",		pack(su_null)},
/* xt_minof */		{"",		pack(su_null)},
/* xt_minus */		{"-",		pack(su_plus)},
/* xt_minuseq */	{"-=",		pack(su_null)},
/* xt_name */		{"",		pack(su_null)},
/* xt_not */		{"!",		pack(su_null)},
/* xt_noteq */		{"!=",		pack(su_null)},
/* xt_null */		{"",		pack(su_null)},
/* xt_obj */		{"",		pack(su_null)},
/* xt_percent */	{"%",		pack(su_null)},
/* xt_pereq */		{"%=",		pack(su_null)},
/* xt_plus */		{"+",		pack(su_plus)},
/* xt_pluseq */		{"+=",		pack(su_null)},
/* xt_postdec */	{"--",		pack(su_null)},
/* xt_postinc */	{"++",		pack(su_null)},
/* xt_predec */		{"--",		pack(su_null)},
/* xt_preinc */		{"++",		pack(su_null)},
/* xt_ptrdiff */	{"-",		pack(su_null)},
/* xt_ptrminus */	{"-",		pack(su_null)},
/* xt_ptrminuseq */	{"-=",		pack(su_null)},
/* xt_ptrplus */	{"+",		pack(su_null)},
/* xt_ptrpluseq */	{"+=",		pack(su_null)},
/* xt_ptrpostdec */	{"--",		pack(su_null)},
/* xt_ptrpostinc */	{"++",		pack(su_null)},
/* xt_ptrpredec */	{"--",		pack(su_null)},
/* xt_ptrpreinc */	{"++",		pack(su_null)},
/* xt_ptrto */		{"",		pack(su_ptrto)},
/* xt_question */	{"?",		pack(su_null)},
/* xt_sizeof */		{"",		pack(su_null)},
/* xt_slash */		{"/",		pack(su_null)},
/* xt_slasheq */	{"/=",		pack(su_null)},
/* xt_sref */		{"",		pack(su_null)},
/* xt_string */		{"",		pack(su_null)},
/* xt_structof */	{"",		pack(su_structof)},
/* xt_svalue */		{"",		pack(su_null)},
/* xt_tilda */		{"~",		pack(su_null)},
/* xt_times */		{"*",		pack(su_times)},
/* xt_timeseq */	{"*=",		pack(su_null)},
/* xt_uand */		{"&",		pack(su_null)},
/* xt_uminus */		{"-",		pack(su_plus)},
/* xt_unionof */	{"",		pack(su_unionof)},
/* xt_uparrow */	{"^",		pack(su_uparrow)},
/* xt_uparroweq */	{"^=",		pack(su_null)},
/* xt_utimes */		{"*",		pack(su_null)},
/* xt_while */		{"",		pack(su_null)},
};
