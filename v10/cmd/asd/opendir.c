/* Copyright (c) 1982 Regents of the University of California */

#include "config.h"
#ifdef LOCALDIR
#include <sys/types.h>
#include <sys/stat.h>
#include "ndir.h"

static char sccsid[] = "@(#)opendir.c 4.2 3/10/82";

/*
 * open a directory.
 */
DIR *
opendir(name)
	char *name;
{
	register DIR *dirp;

	dirp = (DIR *)malloc(sizeof(DIR));
	dirp->dd_fd = open(name, 0);
	if (dirp->dd_fd == -1) {
		free(dirp);
		return NULL;
	}
	dirp->dd_loc = 0;
	return dirp;
}
#endif
