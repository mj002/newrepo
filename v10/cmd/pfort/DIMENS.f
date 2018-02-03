      SUBROUTINE DIMENS
C
C     PROCESSES DIMENSION STMT, RECORDING OF ARRAY BOUNDS, LENGTH
C     ETC. DONE IN ORDER
C
      INTEGER STMT, PSTMT
      LOGICAL ERR, SYSERR, ABORT, ARDECL
      COMMON /DETECT/ ERR, SYSERR, ABORT
      COMMON /INPUT/ NSTMT, PSTMT, STMT(1327)
   10 IF (ARDECL(K2,K3)) GO TO 20
      CALL ERROR1(35H ILLEGAL SYNTAX OF ARRAY DECLARATOR, 35)
      GO TO 40
   20 IF (SYSERR) RETURN
      IF (K2.EQ.NSTMT) GO TO 40
      IF (STMT(K2).EQ.68 .AND. K2+1.NE.NSTMT) GO TO 30
      CALL ERROR1(20H ILLEGAL PUNCTUATION, 20)
      GO TO 40
   30 PSTMT = K2 + 1
      GO TO 10
   40 RETURN
      END
