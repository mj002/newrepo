      SUBROUTINE ODES(F,X,NX,TSTART,TSTOP,DT,ERRPAR,HANDLE)
C
C  TO SOLVE THE INITIAL VALUE PROBLEM FOR
C
C          DX(T)/DT = F(T,X(T)).
C
C  METHOD - RATIONAL EXTRAPOLATION OF GRAGGS MODIFIED MID-POINT RULE.
C
C  INPUT
C
C    F      - CALL F(T,X,NX,FTX) SHOULD RETURN FTX(I)=F(T,X)(I),
C             FOR I=1,...,NX. IF IT CANNOT, IT SHOULD RETURN
C             OKAY=.FALSE. IN COMMON /ODESF/OKAY .
C             F SHOULD BE DECLARED EXTERNAL IN THE SUBPROGRAM
C             CALLING ODES.
C    X      - THE INITIAL VALUES FOR THE SOLUTION.
C    NX     - THE LENGTH OF THE SOLUTION VECTOR X.
C    TSTART - THE INITIAL TIME.
C    TSTOP  - THE FINAL TIME.
C    DT     - THE INITIAL TIME-STEP TO BE USED.
C             THE PERFORMANCE OF ODES IS SUBSTANTIALLY
C             INDEPENDENT OF THE VALUE OF DT CHOSEN BY THE USER.
C             IT IS SUFFICIENT THAT THE USERS CHOICE FOR DT MERELY BE
C             WITHIN SEVERAL ORDERS OF MAGNITUDE OF BEING CORRECT.
C             THE VALUE OF DT WILL BE AUTOMATICALLY CHANGED BY ODES
C             DURING THE INTEGRATION PROCESS IN SUCH A WAY AS TO GET
C             THE SOLUTION, TO THE DESIRED ACCURACY, AT THE LEAST
C             POSSIBLE COST.
C    ERRPAR - EACH COMPONENT X(I) OF THE SOLUTION IS TO BE COMPUTED
C             TO WITHIN AN ABSOLUTE ERROR OF
C
C                     ERRPAR(1) * ABS(X(I)) + ERRPAR(2)
C
C             FOR I=1,...,NX, AT EACH TIME-STEP. THIS ERROR REQUEST MUST
C             ALWAYS BE POSITIVE.
C    HANDLE - OUTPUT ROUTINE WITH A CALLING SEQUENCE OF THE FORM
C
C                     HANDLE(T0,X0,T1,X1,NX,DT,TSTOP,E)
C
C             HANDLE WILL BE CALLED AT THE END OF EACH TIME-STEP.
C
C             THE INPUT TO HANDLE IS AS FOLLOWS
C
C               X0,X1  - X0=X(T0) AND X1=X(T1).
C               T0,T1  - T0=T1 INDICATES A RESTART AND X1 IS FULL OF
C                        GARBAGE.
C               NX     - THE LENGTH OF THE SOLUTION VECTOR X.
C               DT     - THE PROPOSED TIME-STEP FOR THE NEXT STEP.
C               TSTOP  - THE CURRENT FINAL TIME.
C               E      - E(I) GIVES THE REAL ABSOLUTE ERROR IN X1(I),
C                        I=1,...,NX, FOR THE SINGLE CURRENT TIME-STEP.
C
C             THE OUTPUT FROM HANDLE MAY BE ANY OF
C
C               X1     - MAY BE ALTERED IF DESIRED.
C               DT     - THE PROPOSED TIME-STEP FOR THE NEXT STEP.
C               TSTOP  - THE FINAL TIME VALUE.
C
C             HANDLE SHOULD BE DECLARED EXTERNAL IN THE
C             SUBPROGRAM CALLING ODES.
C
C  OUTPUT
C
C    X      - X=X(TSTOP), THE FINAL VALUE FOR THE SOLUTION.
C    TSTOP  - MAY BE ALTERED BY USER SUPPLIED ROUTINE HANDLE.
C    DT     - PROPOSED TIME-STEP FOR THE NEXT STEP, IF ANY.
C
C  SCRATCH SPACE OF LENGTH
C
C                   S(ODES) .LE.
C
C    32 + 12*NX
C
C  REAL WORDS +
C
C    101 + MAX( 2*NX REAL + S(F) ,
C
C               11*NX + 10 REAL + 10 ,
C
C               NX + S(HANDLE) )
C
C  INTEGER WORDS IS ALLOCATED.
C
C  ERROR STATES
C
C    1 - NX.LT.1.
C    2 - DT HAS THE WRONG SIGN ON INPUT.
C    3 - DT=0 ON INPUT.
C    4 - DT RETURNED BY HANDLE HAS THE WRONG SIGN.
C    5 - DT=0 WAS RETURNED BY HANDLE. (RECOVERABLE)
C    6 - THE ERROR DESIRED IN X(I) IS NOT POSITIVE. (RECOVERABLE)
C    7 - DT=0. (RECOVERABLE)
C    8 - CANNOT RAISE DT IN HANDLE WHEN .NOT.OKAY.
C
      COMMON /ODESF/OKAY
C
      REAL X(NX),TSTART,TSTOP,DT
      REAL ERRPAR(2)
      LOGICAL OKAY
      EXTERNAL F,HANDLE
C
      EXTERNAL ODESE
C
      IF (TSTART.EQ.TSTOP) GO TO 10
C
      CALL ODES1(F,X,NX,TSTART,TSTOP,DT,ODESE,ERRPAR,HANDLE,
     1            .FALSE.,.FALSE.)
C
 10   RETURN
C
      END
