      DOUBLE PRECISION FUNCTION WDOTUI(N,XR,XI,INCX,YR,YI,INCY)
      DOUBLE PRECISION XR(1),XI(1),YR(1),YI(1),S,FLOP
      S = 0.0D0
      IF (N .LE. 0) GO TO 20
      IX = 1
      IY = 1
      IF (INCX.LT.0) IX = (-N+1)*INCX + 1
      IF (INCY.LT.0) IY = (-N+1)*INCY + 1
      DO 10 I = 1, N
         S = S + XR(IX)*YI(IY) + XI(IX)*YR(IY)
         IF (S .NE. 0.0D0) S = FLOP(S)
         IX = IX + INCX
         IY = IY + INCY
   10 CONTINUE
   20 WDOTUI = S
      RETURN
      END
