      INTEGER FUNCTION CHK1(IR, IE)
C
C      PROGRAM UNIT AT LAT(IR) CALLS PROGRAM UNIT AT LAT(IE)
C      CHK1 RETURNS 1 IF REF OK ELSE 0
C      CHECKS FOR INCONSISTENT SYBPGM USAGE, CORRECT NO. OF
C      ARGS, CORRECT USAGE MATCH UP OF ARGS
C
      INTEGER PREF, REF, PDSA, DSA, PLAT, SYMLEN, IBR(1)
      COMMON /CREF/ LREF, PREF, REF(100)
      COMMON /CTABL/ LDSA, PDSA, DSA(5000)
      COMMON /GRAPH/ LLAT, PLAT, LAT(6000)
      COMMON /PARAMS/ I1, I2, I3, SYMLEN, I4, I5, I6
      CHK1 = 0
C      CHECKS FOR SELF RECURSION
      IF (IE.NE.IR) GO TO 10
      CALL ERROR2(24H RECURSIVE CALL OF SELF , 24, REF(3), -1, 1, 1)
      GO TO 100
C      CHECKS FOR USAGE OF SUBPGM CONSISTENT WITH DEF
   10 I = IE + SYMLEN + 6
      N = MOD(LAT(I),8)
      IF (N.EQ.REF(4) .OR. N.EQ.6 .AND. REF(4).EQ.1) GO TO 20
      CALL ERROR2(38H INCONSISTENT REFERENCE TO SUBPROGRAM , 38,
     *  LAT(IE), 1, 1, 0)
      CALL ERROR2(1H1, 0, REF(3), -1, 0, 1)
      GO TO 100
C      CHECKS FOR CORRECT NO. ARGS
   20 I = IE + SYMLEN
      IF (LAT(I).EQ.REF(1)/2) GO TO 30
      CALL ERROR2(42H INCORRECT NUMBER OF ARGS IN REFERENCE TO , 42,
     *  LAT(IE), 1, 1, 0)
      CALL ERROR2(1H1, 0, REF(3), -1, 0, 1)
      GO TO 100
C      ARE DONE WITH REF IF IT HAS NO ARGS
 30   CHK1 = 1
      IF(LAT(I)) 100, 100, 40
C      CHECK USAGES OF ARGS IN CALL VS USAGES OF ARGS IN DEF
C      N POINTS TO DUMMY ARG ENTRY
   40 I = LAT(I)
      N = IE + SYMLEN + 1
      L = 5
      DO 80 K=1,I
        MD = IGATT2(LAT(N),8)
        IF (MD.NE.13 .AND. MD.NE.5 .AND. MD.NE.6) GO TO 60
C      HAVE A PROC ARG IN DEF
C      NEED A PROC ARG IN REF
        IF (REF(L).EQ.0) GO TO 50
        MR = IGATT1(REF(L),8)
        IF (MR.EQ.13 .OR. MR.EQ.6 .OR. MR.EQ.5) GO TO 70
 50   IBR(1) = K
      CALL ERROR2(36H INCOMPATIBLE USAGE ASSOCIATED WITH ,36,IBR,-2,1,0)
      CALL ERROR2(17H IN REFERENCE TO ,17,LAT(IE), 1, 0, 0)
      CALL ERROR2(1H1, 0, REF(3), -1, 0, 1)
      CHK1 = 0
      GOTO 70
C      HAVE VARIABLE OR ARRAY AS ARG IN DEF
C      NEED SAME IN REF
   60   IF (REF(L).EQ.0) GO TO 70
        MR = IGATT1(REF(L),8)
        IF (MR.EQ.13 .OR. MR.EQ.5 .OR. MR.EQ.6) GO TO 50
   70   N = LAT(N) + 3
        L = L + 2
   80 CONTINUE
  100 RETURN
      END