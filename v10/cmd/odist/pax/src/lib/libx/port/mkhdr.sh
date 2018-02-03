:
#
# mkhdr hdr [ genhdr ] [ cc [ ccflags ] ]
#
# @(#)mkhdr (ulysses!gsf) 01/31/91
#
case $# in
0)	echo "Usage: $0 hdr [ genhdr ] [ cc [ ccflags ] ]" >&2; exit 1 ;;
esac
hdr=$1
shift
case $1 in
"")	genhdr= ;;
*)	genhdr=$1 ;;
esac
case $# in
0)	;;
*)	shift ;;
esac
cc=${1-cc}
case $# in
0)	;;
*)	shift ;;
esac
tmp=x.$$
trap 'code=$?; rm -f $tmp.?; exit $code' 0 1 2 3
case $hdr in
align)	case $genhdr in
	"")	echo "$0: $hdr: genhdr arg required" >&2; exit 1 ;;
	esac
	cat <<!
/*
 * AT&T Bell Laboratories
 * alignment and pointer pun definitions
 * : : generated by $0 and $genhdr : :
 */

#ifndef __ALIGN_H__
#define __ALIGN_H__

!
	$genhdr $hdr || exit 1
	cat <<!
#endif
!
	;;
dirent)	case $genhdr in
	"")	echo "$0: $hdr: genhdr arg required" >&2; exit 1 ;;
	esac
	cat<<!
/*
 * AT&T Bell Laboratories
 * directory stream access definitions
 *
 *	#include <sys/types.h>
 *	#include <dirent.h>
 *
 * : : generated by $0 and $genhdr : :
 */

#ifndef __DIRENT_H__

#include <lcldirent.h>

#ifndef __DIRENT_H__
#define __DIRENT_H__
#endif

!
	$genhdr $hdr || exit 1
	for i in "" "#ifndef d_ino
#define d_ino	d_fileno
#endif" "#ifndef d_fileno
#define d_fileno	d_ino
#endif"
	do	cat > $tmp.c <<!
#include <sys/types.h>
#include <lcldirent.h>
#ifdef _lcl_dirent
static struct dirent dir;
#else
#include <sys/dir.h>
static struct direct dir;
#endif
$i
int n1 = sizeof(dir.d_ino);
int n2 = sizeof(dir.d_fileno);
!
		if	$cc -I. ${1+"$@"} -c $tmp.c >/dev/null 2>&1
		then	break
		fi
	done
	nl=
	case $i in
	?*)	nl=1
		echo "$i"
		;;
	esac
	cat > $tmp.c <<!
#include <sys/types.h>
#include <lcldirent.h>
#ifdef _lcl_dirent
static struct dirent dir;
#else
#include <sys/dir.h>
static struct direct dir;
#endif
int n1 = sizeof(dir.d_namlen);
!
	if	$cc -I. ${1+"$@"} -c $tmp.c >/dev/null 2>&1
	then	nl=1
		echo "#define _dir_namlen	1	/* dirent.d_namlen */"
	fi
	case $nl in
	?*)	echo ;;
	esac
	cat <<!
#ifndef _lcl_dirent

#ifdef _dir_sys

#include <sys/dir.h>

#else

#undef	_dir_rewinddir

#define DIRSIZ	14

struct direct
{
	short	d_ino;
	char	d_name[DIRSIZ];
};

#endif

#ifdef _dir_rewinddir

/*
 * variable length directory entry format
 */

#define _dir_variable

#ifndef dirent
#define dirent	direct
#endif

#else

/*
 * fixed length directory entry format
 */

#define _dir_fixed

#ifndef DIRSIZ
#define DIRSIZ		14		/* maximum entry link length	*/
#endif

#ifndef MAXNAMLEN
#define	MAXNAMLEN	DIRSIZ		/* maximum filename length	*/
#endif

#ifdef _dir_getdents

struct dirent				/* data from local getdents()	*/
{
	long		d_ino;		/* file number of entry		*/
	long		d_off;		/* entry seek offset		*/
	unsigned short	d_reclen;	/* length of this record	*/
	char		d_name[1];	/* link name			*/
};

#else

#ifndef _dir_namlen
#define _dir_namlen	1
#endif

struct dirent				/* data from readdir()		*/
{
	long		d_fileno;	/* file number of entry		*/
	unsigned short	d_reclen;	/* length of this record	*/
	unsigned short	d_namlen;	/* strlen(d_name)		*/
	char		d_name[1];	/* link name			*/
};

#endif

typedef struct
{
	int		dd_fd;		/* file descriptor		*/
	int		dd_loc;		/* offset in block		*/
	int		dd_size;	/* amount of valid data		*/
	char*		dd_buf;		/* directory block		*/
} DIR;					/* stream data from opendir()	*/

extern DIR*		opendir();
extern struct dirent*	readdir();
extern long		telldir();
extern void		seekdir();
extern void		closedir();

#if !_dir_local
#define rewinddir(dirp)	seekdir(dirp,0L)
#endif

#endif

#endif

#if _dir_namlen
#define DIRNAMLEN(d)	((d)->d_namlen)
#else
#define DIRNAMLEN(d)	strlen((d)->d_name)
#endif

#endif
!
	;;
fsinfo)	case $genhdr in
	"")	echo "$0: $hdr: genhdr arg required" >&2; exit 1 ;;
	esac
	cat <<!
/*
 * AT&T Bell Laboratories
 * file system user interface definitions
 * : : generated by $0 and $genhdr : :
 */

#ifndef __FSINFO_H__
#define __FSINFO_H__

#ifndef __LCLPARAM_H__
#ifndef S_IFMT
#ifndef TM_DEFAULT
#include <sys/types.h>
#endif
#include <sys/stat.h>
#endif
#endif
!
	nl=
	for i in blocks blksize
	do	cat > $tmp.c <<!
#include <sys/types.h>
#include <sys/stat.h>
static struct stat st;
int bytes = sizeof(st.st_$i);
!
		if	$cc ${1+"$@"} -c $tmp.c >/dev/null 2>&1
		then	case $nl in
			"")	nl=1
				echo
				;;
			esac
			echo "#define stat_$i	1	/* struct stat has st_$i */"
		fi
	done
	cat > $tmp.c <<!
#include <sys/types.h>
#include <sys/stat.h>
#ifndef major
#include <sys/param.h>
#ifndef major
#include <sys/sysmacros.h>
#endif
"#ifndef major" =
#ifdef major
"#define major(x)" = major((x))
#else
"#define major(x)" = ((int)(((unsigned)(x)>>8)&0377))
#endif
"#endif" =
"#ifndef minor" =
#ifdef minor
"#define minor(x)" = minor((x))
#else
"#define minor(x)" = ((int)((x)&0377))
#endif
"#endif" =
"#ifndef makedev" =
#ifdef makedev
"#define makedev(x,y)" = makedev((x),(y))
#else
"#define makedev(x,y)" = ((dev_t)((((x)&0377)<<8)|((y)&0377)))
#endif
"#endif" =
#endif
!
	if	$cc ${1+"$@"} -E $tmp.c > $tmp.i 2>/dev/null
	then	sed -e '/"#/!d' -e 's/.*#/#/' -e 's/"[^=]*=//' $tmp.i
	else	cat <<!

#ifndef major
#define major(x)	((int)(((unsigned)(x)>>8)&0377))
#endif
#ifndef minor
#define minor(x)	((int)((x)&0377))
#endif
#ifndef makedev
#define makedev(x,y)	((dev_t)((((x)&0377)<<8)|((y)&0377)))
#endif
!
	fi
	echo
	$genhdr $hdr || exit 1
	cat <<!
#endif
!
	;;
lclparam)
	HDR=LCLPARAM
	cat <<!
/*
 * AT&T Bell Laboratories
 * <sys/param.h> + <sys/types.h> + <sys/stat.h>
 * : : generated by $0 : :
 */

#ifndef __${HDR}_H__
#define __${HDR}_H__

!
	for i in "#include <sys/param.h>" "#include <sys/param.h>
#ifndef S_IFDIR
#include <sys/stat.h>
#endif" "#include <sys/param.h>
#ifndef S_IFDIR
#include <sys/types.h>
#include <sys/stat.h>
#endif" "#ifndef S_IFDIR
#include <sys/types.h>
#include <sys/stat.h>
#endif"
	do	echo "$i
struct stat V_stat_V;
F_stat_F() { V_stat_V.st_mode = 0; }" > $tmp.c
		if	$cc ${1+"$@"} -c $tmp.c >/dev/null 2>&1
		then	echo "$i"
			break
		fi
	done
	cat <<!

#endif
!
	;;
lcl*)	lcl=`echo $hdr | sed -e 's/lcl//'`
	HDR=`echo $hdr | tr '[a-z]' '[A-Z]'`
	cat <<!
/*
 * AT&T Bell Laboratories
 * untrusted local <$lcl.h>
 * : : generated by $0 : :
 */

#ifndef __${HDR}_H__
#define __${HDR}_H__

!
	echo "#include <$lcl.h>" > $tmp.c
	if	$cc ${1+"$@"} -E $tmp.c > $tmp.i 2>/dev/null
	then	f=`sed -e '/^#[line 	]*1[ 	][ 	]*"\/.*\/'$lcl'\.h"/!d' -e '/\/sys\/'$lcl'\.h"/d' -e s'/.*"\(.*\)".*/\1/' $tmp.i`
	else	f=
	fi
	case $lcl in
	dirent) case $f in
		"")	h=sys/dir ;;
		*)	h=dirent ;;
		esac
		v=
		for i in DIR opendir readdir rewinddir seekdir telldir closedir
		do	cat > $tmp.c <<!
#include <sys/types.h>
#include <$h.h>
#ifndef $i
(
#endif
!
			if	$cc ${1+"$@"} -c $tmp.c >/dev/null 2>&1
			then	v="$v $i"
			fi
		done
		case $v in
		?*)	echo "#if _dir_local"
			for i in $v
			do	echo "#undef $i"
			done
			echo "#endif"
			;;
		esac
		;;
	esac
	case $f in
	"")	echo "/* no local <$lcl.h> */"
		;;
	*)	echo "/* begin local <$lcl.h> */"
		cat $f
		cat <<!
/* end local <$lcl.h> */

#undef	_lcl_$lcl
#define _lcl_$lcl	1
!
		;;
	esac
	cat <<!

#endif
!
	;;
limits)	case $genhdr in
	"")	echo "$0: $hdr: genhdr arg required" >&2; exit 1 ;;
	esac
	cat <<!
/*
 * AT&T Bell Laboratories
 * C language implementation integral type sizes
 * : : generated by $0 and $genhdr : :
 */

#ifndef __LIMITS_H__

#include <lcllimits.h>

#ifndef __LIMITS_H__
#define __LIMITS_H__
#endif

!
	$genhdr $hdr || exit 1
	cat <<!
#endif
!
	;;
preroot)cat <<!
/*
 * AT&T Bell Laboratories
 * preroot interface definitions
 * : : generated by $0 : :
 */

#ifndef __PREROOT_H__
#define __PREROOT_H__

!
	if	(/etc/preroot / /bin/echo) >/dev/null 2>&1
	then	cat <<!
#define FS_PREROOT	1			/* preroot enabled	*/
#define PR_BASE		"CCS"			/* preroot base env var	*/
#define PR_COMMAND	"/etc/preroot"		/* the preroot command	*/
#define PR_REAL		"/dev/.."		/* real root pathname	*/
#define PR_SILENT	"CCSQUIET"		/* no command trace	*/

#if __STDC__ || __cplusplus || c_plusplus
#if __cplusplus
extern "C" {
#endif

extern char*		getpreroot(char* path, char* cmd);
extern int		ispreroot();
extern int		realopen(char* path, int mode, int perm);
extern void		setpreroot(int argc, char** argv, char* dir);

#if __cplusplus
}
#endif

#else

extern char*		getpreroot();
extern int		ispreroot();
extern int		realopen();
extern void		setpreroot();

#endif
!
	else	echo "/* preroot not enabled */"
	fi
	cat <<!

#endif
!
	;;
sig)	cat <<!
/*
 * AT&T Bell Laboratories
 * signal support
 * : : generated by $0 : :
 */

#ifndef __SIG_H__
#define __SIG_H__

#include <signal.h>

#ifndef sigmask
#define sigmask(s)	(1<<((s)-1))
#endif

!
	cat > $tmp.c <<!
#include <signal.h>
#ifdef TYPE
#if __STDC__
typedef TYPE (*signal_t)(int);
#else
typedef TYPE (*signal_t)();
#endif
#endif
signal_t f()
{
	signal_t	handler;
	handler = signal(1, SIG_IGN);
	return(handler);
}
!
	if	$cc ${1+"$@"} -c $tmp.c >/dev/null 2>&1
	then	:
	else	for i in void int
		do	$cc ${1+"$@"} -c -DTYPE=$i $tmp.c >/dev/null 2>&1 && break
		done
		cat <<!
#if __STDC__
typedef $i (*signal_t)(int);
#else
typedef $i (*signal_t)();
#endif

!
	fi
	cat <<!
#endif
!
	;;
sigdata)case $genhdr in
	"")	echo "$0: $hdr: genhdr arg required" >&2; exit 1 ;;
	esac
	cat <<!
/*
 * AT&T Bell Laboratories
 * signal name/text tables
 * : : generated by $0 and $genhdr : :
 */

!
	$genhdr $hdr || exit 1
	;;
ttyinfo)
	HDR=TTYINFO
	cat <<!
/*
 * AT&T Bell Laboratories
 * tty info
 * : : generated by $0 : :
 */

#ifndef __${HDR}_H__
#define __${HDR}_H__

!
	x="/* no local tty info */"
	for i in sgtty termio termios sys/termios sys/ttyio sys/ioctl
	do	echo "#include <$i.h>" > $tmp.c
		if	$cc ${1+"$@"} -c $tmp.c >/dev/null 2>&1
		then	x="#include <$i.h>"
			break
		fi
	done
	cat <<!
$x

#endif
!
	;;
*)	echo "$0: $hdr: unknown header" >&2
	exit 1
	;;
esac
