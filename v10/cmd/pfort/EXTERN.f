      SUBROUTINE EXTERN
      LOGICAL ERR, SYSERR, ABORT
      LOGICAL BR, INTEXT
      INTEGER STMT, PSTMT
      COMMON /FACTS/ NAME, NOST, ITYP, IASF
      COMMON /INPUT/ NSTMT, PSTMT, STMT(1327)
      COMMON /DETECT/ ERR, SYSERR, ABORT
C
C     PROCESS AN EXTERNAL STMT. CAUSES IDS TO BE MARKED AS EXTERNAL
C     PROCEDURES. ERROR OCCURS IF ID HAS PREVIOUSLY DEFINED INCONSISTANT
C     USAGE. EXTERNAL CAN'T APPEAR IN BLOCK DATA PGM UNIT.
C
      L = IGATT1(NAME,8)
      IF (L.EQ.11) GO TO 60
   10 CALL NEXTOK(PSTMT, K2, KK)
      IF (KK.EQ.0) GO TO 20
      CALL ERROR1(19H ILLEGAL IDENTIFIER, 19)
      GO TO 70
   20 KK = LOOKUP(K2,.FALSE.)
      IF (SYSERR) RETURN
      L = IGATT1(KK,8)
      IF (L.EQ.0) GO TO 30
      CALL ERROR1(26H ILLEGAL USE OF IDENTIFIER, 26)
      GO TO 40
   30 CALL SATT1(KK, 8, 13)
C
C     CAUSE EXTERNAL BIT TO BE SET IF POSSIBLE BASIC EXTERNAL
C
   40 BR = INTEXT(KK,0,0,.FALSE.)
      IF (K2.EQ.NSTMT) GO TO 70
      IF (STMT(K2).NE.68) GO TO 50
      PSTMT = K2 + 1
      GO TO 10
   50 CALL ERROR1(20H ILLEGAL PUNCTUATION, 20)
      GO TO 70
   60 CALL ERROR1(
     *60H ILLEGAL USAGE OF EXTERNAL STMT WITHIN BLOCK DATA SUBPROGRAM,
     *    60)
   70 RETURN
      END