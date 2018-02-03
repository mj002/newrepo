      LOGICAL FUNCTION D1ODEN(V, NV, T, DT, D, DA, ERROR, INMI,
     1   ERRPAR)
      INTEGER NV
      EXTERNAL D, DA, ERROR, INMI
      REAL ERRPAR(2)
      DOUBLE PRECISION V(NV), T, DT
      COMMON /CSTAK/ DS
      DOUBLE PRECISION DS(500)
      COMMON /D1ODEM/ THETA, EGIVE, MINIT, MAXIT, KEEJAC, IRCS
      INTEGER MINIT, MAXIT, KEEJAC, IRCS
      REAL EGIVE
      DOUBLE PRECISION THETA
      INTEGER IDV, IEV, IVT, ISTKGT, IEV1, IEV2
      INTEGER IVTETA, IS(1000), IVOLD
      REAL RS(1000)
      LOGICAL DONE, LS(1000), D1ODEO
      DOUBLE PRECISION WS(500)
      EQUIVALENCE (DS(1), WS(1), RS(1), IS(1), LS(1))
C NONLINEAR EQUATION SOLVER FOR IODES.
C SCRATCH SPACE ALLOCATED -
C     S(D1ODEN) =  4*NV +
C                 3*NV REAL +
C                 MAX ( S(DA), S(ERROR) )
C LONG REAL WORDS.
C THE PORT LIBRARY STACK AND ITS ALIASES.
      CALL ENTER(1)
      IVOLD = ISTKGT(4*NV, 4)
      IVTETA = IVOLD+NV
      IVT = IVTETA+NV
      IDV = IVT+NV
      IEV = ISTKGT(3*NV, 3)
      IEV1 = IEV+NV
      IEV2 = IEV1+NV
      DONE = D1ODEO(V, NV, T, DT, D, DA, ERROR, INMI, ERRPAR, WS(IVTETA)
     1   , WS(IVT), WS(IVOLD), RS(IEV), RS(IEV1), RS(IEV2), WS(IDV),
     2   THETA, MINIT, MAXIT, KEEJAC, IRCS, EGIVE)
      CALL LEAVE
      D1ODEN = DONE
      RETURN
      END
