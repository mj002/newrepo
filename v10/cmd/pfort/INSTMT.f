      SUBROUTINE INSTMT(EOF, NCARD)
      LOGICAL ILLEG, ILHOL, ILCONT
      LOGICAL NEWRD, EOF, CONT, OPT, P1ERR
      INTEGER PSTMT, CARD(80), OUTUT, BLK, STATE, HCOUNT, STMT
      INTEGER SYMLEN, OUTUT2, OUTUT3, OUTUT4
      DIMENSION IBR(5)
      COMMON /INPUT/ NSTMT, PSTMT, STMT(1327)
      COMMON /OPTNS/ OPT(5), P1ERR
      COMMON /PARAMS/ INUT, OUTUT, NOCHAR, SYMLEN, OUTUT2, OUTUT3,
     *    OUTUT4
      COMMON /FACTS/ NAME, NOST, ITYP, IASF
      DATA NEWRD /.TRUE./
      DATA IBR(1), IBR(2), IBR(3), IBR(4), IBR(5) /1H*,1HC,1H.,1H0,1H /
      DATA CARD(1) /1H /
C
C     ROUTINE INPUTS AN ENTIRE FORTRAN STATEMENT. IT IS PASSED 1 CHAR
C     PER ELEMENT IN THE ARRAY STMT.
C     IN SHOULD BE CALLED AGAIN IF ERR IS TRUE.
C     EACH CARD IS WRITTEN OUT BEFORE BEING PARSED.
C     IN DEBLANKS CARDS USES THE VARIABLE STATE TO FIND
C     HOLLERITHS:
C     STATE = 0.....LOOK FOR BREAK CHAR
C     1.....HAVE BREAK CHAR AND NEED DIGIT
C     2.....HAVE DIGIT AND NEED DIGIT OR "H"
C     3....NEED TO SKIP OVER THESE CHARS, PART OF HOLLERITH
C     NOTE: USE "90" AS AN END OF STMT MARKER
C
C     NEWRD IS TRUE IF A NEW CARD IS NECESSARY; ELSE CARD IN BUFFER
C     IS USED.  EOF IS TRUE WHEN END-OF-FILE CARD ("." IN COL 1)
C     IS READ     NCARD GIVES # OF CARDS READ FOR THIS STMT
C
   10 NOST = NOST + 1
      STATE = 0
      NSTMT = 0
      ILLEG = .FALSE.
      ILCONT = .FALSE.
      ILHOL = .FALSE.
      NCARD = 0
      CONT = .FALSE.
   20 IF (NEWRD) CALL IN(CARD, INUT)
      IF (CARD(1).NE.IBR(2) .OR. CARD(2).NE.IBR(1)) GO TO 40
C
C     DEAL WITH OPTIONS CARD
C
      IF (CONT) GO TO 50
      CALL INOPT(CARD)
   30 IF (OPT(4)) WRITE (OUTUT,99999) CARD
99999 FORMAT (11X, 80A1)
      NEWRD = .TRUE.
      GO TO 20
   40 IF (CARD(1).NE.IBR(2)) GO TO 70
C
C     DEAL WITH COMMENT CARD
C
      IF (.NOT.CONT) GO TO 30
C
C     HAVE COMPLETED STMT
C
   50 NEWRD = .FALSE.
      NSTMT = NSTMT + 1
      STMT(NSTMT) = 90
   60 IF (ILLEG) CALL ERROR1(
     *    40H WARNING - NON-FORTRAN CHARACTER IGNORED, 40)
      IF (ILHOL) CALL ERROR1(
     *    46H WARNING - NON-FORTRAN CHARACTER IN HOLLERITH , 46)
      IF (ILCONT) CALL ERROR1(
     *    45H WARNING - NON-FORTRAN CONTINUATION CHARACTER, 45)
      RETURN
   70 IF (CARD(1).NE.IBR(3)) GO TO 80
C
C     HAVE EOF
C
      IF (CONT) GO TO 50
      EOF = .TRUE.
      GO TO 60
   80 IF (CARD(6).EQ.IBR(4) .OR. CARD(6).EQ.IBR(5)) GO TO 100
C
C     DEAL WITH CONTINUATION CARD
C
      II = MAPCHR(CARD(6),ILCONT)
      IF (CONT) GO TO 110
      CALL ERROR1(40H WARNING - CONTINUATION NOT ALLOWED HERE, 40)
C
C     FLUSH TO NEXT NONCONTINUATION CARD
C
   90 IF (OPT(4)) WRITE (OUTUT,99998) NOST, CARD
99998 FORMAT (1H , I5, 5X, 80A1)
      CALL IN(CARD, INUT)
      IF (CARD(1).NE.IBR(1) .AND. CARD(1).NE.IBR(2) .AND.
     *    CARD(1).NE.IBR(3) .AND. CARD(6).NE.IBR(4) .AND.
     *    CARD(6).NE.IBR(5)) GO TO 90
      NEWRD = .FALSE.
      GO TO 10
C
C     DEAL WITH A NON-CONTINUATION CARD
C
  100 IF (CONT) GO TO 50
C     DEAL WITH A LEGAL CONTIN OR NONCONTIN CARD
  110 NCARD = NCARD + 1
      IF (NCARD.LT.21) GO TO 120
      CALL ERROR1(33H WARNING - TOO MANY CONTINUATIONS, 33)
      GO TO 90
  120 IF (OPT(4)) WRITE (OUTUT,99998) NOST, CARD
      NEWRD = .TRUE.
      BLK = 0
      IF (NSTMT.NE.0) GO TO 150
      DO 130 I=1,5
        NSTMT = NSTMT + 1
        STMT(NSTMT) = MAPCHR(CARD(I),ILLEG)
  130 CONTINUE
  140 I = 7
      GO TO 180
  150 DO 160 I=1,5
        IF (MAPCHR(CARD(I),ILLEG).NE.69) GO TO 170
  160 CONTINUE
      GO TO 140
  170 CALL ERROR1(40H WARNING - ILLEGAL LABEL WILL BE IGNORED, 40)
      GO TO 140
  180 IF (I.LE.72) GO TO 200
C
C     AFTER TRANSLATE CARD CHECK FOR BLANK CARD
C     AND GO BACK FOR A CONTINUATION CARD
C
      IF (BLK.NE.66) GO TO 190
      CALL ERROR1(33H WARNING - BLANK CARD ENCOUNTERED, 33)
      GO TO 90
  190 CONT = .TRUE.
      GO TO 20
  200 IF (STATE.NE.3) GO TO 210
C
C     STATE 3 --ARE PROCESSING A HOLLERITH
C
      HCOUNT = HCOUNT - 1
      KK = MAPCHR(CARD(I),ILHOL)
      IF (HCOUNT.EQ.0) STATE = 0
      GO TO 290
  210 NSTMT = NSTMT + 1
      STMT(NSTMT) = MAPCHR(CARD(I),ILLEG)
      IF (STMT(NSTMT).NE.69) GO TO 220
C
C     BLANK ENCOUNTERED AND DELETED
C
      BLK = BLK + 1
      NSTMT = NSTMT - 1
      GO TO 290
  220 KK = STATE + 1
      GO TO (280, 260, 230), KK
C
C     STATE 2--SKIP OVER LEADING DIGIT STRING; LOOK FOR H
C
  230 IF (STMT(NSTMT).LE.9) GO TO 290
      IF (STMT(NSTMT).NE.37) GO TO 270
C
C     PROCESS HOLLERITH COUNT
C
      STMT(NSTMT) = -STMT(NSTMT)
      STATE = 3
      KK = NSTMT - KST
      I10 = 1
      HCOUNT = 0
      DO 240 K=1,KK
        JJ = NSTMT - K
        HCOUNT = HCOUNT + I10*STMT(JJ)
        I10 = I10*10
  240 CONTINUE
      STMT(KST) = HCOUNT - 2048
      NSTMT = KST
      IF (HCOUNT.LE.0) GO TO 250
      GO TO 290
C
C     AVOID THE 0H CONSTRUCTION
C
  250 CALL ERROR1(44H WARNING - 0H ILLEGAL HOLLERITH CONSTRUCTION, 44)
      STATE = 0
      GO TO 290
C
C     STATE 1--LOOK FOR START OF DIGIT STRING BEFORE H
C
  260 IF (STMT(NSTMT).GT.9) GO TO 270
      STATE = 2
      KST = NSTMT
      GO TO 290
C
C     CHECK FOR NESTED SPECIAL HEADING CHARS
C
  270 STATE = 0
  280 IF ((STMT(NSTMT).LT.65) .OR. (STMT(NSTMT).GT.68)) GO TO 290
      STATE = 1
  290 I = I + 1
      GO TO 180
      END
