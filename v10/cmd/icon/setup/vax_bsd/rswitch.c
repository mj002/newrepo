/*
 * This is the co-expression context switch for the VAX-11 operating
 * under Berkeley 4.3bsd. It should work for 4.1 and 4.2,
 */

/*
 * coswitch
 */

/* >coswitch */
coswitch(old_cs, new_cs, first)
int *old_cs, *new_cs;
int first;
   {
   asm("  movl 4(ap),r0");
   asm("  movl 8(ap),r1");
   asm("  movl sp,0(r0)");
   asm("  movl fp,4(r0)");
   asm("  movl ap,8(r0)");
   asm("  movl r11,16(r0)");
   asm("  movl r10,20(r0)");
   asm("  movl r9,24(r0)");
   asm("  movl r8,28(r0)");
   asm("  movl r7,32(r0)");
   asm("  movl r6,36(r0)");
   if (first == 0) {		/* this is first activation */
      asm("  movl 0(r1),sp");
      asm("  clrl fp");
      asm("  clrl ap");
      interp(0, 0);
      syserr("interp() returned in coswitch");
      }
   else {
      asm(" movl 0(r1),sp");
      asm(" movl 4(r1),fp");
      asm(" movl 8(r1),ap");
      asm(" movl 16(r1),r11");
      asm(" movl 20(r1),r10");
      asm(" movl 24(r1),r9");
      asm(" movl 28(r1),r8");
      asm(" movl 32(r1),r7");
      asm(" movl 36(r1),r6");
      }
   }
/* <coswitch */
