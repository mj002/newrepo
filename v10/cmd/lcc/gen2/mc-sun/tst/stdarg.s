.data
.globl _x
.align 2; _x:.long 1
.long 2
.long 3
.long 4
.text
.globl _main
.align 2
_main:link a6,#-116
fmovemx #0x10,a6@(-12)
moveml #0x2000,a6@(-16)
movl #L2,sp@
jbsr _print:l
movl #L3,sp@
movl #L4,sp@(4)
jbsr _print:l
movl #L5,sp@
movl #3,sp@(4)
movl #10,sp@(8)
jbsr _print:l
movl #L6,sp@
movl #L7,sp@(4)
movl #L8,sp@(8)
movl #4,sp@(12)
movl #10,sp@(16)
jbsr _print:l
movl #L9,sp@
movl #L7,sp@(4)
movl #L8,sp@(8)
fmoved L10:l,fp3
fmoved fp3,sp@(12)
movl #10,sp@(20)
jbsr _print:l
movl #L11,sp@
lea _x:l,a0
movl a0,a5
lea sp@(4),a1; movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
movl a5,a0
lea sp@(20),a1; movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
movl a5,a0
lea sp@(36),a1; movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
movl a5,a0
lea sp@(52),a1; movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
movl a5,a0
lea sp@(68),a1; movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
movl a5,a0
lea sp@(84),a1; movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
jbsr _print:l
L1:
moveml a6@(-16),#0x2000
fmovemx a6@(-12),#0x10
unlk a6
rts
.globl _print
.align 2
_print:link a6,#-64
fmovemx #0x10,a6@(-28)
moveml #0x2490,a6@(-44)
lea a6@(12),a5
jra L19
L16:
movl a6@(8),a2
cmpb #37,a2@; jne L20
movl a6@(8),a2
lea a2@(1),a2
movl a2,a6@(8)
movb a2@,d4
extbl d4
movl d4,d7
cmpl #115,d7; jeq L36
cmpl #115,d7; jgt L40
cmpl #98,d7; jlt L22
cmpl #102,d7; jgt L22
movl @(L42-392:l,d7:l:4),a2
jra a2@
L42:
.align 2; .long L25
.align 2; .long L30
.align 2; .long L32
.align 2; .long L22
.align 2; .long L38
L41:
L40:
cmpl #119,d7; jeq L34
jra L22
L25:
lea a5@(16),a2
movl a2,a5
lea a2@(-16),a0
lea a6@(-16),a1
movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
movl #L26,sp@
movl a6@(-16),sp@(4)
movl a6@(-12),sp@(8)
movl a6@(-8),sp@(12)
movl a6@(-4),sp@(16)
jbsr _printf:l
jra L21
L30:
movl #L31,sp@
lea a5@(4),a2
movl a2,a5
movb a2@(-1),d4
extbl d4
movl d4,sp@(4)
jbsr _printf:l
jra L21
L32:
movl #L33,sp@
lea a5@(4),a2
movl a2,a5
movl a2@(-4),sp@(4)
jbsr _printf:l
jra L21
L34:
movl #L35,sp@
lea a5@(4),a2
movl a2,a5
movw a2@(-2),d4
extl d4
movl d4,sp@(4)
jbsr _printf:l
jra L21
L36:
movl #L37,sp@
lea a5@(4),a2
movl a2,a5
movl a2@(-4),sp@(4)
jbsr _printf:l
jra L21
L38:
movl #L39,sp@
lea a5@(8),a2
movl a2,a5
fmoved a2@(-8),fp3
fmoved fp3,sp@(4)
jbsr _printf:l
jra L21
L22:
movl #L31,sp@
movl a6@(8),a2
movb a2@,d4
extbl d4
movl d4,sp@(4)
jbsr _printf:l
jra L21
L20:
movl #L31,sp@
movl a6@(8),a2
movb a2@,d4
extbl d4
movl d4,sp@(4)
jbsr _printf:l
L21:
L17:
movl a6@(8),a2
lea a2@(1),a2
movl a2,a6@(8)
L19:
movl a6@(8),a2
tstb a2@; jne L16
L13:
moveml a6@(-44),#0x2490
fmovemx a6@(-28),#0x10
unlk a6
rts
.text
.align 2; L39:.byte 37,102,0
.align 2; L37:.byte 37,115,0
.align 2; L35:.byte 37,120,0
.align 2; L33:.byte 37,100,0
.align 2; L31:.byte 37,99,0
.align 2; L26:.ascii "{%d %d %d %d}\000"
.align 2; L11:.ascii "%b %b %b %b %b %b\012\000"
.align 2; L10:.long 0x40140000,0x0
.align 2; L9:.ascii "%s%s %f%c\000"
.align 2; L8:.ascii "st\000"
.align 2; L7:.ascii "te\000"
.align 2; L6:.ascii "%s%s %w%c\000"
.align 2; L5:.ascii "test %d%c\000"
.align 2; L4:.byte 50,0
.align 2; L3:.ascii "test %s\012\000"
.align 2; L2:.ascii "test 1\012\000"
