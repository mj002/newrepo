      SUBROUTINE D1ODEH(T0, V0, T1, V1, NV, DT, TSTOP, EV, OK,
     1   HANDLE)
      INTEGER NV
      EXTERNAL HANDLE
      REAL EV(NV)
      LOGICAL OK
      DOUBLE PRECISION T0, V0(NV), T1, V1(NV), DT, TSTOP
      COMMON /D1ODEF/ FNUM
      INTEGER FNUM
      COMMON /D10DER/ NJS, NFS, NTSS, NSSS, NES, NNITS, NNDS, NNFS, NRS
      INTEGER NJS, NFS, NTSS, NSSS, NES, NNITS
      INTEGER NNDS, NNFS, NRS
      COMMON /D1ODEM/ THETA, EGIVE, MINIT, MAXIT, KEEJAC, IRCS
      INTEGER MINIT, MAXIT, KEEJAC, IRCS
      REAL EGIVE
      DOUBLE PRECISION THETA
      COMMON /D1ODEG/ TJ, DTJ, GETJAC, SEPATE, USENGJ, USENNS, USENFD
      LOGICAL GETJAC, SEPATE, USENGJ, USENNS, USENFD
      DOUBLE PRECISION TJ, DTJ
      COMMON /D10DET/ TGOOD
      DOUBLE PRECISION TGOOD
C OUTPUT FILTER FOR IODES.
C SCRATCH SPACE ALLOCATED - S(D1ODEH) = S(HANDLE).
      IF (T0 .EQ. T1) GOTO 1
         FNUM = 0
         TGOOD = T1
         GOTO  2
   1     NRS = NRS+1
   2  IF (T0 .NE. T1 .OR. KEEJAC .NE. 3) GOTO 3
         GETJAC = T0 .NE. TJ
         TJ = T0
   3  NTSS = NTSS+1
      CALL HANDLE(T0, V0, T1, V1, NV, DT, TSTOP)
      RETURN
      END