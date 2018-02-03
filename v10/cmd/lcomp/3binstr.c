From research!whuxld!bjb Thu Mar  8 13:23 EST 1984*
To: research!seki!pjw

/*
 *	static char *ID_opsout = "@(#) ops.out: 1.5 4/26/83";
 */
{"mcomb",	IS25|INSTRB,	DYAOP1-256,	8,	0x8bL},
{"mcomh",	IS25|INSTRH,	DYAOP1-256,	8,	0x8aL},
{"mcomw",	IS25|INSTRW,	DYAOP1-256,	8,	0x88L},
{"movzbh",	IS25|INSTRBH,	DYAOP7-256,	8,	0x87L},
{"movzbw",	IS25|INSTRBW,	DYAOP7-256,	8,	0x87L},
{"movzhw",	IS25|INSTRHW,	DYAOP7-256,	8,	0x86L},
{"pushzb",	IS25|INSTRB,	PUSHOP1-256,	8,	0x87L},
{"pushzh",	IS25|INSTRHW,	PUSHOP1-256,	8,	0x84L},
{"movaw",	IS25|INSTRW,	DYAOP2-256,	8,	0x4L},
{"pushaw",	IS25|INSTRW,	MONOP2-256,	8,	0xe0L},
{"andb2",	IS25|INSTRB,	DYAOP1-256,	8,	0xbbL},
{"andh2",	IS25|INSTRH,	DYAOP1-256,	8,	0xbaL},
{"andw2",	IS25|INSTRW,	DYAOP1-256,	8,	0xb8L},
{"andb3",	IS25|INSTRB,	TRIOP1-256,	8,	0xfbL},
{"andh3",	IS25|INSTRH,	TRIOP1-256,	8,	0xfaL},
{"andw3",	IS25|INSTRW,	TRIOP1-256,	8,	0xf8L},
{"orb2",	IS25|INSTRB,	DYAOP1-256,	8,	0xb3L},
{"orh2",	IS25|INSTRH,	DYAOP1-256,	8,	0xb2L},
{"orw2",	IS25|INSTRW,	DYAOP1-256,	8,	0xb0L},
{"orb3",	IS25|INSTRB,	TRIOP1-256,	8,	0xf3L},
{"orh3",	IS25|INSTRH,	TRIOP1-256,	8,	0xf2L},
{"orw3",	IS25|INSTRW,	TRIOP1-256,	8,	0xf0L},
{"xorb2",	IS25|INSTRB,	DYAOP1-256,	8,	0xb7L},
{"xorh2",	IS25|INSTRH,	DYAOP1-256,	8,	0xb6L},
{"xorw2",	IS25|INSTRW,	DYAOP1-256,	8,	0xb4L},
{"xorb3",	IS25|INSTRB,	TRIOP1-256,	8,	0xf7L},
{"xorh3",	IS25|INSTRH,	TRIOP1-256,	8,	0xf6L},
{"xorw3",	IS25|INSTRW,	TRIOP1-256,	8,	0xf4L},
{"bitb",	IS25|INSTRB,	CMPOP2-256,	8,	0x3bL},
{"bith",	IS25|INSTRH,	CMPOP2-256,	8,	0x3aL},
{"bitw",	IS25|INSTRW,	CMPOP2-256,	8,	0x38L},
{"llsw2",	IS25|INSTRW,	DYAOP3-256,	16,	0xd0L},
{"llsw3",	IS25|INSTRW,	SHFTOP1-256,	16,	0xd0L},
{"lrsw2",	IS25|INSTRBW,	DYAOP3-256,	16,	0xd4L},
{"lrsw3",	IS25|INSTRBW,	SHFTOP1-256,	16,	0xd4L},
{"movb",	IS25|INSTRB,	DYAOP4-256,	16,	0x8387L},
{"movh",	IS25|INSTRH,	DYAOP4-256,	16,	0x8286L},
{"movw",	IS25|INSTRW,	DYAOP4-256,	16,	0x8084L},
{"movbbh",	IS25|INSTRBH,	DYAOP5-256,	16,	0x87E7L},
{"movbbw",	IS25|INSTRBW,	DYAOP5-256,	16,	0x87E7L},
{"movbhw",	IS25|INSTRHW,	DYAOP5-256,	8,	0x86L},
{"movthb",	IS25|INSTRHB,	DYAOP5-256,	8,	0x86L},
{"movtwb",	IS25|INSTRWB,	DYAOP5-256,	8,	0x84L},
{"movtwh",	IS25|INSTRWH,	DYAOP5-256,	8,	0x84L},
{"mnegh",	IS25|INSTRH,	DYAOP1-256,	8,	0x8eL},
{"mnegw",	IS25|INSTRW,	DYAOP1-256,	8,	0x8cL},
{"pushbb",	IS25|INSTRBW,	PUSHOP1-256,	8,	0x87L},
{"pushbh",	IS25|INSTRH,	PUSHOP1-256,	8,	0x86L},
{"pushw",	IS25|INSTRW,	MONOP1-256,	8,	0xa0L},
{"addb2",	IS25|INSTRB,	DYAOP6-256,	16,	0x939fL},
{"addh2",	IS25|INSTRH,	DYAOP6-256,	16,	0x929eL},
{"addw2",	IS25|INSTRW,	DYAOP6-256,	16,	0x909cL},
{"addb3",	IS25|INSTRB,	TRIOP1-256,	8,	0xdfL},
{"addh3",	IS25|INSTRH,	TRIOP1-256,	8,	0xdeL},
{"addw3",	IS25|INSTRW,	TRIOP1-256,	8,	0xdcL},
{"subb2",	IS25|INSTRB,	DYAOP6-256,	16,	0x97bfL},
{"subh2",	IS25|INSTRH,	DYAOP6-256,	16,	0x96beL},
{"subw2",	IS25|INSTRW,	DYAOP6-256,	16,	0x94bcL},
{"subb3",	IS25|INSTRB,	TRIOP1-256,	8,	0xffL},
{"subh3",	IS25|INSTRH,	TRIOP1-256,	8,	0xfeL},
{"subw3",	IS25|INSTRW,	TRIOP1-256,	8,	0xfcL},
{"mulw2",	IS25|INSTRW,	DYAOP1-256,	8,	0xa8L},
{"umulw2",	IS25|INSTRW,	DYAOP8-256,	8,	0xa8L},
{"mulw3",	IS25|INSTRW,	TRIOP1-256,	8,	0xe8L},
{"umulw3",	IS25|INSTRW,	TRIOP2-256,	8,	0xe8L},
{"divw2",	IS25|INSTRW,	DYAOP1-256,	8,	0xacL},
{"udivw2",	IS25|INSTRW,	DYAOP8-256,	8,	0xacL},
{"divw3",	IS25|INSTRW,	TRIOP1-256,	8,	0xecL},
{"udivw3",	IS25|INSTRW,	TRIOP2-256,	8,	0xecL},
{"modw2",	IS25|INSTRW,	DYAOP1-256,	8,	0xa4L},
{"umodw2",	IS25|INSTRW,	DYAOP8-256,	8,	0xa4L},
{"modw3",	IS25|INSTRW,	TRIOP1-256,	8,	0xe4L},
{"umodw3",	IS25|INSTRW,	TRIOP2-256,	8,	0xe4L},
#if	FLOAT
{"fadds2",	INSTRW,		FPDYAOP1-256,	8,	0x31L},
{"fsubs2",	INSTRW,		FPDYAOP1-256,	8,	0x41L},
{"fmuls2",	INSTRW,		FPDYAOP1-256,	8,	0x51L},
{"fdivs2",	INSTRW,		FPDYAOP1-256,	8,	0x61L},
{"movhs",	INSTRHW,		FPDYAOP2-256,	8,	0x71L},
{"movws",	INSTRW,		FPDYAOP2-256,	8,	0x81L},
{"movss",	INSTRW,		FPDYAOP1-256,	8,	0x91L},
{"movsd",	INSTRW,		FPDYAOP1-256,	8,	0xa1L},
{"movsh",	INSTRW,		FPDYAOP1-256,	8,	0xb1L},
{"movtsh",	INSTRW,		FPDYAOP1-256,	8,	0xc1L},
{"movsw",	INSTRW,		FPDYAOP1-256,	8,	0xd1L},
{"movtsw",	INSTRW,		FPDYAOP1-256,	8,	0xe1L},
{"fcmps",	INSTRW,		FPCMPOP1-256,	8,	0xf1L},
{"faddd2",	INSTRD,		FPDYAOP1-256,	8,	0x35L},
{"fsubd2",	INSTRD,		FPDYAOP1-256,	8,	0x45L},
{"fmuld2",	INSTRD,		FPDYAOP1-256,	8,	0x55L},
{"fdivd2",	INSTRD,		FPDYAOP1-256,	8,	0x65L},
{"movhd",	INSTRH,		FPDYAOP2-256,	8,	0x75L},
{"movwd",	INSTRW,		FPDYAOP2-256,	8,	0x85L},
{"movds",	INSTRD,		FPDYAOP1-256,	8,	0x95L},
{"movdd",	INSTRD,		FPDYAOP1-256,	8,	0xa5L},
{"movdh",	INSTRD,		FPDYAOP1-256,	8,	0xb5},
{"movtdh",	INSTRD,		FPDYAOP1-256,	8,	0xc5},
{"movdw",	INSTRD,		FPDYAOP1-256,	8,	0xd5L},
{"movtdw",	INSTRD,		FPDYAOP1-256,	8,	0xe5L},
{"fcmpd",	INSTRD,		FPCMPOP1-256,	8,	0xf5L},
{"fadds3",	INSTRW,		FPTRIOP1-256,	8,	0x39L},
{"fsubs3",	INSTRW,		FPTRIOP1-256,	8,	0x49L},
{"fmuls3",	INSTRW,		FPTRIOP1-256,	8,	0x59L},
{"fdivs3",	INSTRW,		FPTRIOP1-256,	8,	0x69L},
{"faddd3",	INSTRD,		FPTRIOP1-256,	8,	0x79L},
{"fsubd3",	INSTRD,		FPTRIOP1-256,	8,	0x89L},
{"fmuld3",	INSTRD,		FPTRIOP1-256,	8,	0x99L},
{"fdivd3",	INSTRD,		FPTRIOP1-256,	8,	0xa9L},
#endif
{"alsw2",	IS25|INSTRBW,	DYAOP3-256,	16,	0xc0L},
{"alsw3",	IS25|INSTRBW,	SHFTOP1-256,	16,	0xc0L},
{"arsw2",	IS25|INSTRW,	DYAOP3-256,	16,	0xc4L},
{"arsw3",	IS25|INSTRW,	SHFTOP1-256,	16,	0xc4L},
{"cmpb",	IS25|INSTRB,	CMPOP1-256,	16,	0x2b3fL},
{"cmph",	IS25|INSTRH,	CMPOP1-256,	16,	0x2a3eL},
{"cmpw",	IS25|INSTRW,	CMPOP1-256,	16,	0x283cL},
{"jbc",		IS25|INSTRW,	JMBOP1-256,	8,	0x38L},
{"jbs",		IS25|INSTRW,	JMBOP1-256,	8,	0x38L},
{"rsb",		IS25|INSTRW,	ZEROP1-256,	8,	0x78L},
{"acjl",	IS25|INSTRW,	LOOPOP1-256,	24,	0xb83c4bL},
{"acjlu",	IS25|INSTRW,	LOOPOP1-256,	24,	0xb83c5bL},
{"acjle",	IS25|INSTRW,	LOOPOP1-256,	24,	0xb83c4fL},
{"acjleu",	IS25|INSTRW,	LOOPOP1-256,	24,	0xb83c5fL},
{"atjnzb",	IS25|INSTRW,	LOOPOP2-256,	8,	0x2bL},
{"atjnzh",	IS25|INSTRW,	LOOPOP2-256,	8,	0x2aL},
{"atjnzw",	IS25|INSTRW,	LOOPOP2-256,	8,	0x28L},
{"movblb",	IS25|INSTRB,	BLOCKOP-256,	8,	0x87L},
{"movblh",	IS25|INSTRH,	BLOCKOP-256,	8,	0x86L},
{"movblw",	IS25|INSTRW,	ZEROP1-256,	16,	0x3019L},
{"call",	IS25|INSTRW,	CALOP1-256,	8,	0x2cL},
{"save",	IS25|INSTRW,	SAVOP1-256,	8,	0x10L},
{"ret",		IS25|INSTRW,	RETOP1-256,	16,	0x0818L},
{"insv",	IS25|INSTRW,	FLDOP1-256,	8,	0xc8L},
{"extzv",	IS25|INSTRW,	FLDOP2-256,	8,	0xccL},
{"jz",		IS25|INSTRW,	JMPOP1-256,	16,	0x777fL},
{"jnz",		IS25|INSTRW,	JMPOP1-256,	16,	0x7f77L},
{"jpos",	IS25|INSTRW,	JMPOP1-256,	16,	0x4f47L},
{"jnpos",	IS25|INSTRW,	JMPOP1-256,	16,	0x474fL},
{"jneg",	IS25|INSTRW,	JMPOP1-256,	16,	0x434bL},
{"jnneg",	IS25|INSTRW,	JMPOP1-256,	16,	0x4b43L},
{"je",		IS25|INSTRW,	JMPOP1-256,	16,	0x777fL},
{"jne",		IS25|INSTRW,	JMPOP1-256,	16,	0x7f77L},
{"jl",		IS25|INSTRW,	JMPOP1-256,	16,	0x434bL},
{"jle",		IS25|INSTRW,	JMPOP1-256,	16,	0x474fL},
{"jg",		IS25|INSTRW,	JMPOP1-256,	16,	0x4f47L},
{"jge",		IS25|INSTRW,	JMPOP1-256,	16,	0x4b43L},
{"jlu",		IS25|INSTRW,	JMPOP1-256,	16,	0x535bL},
{"jleu",	IS25|INSTRW,	JMPOP1-256,	16,	0x575fL},
{"jgu",		IS25|INSTRW,	JMPOP1-256,	16,	0x5f57L},
{"jgeu",	IS25|INSTRW,	JMPOP1-256,	16,	0x5b53L},
{"jmp",		IS25|INSTRW,	JMPOP1-256,	8,	0x7bL},
{"jsb",		IS25|INSTRW,	JSBOP1-256,	16,	0x3437L},
{"BEB",		INSTRB,		JMPOP1-256,	8,	0x7fL},
{"BNEB",	INSTRB,		JMPOP1-256,	8,	0x77L},
{"BLB",		INSTRB,		JMPOP1-256,	8,	0x4bL},
{"BLEB",	INSTRB,		JMPOP1-256,	8,	0x4fL},
{"BGB",		INSTRB,		JMPOP1-256,	8,	0x47L},
{"BGEB",	INSTRB,		JMPOP1-256,	8,	0x43L},
{"BLUB",	INSTRB,		JMPOP1-256,	8,	0x5bL},
{"BLEUB",	INSTRB,		JMPOP1-256,	8,	0x5fL},
{"BGUB",	INSTRB,		JMPOP1-256,	8,	0x57L},
{"BGEUB",	INSTRB,		JMPOP1-256,	8,	0x53L},
{"BRB",		INSTRB,		JMPOP1-256,	8,	0x7bL},
{"BSBB",	INSTRB,		BSBOP1-256,	8,	0x37L},
{"BEH",		INSTRH,		JMPOP1-256,	8,	0x7eL},
{"BNEH",	INSTRH,		JMPOP1-256,	8,	0x76L},
{"BLH",		INSTRH,		JMPOP1-256,	8,	0x4aL},
{"BLEH",	INSTRH,		JMPOP1-256,	8,	0x4eL},
{"BGH",		INSTRH,		JMPOP1-256,	8,	0x46L},
{"BGEH",	INSTRH,		JMPOP1-256,	8,	0x42L},
{"BLUH",	INSTRH,		JMPOP1-256,	8,	0x5aL},
{"BLEUH",	INSTRH,		JMPOP1-256,	8,	0x5eL},
{"BGUH",	INSTRH,		JMPOP1-256,	8,	0x56L},
{"BGEUH",	INSTRH,		JMPOP1-256,	8,	0x52L},
{"BRH",		INSTRH,		JMPOP1-256,	8,	0x7aL},
{"BSBH",	INSTRH,		BSBOP1-256,	8,	0x36L},
{"CALL",	INSTRW,		CALOP2-256,	8,	0x2cL},
{"SAVE",	INSTRW,		SAVOP2-256,	8,	0x10L},
{"RESTORE",	INSTRW,		SAVOP2-256,	8,	0x18L},
{"RET",		INSTRW, 	ZEROP1-256,	8,	0x08L},
{"GATE",	INSTRW,		ZEROP1-256,	16,	0x3061L},
{"RETG",	INSTRW,		ZEROP1-256,	16,	0x3045L},
{"CALLPS",	INSTRW,		ZEROP1-256,	16,	0x30acL},
{"RETPS",	INSTRW,		ZEROP1-256,	16,	0x30c8L},
{"MVERNO",	INSTRW,		ZEROP1-256,	16,	0x3009L}, /* 12 */
{"DISVJMP",	INSTRW,		ZEROP1-256,	16,	0x3013L},
{"ENBVJMP",	INSTRW,		ZEROP1-256,	16,	0x300dL},
{"STREND",	INSTRW,		ZEROP1-256,	16,	0x301fL},
{"STRCPY",	INSTRW,		ZEROP1-256,	16,	0x3035L},
{"SLFTST",	INSTRW,		ZEROP1-256,	16,	0x303cL},
{"WAIT",	INSTRW,		ZEROP1-256,	8,	0x2fL},
{"BPT",		INSTRW, 	ZEROP1-256,	8,	0x2eL},
{"SWAPWI",	INSTRW,		MONOP1-256,	8,	0x1cL},
{"SWAPHI",	INSTRH,		MONOP1-256,	8,	0x1eL},
{"SWAPBI",	INSTRB,		MONOP1-256,	8,	0x1fL},
{"POPW",	INSTRW,		MONOP3-256,	8,	0x20L},
{"TSTW",	INSTRW,		MONOP1-256,	8,	0x28L},
{"TSTH",	INSTRH,		MONOP1-256,	8,	0x2aL},
{"TSTB",	INSTRB,		MONOP1-256,	8,	0x2bL},
{"RGEQ",	INSTRW,		ZEROP1-256,	8,	0x40L},
{"RGEQU",	INSTRW,		ZEROP1-256,	8,	0x50L},
{"RGTR",	INSTRW,		ZEROP1-256,	8,	0x44L},
{"RLSS",	INSTRW,		ZEROP1-256,	8,	0x48L},
{"RLSSU",	INSTRW,		ZEROP1-256,	8,	0x58L},
{"RLEQ",	INSTRW,		ZEROP1-256,	8,	0x4cL},
{"RCC",		INSTRW,		ZEROP1-256,	8,	0x50L},
{"RGTRU",	INSTRW,		ZEROP1-256,	8,	0x54L},
{"RCS",		INSTRW,		ZEROP1-256,	8,	0x58L},
{"RLEQU",	INSTRW,		ZEROP1-256,	8,	0x5cL},
{"RVC",		INSTRW,		ZEROP1-256,	8,	0x60L},
{"BVCH",	INSTRH,		JMPOP1-256,	8,	0x62L},
{"BVCB",	INSTRB,		JMPOP1-256,	8,	0x63L},
{"RVS",		INSTRW,		ZEROP1-256,	8,	0x68L},
{"BVSH",	INSTRH,		JMPOP1-256,	8,	0x6aL},
{"BVSB",	INSTRB,		JMPOP1-256,	8,	0x6bL},
{"BCCB",	INSTRB,		JMPOP1-256,	8,	0x53L},
{"BCSB",	INSTRB,		JMPOP1-256,	8,	0x5bL},
{"BCCH",	INSTRH,		JMPOP1-256,	8,	0x52L},
{"BCSH",	INSTRH,		JMPOP1-256,	8,	0x5aL},
{"BEQLBX",	INSTRB,		JMPOP1-256,	8,	0x6fL},
{"BEQLHX",	INSTRH,		JMPOP1-256,	8,	0x6eL},
{"BNEQBX",	INSTRB,		JMPOP1-256,	8,	0x67L},
{"BNEQHX",	INSTRH,		JMPOP1-256,	8,	0x66L},
{"JMP",		INSTRW,		JMPOP1-256,	8,	0x24L},
{"JSB",		INSTRW,		JSBOP2-256,	8,	0x34L},
{"RNEQ",	INSTRW,		ZEROP1-256,	8,	0x74L},
{"RNEQU",	INSTRW,		ZEROP1-256,	8,	0x64L},
{"NOP",		INSTRW,		ZEROP1-256,	8,	0x70L},
{"NOP3",	INSTRW,		ZEROP1-256,	24,	0x720000L},
{"NOP2",	INSTRW,		ZEROP1-256,	16,	0x7300L},
{"REQL",	INSTRW,		ZEROP1-256,	8,	0x7cL},
{"REQLU",	INSTRW,		ZEROP1-256,	8,	0x6cL},
{"CLRW",	INSTRW,		MONOP1-256,	8,	0x80L},
{"CLRH",	INSTRH,		MONOP1-256,	8,	0x82L},
{"CLRB",	INSTRB,		MONOP1-256,	8,	0x83L},
{"MNEGB",	INSTRB,		DYAOP1-256,	8,	0x8fL},
{"INCW",	INSTRW,		MONOP1-256,	8,	0x90L},
{"INCH",	INSTRH,		MONOP1-256,	8,	0x92L},
{"INCB",	INSTRB,		MONOP1-256,	8,	0x93L},
{"DECW",	INSTRW,		MONOP1-256,	8,	0x94L},
{"DECH",	INSTRH,		MONOP1-256,	8,	0x96L},
{"DECB",	INSTRB,		MONOP1-256,	8,	0x97L},
{"MODH2",	INSTRH,		DYAOP1-256,	8,	0xa6L},
{"MODB2",	INSTRB,		DYAOP1-256,	8,	0xa7L},
{"MULH2",	INSTRH,		DYAOP1-256,	8,	0xaaL},
{"MULB2",	INSTRB,		DYAOP1-256,	8,	0xabL},
{"DIVH2",	INSTRH,		DYAOP1-256,	8,	0xaeL},
{"DIVB2",	INSTRB,		DYAOP1-256,	8,	0xafL},
{"ALSW3",	INSTRW,		TRIOP1-256,	8,	0xc0L},
{"ARSW3",	INSTRW,		TRIOP1-256,	8,	0xc4L},
{"ARSH3",	INSTRH,		TRIOP1-256,	8,	0xc6L},
{"ARSB3",	INSTRB,		TRIOP1-256,	8,	0xc7L},
{"LLSW3",	INSTRW,		TRIOP1-256,	8,	0xd0L},
{"LLSH3",	INSTRH,		TRIOP1-256,	8,	0xd2L},
{"LLSB3",	INSTRB,		TRIOP1-256,	8,	0xd3L},
{"LRSW3",	INSTRW,		TRIOP1-256,	8,	0xd4L},
{"ROTW",	INSTRW,		TRIOP1-256,	8,	0xd8L},
{"MODH3",	INSTRH,		TRIOP1-256,	8,	0xe6L},
{"MODB3",	INSTRB,		TRIOP1-256,	8,	0xe7L},
{"MULH3",	INSTRH,		TRIOP1-256,	8,	0xeaL},
{"MULB3",	INSTRB,		TRIOP1-256,	8,	0xebL},
{"DIVH3",	INSTRH,		TRIOP1-256,	8,	0xeeL},
{"DIVB3",	INSTRB,		TRIOP1-256,	8,	0xefL},
{"MCOMB",	INSTRB,		DYAOP1-256,	8,	0x8bL},
{"MCOMH",	INSTRH,		DYAOP1-256,	8,	0x8aL},
{"MCOMW",	INSTRW,		DYAOP1-256,	8,	0x88L},
{"MOVTRW",	INSTRW,		DYAOP2-256,	8,	0xcL},
{"MOVAW",	INSTRW,		DYAOP2-256,	8,	0x4L},
{"PUSHAW",	INSTRW,		MONOP2-256,	8,	0xe0L},
{"ANDB2",	INSTRB,		DYAOP1-256,	8,	0xbbL},
{"ANDH2",	INSTRH,		DYAOP1-256,	8,	0xbaL},
{"ANDW2",	INSTRW,		DYAOP1-256,	8,	0xb8L},
{"ANDB3",	INSTRB,		TRIOP1-256,	8,	0xfbL},
{"ANDH3",	INSTRH,		TRIOP1-256,	8,	0xfaL},
{"ANDW3",	INSTRW,		TRIOP1-256,	8,	0xf8L},
{"ORB2",	INSTRB,		DYAOP1-256,	8,	0xb3L},
{"ORH2",	INSTRH,		DYAOP1-256,	8,	0xb2L},
{"ORW2",	INSTRW,		DYAOP1-256,	8,	0xb0L},
{"ORB3",	INSTRB,		TRIOP1-256,	8,	0xf3L},
{"ORH3",	INSTRH,		TRIOP1-256,	8,	0xf2L},
{"ORW3",	INSTRW,		TRIOP1-256,	8,	0xf0L},
{"XORB2",	INSTRB,		DYAOP1-256,	8,	0xb7L},
{"XORH2",	INSTRH,		DYAOP1-256,	8,	0xb6L},
{"XORW2",	INSTRW,		DYAOP1-256,	8,	0xb4L},
{"XORB3",	INSTRB,		TRIOP1-256,	8,	0xf7L},
{"XORH3",	INSTRH,		TRIOP1-256,	8,	0xf6L},
{"XORW3",	INSTRW,		TRIOP1-256,	8,	0xf4L},
{"BITB",	INSTRB,		CMPOP2-256,	8,	0x3bL},
{"BITH",	INSTRH,		CMPOP2-256,	8,	0x3aL},
{"BITW",	INSTRW,		CMPOP2-256,	8,	0x38L},
{"MOVB",	INSTRB,		DYAOP4-256,	8,	0x87L},
{"MOVH",	INSTRH,		DYAOP4-256,	8,	0x86L},
{"MOVW",	INSTRW,		DYAOP4-256,	8,	0x84L},
{"MNEGH",	INSTRH,		DYAOP1-256,	8,	0x8eL},
{"MNEGW",	INSTRW,		DYAOP1-256,	8,	0x8cL},
{"PUSHW",	INSTRW,		MONOP1-256,	8,	0xa0L},
{"ADDB2",	INSTRB,		DYAOP6-256,	8,	0x9fL},
{"ADDH2",	INSTRH,		DYAOP6-256,	8,	0x9eL},
{"ADDW2",	INSTRW,		DYAOP6-256,	8,	0x9cL},
{"ADDB3",	INSTRB,		TRIOP1-256,	8,	0xdfL},
{"ADDH3",	INSTRH,		TRIOP1-256,	8,	0xdeL},
{"ADDW3",	INSTRW,		TRIOP1-256,	8,	0xdcL},
{"SUBB2",	INSTRB,		DYAOP6-256,	8,	0xbfL},
{"SUBH2",	INSTRH,		DYAOP6-256,	8,	0xbeL},
{"SUBW2",	INSTRW,		DYAOP6-256,	8,	0xbcL},
{"SUBB3",	INSTRB,		TRIOP1-256,	8,	0xffL},
{"SUBH3",	INSTRH,		TRIOP1-256,	8,	0xfeL},
{"SUBW3",	INSTRW,		TRIOP1-256,	8,	0xfcL},
{"MULW2",	INSTRW,		DYAOP1-256,	8,	0xa8L},
{"MULW3",	INSTRW,		TRIOP1-256,	8,	0xe8L},
{"DIVW2",	INSTRW,		DYAOP1-256,	8,	0xacL},
{"DIVW3",	INSTRW,		TRIOP1-256,	8,	0xecL},
{"MODW2",	INSTRW,		DYAOP1-256,	8,	0xa4L},
{"MODW3",	INSTRW,		TRIOP1-256,	8,	0xe4L},
{"CMPB",	INSTRB,		CMPOP1-256,	8,	0x3fL},
{"CMPH",	INSTRH,		CMPOP1-256,	8,	0x3eL},
{"CMPW",	INSTRW,		CMPOP1-256,	8,	0x3cL},
{"RSB",		INSTRW,		ZEROP1-256,	8,	0x78L},
{"MOVBLW",	INSTRW,		ZEROP1-256,	16,	0x3019L},
{"INSFW",	INSTRW,		FLDOP4-256,	8,	0xc8L},
{"INSFH",	INSTRH,		FLDOP4-256,	8,	0xcaL},
{"INSFB",	INSTRB,		FLDOP4-256,	8,	0xcbL},
{"EXTFW",	INSTRW,		FLDOP4-256,	8,	0xccL},
{"EXTFH",	INSTRH,		FLDOP4-256,	8,	0xceL},
{"EXTFB",	INSTRB,		FLDOP4-256,	8,	0xcfL},
{"EXTOP",	INSTRX,		SIMOP-256,	8,	0x14L},
{"getsm",	INSTRX,		ZEROP1-256,	8,	0x1L},
{"putsm",	INSTRX,		ZEROP1-256,	8,	0x11L},
{"ungetsm",	INSTRX,		ZEROP1-256,	8,	0x21L},
{"r0",		DUMMY,		REGISTER-256,	4,	0x0L},
{"r1",		DUMMY,		REGISTER-256,	4,	0x1L},
{"r2",		DUMMY,		REGISTER-256,	4,	0x2L},
{"r3",		DUMMY,		REGISTER-256,	4,	0x3L},
{"r4",		DUMMY,		REGISTER-256,	4,	0x4L},
{"r5",		DUMMY,		REGISTER-256,	4,	0x5L},
{"r6",		DUMMY,		REGISTER-256,	4,	0x6L},
{"r7",		DUMMY,		REGISTER-256,	4,	0x7L},
{"r8",		DUMMY,		REGISTER-256,	4,	0x8L},
{"fp",		DUMMY,		REGISTER-256,	4,	0x9L},
{"ap",		DUMMY,		REGISTER-256,	4,	0xaL},
{"psw",		DUMMY,		REGISTER-256,	4,	0xbL},
{"sp",		DUMMY,		REGISTER-256,	4,	0xcL},
{"pcbp",	DUMMY,		REGISTER-256,	4,	0xdL},
{"isp",		DUMMY,		REGISTER-256,	4,	0xeL},
{".globl",	PSEUDO,		PSGLOBAL-256,	0,	0x0L},
{".comm",	PSEUDO, 	PSCOMM-256,	0,	0x0L},
{".set",	PSEUDO,		PSSET-256,	0,	0x0L},
{".zero",	PSEUDO,		PSZERO-256,	0,	0x0L},
{".bss",	BSS,		PSBSS-256,	0,	0x0L},
{".file",	PSEUDO,		PSFILE-256,	0,	0x0L},
{".align",	PSEUDO,		PSALIGN-256,	0,	0x0L},
{".byte",	PSEUDO,		PSBYTE-256,	0,	0x0L},
{".half",	PSEUDO,		PSHALF-256,	0,	0x0L},
{".word",	PSEUDO,		PSWORD-256,	0,	0x0L},
#if	FLOAT
{".float",	PSEUDO,		PSFLOAT-256,	0,	0x0L},
{".double",	PSEUDO,		PSDOUBLE-256,	0,	0x0L},
#endif
{".def",	PSEUDO,		PSDEF-256,	0,	0x0L},
{".val",	PSEUDO,		PSVAL-256,	0,	0x0L},
{".scl",	PSEUDO,		PSSCL-256,	0,	0x0L},
{".type",	PSEUDO,		PSTYPE-256,	0,	0x0L},
{".tag",	PSEUDO,		PSTAG-256,	0,	0x0L},
{".line",	PSEUDO,		PSLINE-256,	0,	0x0L},
{".size",	PSEUDO,		PSSIZE-256,	0,	0x0L},
{".dim",	PSEUDO,		PSDIM-256,	0,	0x0L},
{".endef",	PSEUDO,		PSENDEF-256,	0,	0x0L},
{".ln",		PSEUDO,		PSLN-256,	0,	0x0L},
{".text",	TXT,		SECTIONN-256,	0,	0x0L},
{".data",	DAT,		SECTIONN-256,	0,	0x0L},
{"sbyte",	DUMMY,		EXPTYPE-256,	8,	0x7L},
{"ubyte",	DUMMY,		EXPTYPE-256,	8,	0x3L},
{"byte",	DUMMY,		EXPTYPE-256,	8,	0x7L},
{"shalf",	DUMMY,		EXPTYPE-256,	8,	0x6L},
{"half",	DUMMY,		EXPTYPE-256,	8,	0x6L},
{"uhalf",	DUMMY,		EXPTYPE-256,	8,	0x2L},
{"sword",	DUMMY,		EXPTYPE-256,	8,	0x4L},
{"word",	DUMMY,		EXPTYPE-256,	8,	0x4L},
{"uword",	DUMMY,		EXPTYPE-256,	8,	0x0L},

