.set reorder
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=0
.frame $sp,0,$31
L.1:
j $31
.end main
.globl memchar
.text
.align 2; .ent memchar
memchar:
$fp2=8
.frame $sp,8,$31
addu $sp,$sp,-8
lw $25,$fp2-8($sp)
addu $24,$25,1
sw $24,$fp2-8($sp)
lb $25,($25)
sb $25,$fp2-1($sp)
lw $25,$fp2-8($sp)
addu $25,$25,1
sw $25,$fp2-8($sp)
lb $25,($25)
sb $25,$fp2-1($sp)
lw $25,$fp2-8($sp)
addu $24,$25,-1
sw $24,$fp2-8($sp)
lb $25,($25)
sb $25,$fp2-1($sp)
lw $25,$fp2-8($sp)
addu $25,$25,-1
sw $25,$fp2-8($sp)
lb $25,($25)
sb $25,$fp2-1($sp)
L.2:
addu $sp,$sp,8
j $31
.end memchar
.globl memint
.text
.align 2; .ent memint
memint:
$fp3=8
.frame $sp,8,$31
addu $sp,$sp,-8
lw $25,$fp3-8($sp)
addu $24,$25,4
sw $24,$fp3-8($sp)
lw $25,($25)
sw $25,$fp3-4($sp)
lw $25,$fp3-8($sp)
addu $25,$25,4
sw $25,$fp3-8($sp)
lw $25,($25)
sw $25,$fp3-4($sp)
lw $25,$fp3-8($sp)
addu $24,$25,-4
sw $24,$fp3-8($sp)
lw $25,($25)
sw $25,$fp3-4($sp)
lw $25,$fp3-8($sp)
addu $25,$25,-4
sw $25,$fp3-8($sp)
lw $25,($25)
sw $25,$fp3-4($sp)
L.3:
addu $sp,$sp,8
j $31
.end memint
.globl regchar
.text
.align 2; .ent regchar
regchar:
$fp4=0
.frame $sp,0,$31
move $25,$24
addu $24,$25,1
lb $15,($25)
addu $25,$24,1
move $24,$25
lb $15,($25)
move $25,$24
addu $24,$25,-1
lb $15,($25)
addu $25,$24,-1
move $24,$25
lb $15,($25)
L.4:
j $31
.end regchar
.globl regint
.text
.align 2; .ent regint
regint:
$fp5=0
.frame $sp,0,$31
move $25,$24
addu $24,$25,4
lw $15,($25)
addu $25,$24,4
move $24,$25
lw $15,($25)
move $25,$24
addu $24,$25,-4
lw $15,($25)
addu $25,$24,-4
move $24,$25
lw $15,($25)
L.5:
j $31
.end regint
