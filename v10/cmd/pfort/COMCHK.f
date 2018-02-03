      SUBROUTINE COMCHK(MAIN)
      INTEGER TEMP(1), PLAT, STAR, PCOM, COM, PNODE, SYMLEN
      INTEGER ZERO(1)
      COMMON /COMS/ LCOM, PCOM, COM(300)
      COMMON /GRAPH/ LLAT, PLAT, LAT(6000)
      COMMON /PARAMS/ I1, I2, I3, SYMLEN, I4, I5, I6
      COMMON /HEAD/ LNODE, PNODE, NODE(500)
      COMMON /SCR2/ LNN, NN(500)
      DATA STAR /1H*/
      DATA ZERO(1) /0/
C
C     ALGORITHM TO CHECK FOR LEGAL USE OF COMMON IN PGM UNITS
C     NODE(MAIN) POINTS TO SUPEROOT ENTRY IN LAT
C
      IF (PCOM.LE.0) GO TO 130
      LK = 1
   10 IF (LK.GE.PCOM-1) GO TO 130
C
C     CHECK COMMON ISNT BLANK COMMON
C
      CALL S5UNPK(COM(LK), TEMP, 1)
      IF (TEMP(1).EQ.STAR) GO TO 120
C
C     CHECK THAT COMMON BLOCK NOT IN BLOCK DATA PGM
C     NEED NOT CHECK THE COMMON
C
      K = LK + SYMLEN + 1
      IF (COM(K).EQ.1) GO TO 120
C
C     NEED ALGORITHM TO CHECK OUT THIS COMMON
C
      L = PNODE - 1
      DO 20 K=1,L
        NN(K) = 0
        IF (NODE(K).LT.0) NN(K) = 1
   20 CONTINUE
      ICNT = 0
      NN(MAIN) = 2
C
C     SEARCH FOR A 2 NODE
C
   30 L = PNODE - 1
      DO 40 K=1,L
        IF (NN(K).EQ.2) GO TO 50
   40 CONTINUE
      GO TO 120
C
C     FOUND A 2 NODE; CHANGE TO 1 TO SHOW HAVE VISITED IT;
C     IF SUBPGM CONTAINS COMMON IN QUESTION INCREMENT COUNT;
C     IF COUNT> 1 ERROR IN USAGE
C     IF SUBPGM DOESN'T CONTAIN COMMON, MARK HIS DESC 2 IF THEY ARE 0.
C
   50 NN(K) = 1
      LBR = NODE(K)
      L = NODE(K) + SYMLEN + 2
      L = LAT(L)
   60 IF (L.EQ.0) GO TO 90
      IF (LAT(L).NE.LK) GO TO 80
C
C     FOUND COMMON LK AT THIS NODE
C     MARK NODE TO A 3
C
      NN(K) = 3
      ICNT = ICNT + 1
      IF (ICNT.LE.1) GO TO 30
      CALL ERROR2(31H ILLEGAL USAGE OF COMMON BLOCK , 31, COM(LK),
     *  1, 1, 0)
      K = PNODE - 1
      DO 70 I=1,K
        L = NODE(I)
        IF (NN(I).EQ.3) CALL ERROR2(19H WHICH APPEARED IN , 19, LAT(L),
     *  1, 0, 0)
   70 CONTINUE
      CALL ERROR2( 1H1, 0, ZERO(1), -3, 0, 1)
      GO TO 120
   80 L = LAT(L+2)
      GO TO 60
C
C     ARE DONE SEARCHING FOR COMMON LK AT THIS NODE
C     ADD DESCENDENTS ONTO LIST TO BE VISITED
C
   90 L = NODE(K) + SYMLEN + 4
      L = LAT(L)
  100 IF (L.EQ.0) GO TO 30
      K = PNODE - 1
C
C     FIND DESC OF NODE AND IF NOT VISITED SET TO 2
C
      DO 110 I=1,K
        IF (NODE(I).NE.LAT(L)) GO TO 110
        IF (NN(I).EQ.0) NN(I) = 2
  110 CONTINUE
      L = LAT(L+1)
      GO TO 100
  120 LK = LK + SYMLEN + 5
      GO TO 10
  130 RETURN
      END