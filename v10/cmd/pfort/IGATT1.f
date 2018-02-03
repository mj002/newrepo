      INTEGER FUNCTION IGATT1(INDEX, FIELD)
C
      COMMON /CTABL/ LDSA, PDSA, DSA(5000)
C
      INTEGER INDEX, FIELD
      INTEGER LDSA, PDSA, DSA
      INTEGER FWTH(8), FPOS(8)
C
      DATA FWTH(1) /16/, FPOS(1) /1/
      DATA FWTH(2) /2/, FPOS(2) /16/
      DATA FWTH(3) /2/, FPOS(3) /32/
      DATA FWTH(4) /2/, FPOS(4) /64/
      DATA FWTH(5) /2/, FPOS(5) /128/
      DATA FWTH(6) /2/, FPOS(6) /256/
      DATA FWTH(7) /4/, FPOS(7) /512/
      DATA FWTH(8) /32/, FPOS(8) /2048/
C
      IGATT1 = MOD(DSA(INDEX)/FPOS(FIELD),FWTH(FIELD))
C
      RETURN
C
      END
