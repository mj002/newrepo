.set reorder
.globl words
.data; .align 2; words:.word 0x1
.word 0x2
.word 0x3
.ascii "if\000"
.space 3
.space 2
.word 0x4
.word 0x5
.space 4
.ascii "for"
.space 3
.space 2
.word 0x6
.word 0x7
.word 0x8
.ascii "else\000"
.space 1
.space 2
.word 0x9
.word 0xa
.word 0xb
.ascii "whil"
.byte 101
.space 1
.space 2
.word 0x0
.space 8
.space 8
.globl wordlist
.sdata; .align 2; wordlist:.word words
.globl x
.data; .align 2; x:.word 0x1
.word 0x2
.word 0x3
.word 0x4
.word 0x0
.word 0x5
.word 0x6
.space 12
.word 0x7
.space 16
.globl y
.data; .align 2; y:.word x
.word x+20
.word x+40
.word 0x0
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $23,$0
b L.8
L.5:
move $30,$0
b L.12
L.9:
la $4,L.13
sll $25,$30,2
sll $24,$23,2
lw $24,y($24)
addu $25,$25,$24
lw $5,($25)
jal printf  # int
L.10:
addu $30,$30,1
L.12:
sll $25,$30,2
sll $24,$23,2
lw $24,y($24)
addu $25,$25,$24
lw $25,($25)
bne $25,$0,L.9
la $4,L.14
jal printf  # int
L.6:
addu $23,$23,1
L.8:
sll $25,$23,2
lw $25,y($25)
bne $25,$0,L.5
jal f  # int
lw $4,wordlist
jal g  # int
move $2,$0
L.4:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end main
.data; .align 2; L.16:.word L.17
.word L.18
.word L.19
.word L.20
.word 0x0
.text
.globl f
.text
.align 2; .ent f
f:
$fp2=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
la $30,L.16
b L.24
L.21:
la $4,L.25
lw $5,($30)
jal printf  # int
L.22:
addu $30,$30,4
L.24:
lw $25,($30)
bne $25,$0,L.21
L.15:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end f
.globl g
.text
.align 2; .ent g
g:
$fp3=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $30,$4
b L.30
L.27:
move $23,$0
b L.34
L.31:
la $4,L.35
sll $25,$23,2
addu $25,$25,$30
lw $5,($25)
jal printf  # int
L.32:
addu $23,$23,1
L.34:
bltu $23,3,L.31
la $4,L.25
addu $5,$30,12
jal printf  # int
L.28:
addu $30,$30,20
L.30:
lw $25,($30)
bne $25,$0,L.27
jal h  # int
L.26:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end g
.globl h
.text
.align 2; .ent h
h:
$fp4=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0000000,-8
sw $30,20($sp)
sw $31,24($sp)
move $30,$0
b L.40
L.37:
la $4,L.41
li $25,20
mul $25,$25,$30
lw $5,words($25)
lw $6,words+4($25)
lw $7,words+8($25)
la $25,words+12($25)
sw $25,16($sp)
jal printf  # int
L.38:
addu $30,$30,1
L.40:
bltu $30,5,L.37
L.36:
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end h
.rdata
L.41:.ascii "%d %d %d %s\012\000"
L.35:.ascii "%d \000"
L.25:.byte 37,115,10,0
L.20:.ascii "while\000"
L.19:.ascii "else\000"
L.18:.ascii "for\000"
L.17:.ascii "if\000"
L.14:.byte 10,0
L.13:.ascii " %d\000"
