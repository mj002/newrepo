      SUBROUTINE GETVAL(S)
      DOUBLE PRECISION S
C     FORM NUMERICAL VALUE
      INTEGER SYM,SYN(4),BUF(256),CHAR,FLP(2),FIN,FUN,LHS,RHS,RAN(2)
      COMMON /COM/ SYM,SYN,BUF,CHAR,FLP,FIN,FUN,LHS,RHS,RAN
      S = 0.0D0
   10 IF (CHAR .GT. 9) RETURN
      S = 10.0D0*S + CHAR
      CALL GETCH
      GO TO 10
      END
