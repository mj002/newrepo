#
#	sdb	Makefile	4.2	7/8/81
#
# Conditional compilation flags:
#	STD - Distribution systems UNIX/32V, VM/UNIX 2.n.  won't work.
#	VMUNIX - virtual memory system - keep namelist in VM.
#	FLEXNAMES - New a.out format with string table (VMUNIX Version 4)
#
# The files old.c and old.h contain routines and definitions peculiar
# to old versions of the system.
#
CFLAGS=	-DVMUNIX -DFLEXNAMES -g

SRCS=	access.c decode.c display.c docomm.c fio.c main.c message.c \
	old.c opset.c optab.c pcs.c prvar.c re.c runpcs.c \
	setup.c sub.c symt.c udef.c version.c xeq.c
OBJS=	access.o decode.o display.o docomm.o fio.o main.o message.o \
	old.o opset.o optab.o pcs.o prvar.o re.o runpcs.o \
	setup.o sub.o symt.o udef.o version.o xeq.o
HDRS=	cdefs.h defs.h head.h mac.h machine.h mode.h old.h

sdb: ${OBJS}
	cc -g -o sdb ${OBJS}

install: sdb
	cp sdb sdb.temp
	strip sdb
	cp sdb $(DESTDIR)/usr/bin
	mv sdb.temp sdb

clean:
	rm -f *.o sdb

lint:
	lint ${SRCS}

print:
	ls -l Makefile | pr
	pr -f *.h *.c

depend:
	/bin/grep '^#[ 	]*include' ${SRCS} \
		| sed '/<.*>/d' \
		| sed 's/:[^"]*"\([^"]*\)".*/: \1/' \
		| sed 's/\.c/.o/' >>makedep
	echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
	echo '$$r makedep' >>eddep
	echo 'w' >>eddep
	cp Makefile Makefile.bak
	ed - Makefile < eddep
	rm eddep makedep
	echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
	echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
	echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE -- make depend uses it
