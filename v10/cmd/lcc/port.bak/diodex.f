      SUBROUTINE DIODEX
      COMMON /D10DER/ STATS
      INTEGER STATS(9)
      INTEGER I1MACH
      INTEGER TEMP
C TO PRINT THE RUN-TIME STATISTICS FOR IODE.
      CALL D1ODEX(STATS, 0)
      TEMP = I1MACH(2)
      WRITE (TEMP,  1) STATS
   1  FORMAT (33H DIODE(J,F,TS,SS,E,NIT,ND,NF,R) =, 9(I5))
      RETURN
      END
