      DOUBLE PRECISION FUNCTION DCEIL(X)
C
C  DCEIL RETURNS CEIL(X)
C
      DOUBLE PRECISION X
C
      DCEIL = DBLE( FLOAT ( IDINT(X) ) )
      IF (X .LE. 0.0D0) RETURN
      IF (DCEIL .NE. X) DCEIL = DCEIL + 1.0D0
C
      RETURN
      END
