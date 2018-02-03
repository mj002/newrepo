      SUBROUTINE COMAND(ID)
      INTEGER ID(4)
      DOUBLE PRECISION STKR(50005),STKI(50005)
      INTEGER IDSTK(4,48),LSTK(48),MSTK(48),NSTK(48),VSIZE,LSIZE,BOT,TOP
      INTEGER ALFA(52),ALFB(52),ALFL,CASE
      INTEGER IDS(4,32),PSTK(32),RSTK(32),PSIZE,PT,PTZ
      INTEGER DDT,ERR,FMT,LCT(4),LIN(1024),LPT(6),RIO,WIO,RTE,WTE,HIO
      INTEGER SYM,SYN(4),BUF(256),CHAR,FLP(2),FIN,FUN,LHS,RHS,RAN(2)
      COMMON /VSTK/ STKR,STKI,IDSTK,LSTK,MSTK,NSTK,VSIZE,LSIZE,BOT,TOP
      COMMON /ALFS/ ALFA,ALFB,ALFL,CASE
      COMMON /RECU/ IDS,PSTK,RSTK,PSIZE,PT,PTZ
      COMMON /IOP/ DDT,ERR,FMT,LCT,LIN,LPT,RIO,WIO,RTE,WTE,HIO
      COMMON /COM/ SYM,SYN,BUF,CHAR,FLP,FIN,FUN,LHS,RHS,RAN
      INTEGER CMD(4,17),CMDL,A,D,E,Z,CH,BLANK,NAME,DOT,H(4)
      INTEGER SEMI,COMMA,EOL
      DOUBLE PRECISION URAND
      LOGICAL EQID
      DATA CMDL/17/,A/10/,D/13/,E/14/,Z/35/,EOL/99/,SEMI/39/,COMMA/48/
      DATA BLANK/36/,NAME/1/,DOT/47/
C
C       CLEAR ELSE  END   EXIT
C       FOR   HELP  IF    LONG
C       RETUR SEMI
C       SHORT WHAT  WHILE
C       WHO   WHY   LALA  FOO
      DATA CMD/
     $  12,21,14,10, 14,21,28,14, 14,23,13,36, 14,33,18,29,
     $  15,24,27,36, 17,14,21,25, 18,15,36,36, 21,24,23,16,
     $  27,14,29,30, 28,14,22,18,
     $  28,17,24,27, 32,17,10,29, 32,17,18,21,
     $  32,17,24,36, 32,17,34,36, 21,10,21,10, 15,30,12,20/
C
      IF (DDT .EQ. 1) WRITE(WTE,100)
  100 FORMAT(0X,'COMAND')
      FUN = 0
      DO 10 K = 1, CMDL
        IF (EQID(ID,CMD(1,K))) GO TO 20
   10 CONTINUE
      FIN = 0
      RETURN
C
   20 IF (CHAR.EQ.COMMA .OR. CHAR.EQ.SEMI .OR. CHAR.EQ.EOL) GO TO 22
      IF (CHAR.LE.Z .OR. K.EQ.6) GO TO 22
      CALL ERROR(16)
      RETURN
C
   22 FIN = 1
      GO TO (25,36,38,40,30,80,34,52,44,55,50,65,32,60,70,46,48),K
C
C     CLEAR
   25 IF (CHAR.GE.A .AND. CHAR.LE.Z) GO TO 26
      BOT = LSIZE-3
      GO TO 98
   26 CALL GETSYM
      TOP = TOP+1
      MSTK(TOP) = 0
      NSTK(TOP) = 0
      RHS = 0
      CALL STACKP(SYN)
      IF (ERR .GT. 0) RETURN
      FIN = 1
      GO TO 98
C
C     FOR, WHILE, IF, ELSE, END
   30 FIN = -11
      GO TO 99
   32 FIN = -12
      GO TO 99
   34 FIN = -13
      GO TO 99
   36 FIN = -14
      GO TO 99
   38 FIN = -15
      GO TO 99
C
C     EXIT
   40 IF (PT .GT. PTZ) FIN = -16
      IF (PT .GT. PTZ) GO TO 98
      K = IDINT(STKR(VSIZE-2))
      WRITE(WTE,140) K
      IF (WIO .NE. 0) WRITE(WIO,140) K
  140 FORMAT(/0X,'total flops ',I9//0X,'ADIOS'/)
      FUN = 99
      GO TO 98
C
C     RETURN
   44 K = LPT(1) - 7
      IF (K .LE. 0) FUN = 99
      IF (K .LE. 0) GO TO 98
      CALL FILES(-RIO,BUF,BUF)
      LPT(1) = LIN(K+1)
      LPT(4) = LIN(K+2)
      LPT(6) = LIN(K+3)
      PTZ = LIN(K+4)
      RIO = LIN(K+5)
      LCT(4) = LIN(K+6)
      CHAR = BLANK
      SYM = COMMA
      GO TO 99
C
C     LALA
   46 WRITE(WTE,146)
  146 FORMAT(0X,'QUIT SINGING AND GET BACK TO WORK.')
      GO TO 98
C
C     FOO
   48 WRITE(WTE,148)
  148 FORMAT(0X,'YOUR PLACE OR MINE')
      GO TO 98
C
C     SHORT, LONG
   50 FMT = 1
      GO TO 54
   52 FMT = 2
   54 IF (CHAR.EQ.E .OR. CHAR.EQ.D) FMT = FMT+2
      IF (CHAR .EQ. Z) FMT = 5
      IF (CHAR.EQ.E .OR. CHAR.EQ.D .OR. CHAR.EQ.Z) CALL GETSYM
      GO TO 98
C
C     SEMI
   55 LCT(3) = 1 - LCT(3)
      GO TO 98
C
C     WHO
   60 WRITE(WTE,160)
      IF (WIO .NE. 0) WRITE(WIO,160)
  160 FORMAT(0X,'Your current variables are...')
      CALL PRNTID(IDSTK(1,BOT),LSIZE-BOT+1)
      L = VSIZE-LSTK(BOT)+1
      WRITE(WTE,161) L,VSIZE
      IF (WIO .NE. 0) WRITE(WIO,161) L,VSIZE
  161 FORMAT(0X,'using ',I7,' out of ',I7,' elements.')
      GO TO 98
C
C     WHAT
   65 WRITE(WTE,165)
  165 FORMAT(0X,'The functions and commands are...')
      H(1) = 0
      CALL FUNS(H)
      CALL PRNTID(CMD,CMDL-2)
      GO TO 98
C
C     WHY
   70 K = IDINT(9.0D0*URAND(RAN(1))+1.0D0)
      GO TO (71,72,73,74,75,76,77,78,79),K
   71 WRITE(WTE,171)
  171 FORMAT(0X,'WHAT?')
      GO TO 98
   72 WRITE(WTE,172)
  172 FORMAT(0X,'R.T.F.M.')
      GO TO 98
   73 WRITE(WTE,173)
  173 FORMAT(0X,'HOW THE HELL SHOULD I KNOW?')
      GO TO 98
   74 WRITE(WTE,174)
  174 FORMAT(0X,'PETE MADE ME DO IT.')
      GO TO 98
   75 WRITE(WTE,175)
  175 FORMAT(0X,'INSUFFICIENT DATA TO ANSWER.')
      GO TO 98
   76 WRITE(WTE,176)
  176 FORMAT(0X,'IT FEELS GOOD.')
      GO TO 98
   77 WRITE(WTE,177)
  177 FORMAT(0X,'WHY NOT?')
      GO TO 98
   78 WRITE(WTE,178)
  178 FORMAT(0X,'/--ERROR'/0X,'STUPID QUESTION.')
      GO TO 98
   79 WRITE(WTE,179)
  179 FORMAT(0X,'SYSTEM ERROR, RETRY')
      GO TO 98
C
C     HELP
   80 DO 81 I = 1, 4
         H(I) = ALFA(BLANK+1)
   81 CONTINUE
      IF (CHAR .EQ. EOL) GO TO 84
      CALL GETSYM
      IF (SYM .EQ. NAME) GO TO 82
      IF (SYM .EQ. 0) SYM = DOT
      H(1) = ALFA(SYM+1)
      GO TO 84
   82 DO 83 I = 1, 4
        CH = SYN(I)
        H(I) = ALFA(CH+1)
   83 CONTINUE
   84 CALL HELPER(H)
      GO TO 98
C
   98 CALL GETSYM
   99 RETURN
      END
