.text
.globl _main
_main:.word 0x0
subl2 $48,sp
ret
.globl _f
_f:.word 0x0
subl2 $48,sp
moval _f,r1
calls $0,(r1)
movl r0,r1
moval _f,r2
movl r1,-44(fp)
calls $0,(r2)
movl r0,r2
movl -44(fp),r1
moval 4(ap),r3
addl3 r2,r1,r1
movl r1,(r3)
ret
.globl _f2
_f2:.word 0x0
subl2 $52,sp
moval _f,r1
calls $0,(r1)
movl r0,r1
moval 4(ap),r2
movl (r2),r2
movl $0,r3
cmpl r2,r3; jeql L2
moval _f,r2
movl r1,-48(fp)
calls $0,(r2)
movl r0,r2
movl -48(fp),r1
moval -4(fp),r3
movl r2,(r3)
moval L3,r2
jmp (r2)
L2:moval -4(fp),r2
movl $1,r3
movl r3,(r2)
L3:moval 4(ap),r2
moval -4(fp),r3
movl (r3),r3
addl3 r3,r1,r1
movl r1,(r2)
ret
.globl _f3
_f3:.word 0x0
subl2 $92,sp
moval -4(fp),r1
movl $0,r2
movl r2,(r1)
moval -8(fp),r1
movl $0,r2
movl r2,(r1)
moval -12(fp),r1
movl $0,r2
movl r2,(r1)
moval -16(fp),r1
movl $0,r2
movl r2,(r1)
moval -20(fp),r1
movl $0,r2
movl r2,(r1)
moval -24(fp),r1
movl $0,r2
movl r2,(r1)
moval -28(fp),r1
movl $0,r2
movl r2,(r1)
moval -32(fp),r1
movl $0,r2
movl r2,(r1)
moval -36(fp),r1
movl $0,r2
movl r2,(r1)
moval -40(fp),r1
movl $0,r2
movl r2,(r1)
moval 8(ap),r1
movl (r1),r2
movl $4,r3
addl3 r3,r2,r3
movl r3,(r1)
moval -44(fp),r1
moval 4(ap),r3
movl (r3),r3
movl r3,(r1)
movl $0,r1
cmpl r3,r1; jeql L5
moval _f,r1
movl r2,-84(fp)
calls $0,(r1)
movl r0,r1
movl -84(fp),r2
moval -44(fp),r3
movl r1,(r3)
L5:moval -44(fp),r1
movl (r1),r1
movl r1,(r2)
ret
.globl _f4
_f4:.word 0x7c0
subl2 $76,sp
moval -4(fp),r1
movl $0,r2
movl r2,(r1)
moval -8(fp),r1
movl $0,r2
movl r2,(r1)
moval -12(fp),r1
movl $0,r2
movl r2,(r1)
moval -16(fp),r1
movl $0,r2
movl r2,(r1)
moval -20(fp),r1
movl $0,r2
movl r2,(r1)
moval -24(fp),r1
movl $0,r2
movl r2,(r1)
moval _i,r1
movl (r1),r1
movl $3,r2
ashl r2,r1,r2
moval _a,r3
addl3 r3,r2,r3
movd (r3),r3
moval _b,r5
addl3 r5,r2,r2
movd (r2),r5
addd3 r5,r3,r7
moval _10,r2
movd (r2),r9
cmpd r7,r9; jeql L8
movl $0,r2
cmpl r1,r2; jeql L8
subd3 r5,r3,r1
cmpd r1,r9; jeql L8
moval -28(fp),r1
movl $1,r2
movl r2,(r1)
moval L9,r1
jmp (r1)
L8:moval -28(fp),r1
movl $0,r2
movl r2,(r1)
L9:moval _i,r1
moval -28(fp),r2
movl (r2),r2
movl r2,(r1)
ret
.globl _f5
_f5:.word 0xfc0
subl2 $56,sp
moval _x,r1
moval _k,r2
movl (r2),r2
moval _m,r3
movl (r3),r3
mull3 r3,r2,r4
movl $3,r5
ashl r5,r4,r4
moval _A,r6
movl (r6),r6
addl3 r6,r4,r4
movd (r4),r7
moval _j,r9
movl (r9),r9
mull3 r3,r9,r3
ashl r5,r3,r3
addl3 r6,r3,r3
moval -8(fp),r0
movl r3,(r0)
movd (r3),r10
muld3 r10,r7,r6
moval _n,r8
movl (r8),r8
mull3 r8,r2,r2
ashl r5,r2,r2
moval _B,r10
movl (r10),r10
addl3 r10,r2,r2
moval -4(fp),r0
movl r2,(r0)
movd (r2),r2
mull3 r8,r9,r8
ashl r5,r8,r5
addl3 r10,r5,r5
movd (r5),r8
muld3 r8,r2,r2
addd3 r2,r6,r2
movd r2,(r1)
movd (r4),r2
movd (r5),r4
muld3 r4,r2,r2
moval -4(fp),r4
movl (r4),r4
movd (r4),r4
moval -8(fp),r6
movl (r6),r6
movd (r6),r6
muld3 r6,r4,r4
subd3 r4,r2,r2
movd r2,(r1)
ret
.data
.globl _x
.align 2; _x:.space 8
.globl _B
.align 2; _B:.space 4
.globl _A
.align 2; _A:.space 4
.globl _n
.align 2; _n:.space 4
.globl _m
.align 2; _m:.space 4
.globl _k
.align 2; _k:.space 4
.globl _j
.align 2; _j:.space 4
.globl _i
.align 2; _i:.space 4
.globl _b
.align 2; _b:.space 80
.globl _a
.align 2; _a:.space 80
.text 1
.align 2; _10:.long 0x0,0x0

