# include	"trek.h"

/**
 **	shield and cloaking device control
 **/

CVNTAB Udtab[]
{
	"d",		"own",
	"u",		"p",
	0
};

shield()
{
	protect(SHIELD);
}

cloak()
{
	protect(CLOAK);
}

protect(sw)
int	sw;
{
	register int		i;
	char			*device;
	int			ind;
	char			*stat;

	if (sw == CLOAK)
	{
		/* cloaking device */
		if (Status.ship == QUEENE) {
			error("Ye Faire Queene does not have the cloaking device.\n");
			return;
		}
		device = "Cloaking device";
		ind = CLOAK;
		stat = &Status.cloaked;
	}
	else
	{
		/* shields */
		device = "Shields";
		ind = SHIELD;
		stat = &Status.shldup;
	}
	if (Damage[ind])
	{
		error(device, " inoperable\n");
		return;
	}
	if(sw) {
		if(lineended()) {
			printf("%s %s; ", device,
				(*stat?"up":"down")
				);
		}
		if((i=getcodpar("Up or down", Udtab))<0) return;
	} else {
		i = !*stat;
	}
	if(*stat==i)
		return;
	if (i)
		if(sw!=CLOAK)
			Status.energy =- Param.shupengy;
		else
			Status.cloakdate = Status.date;
	Move.free = 0;
	if(sw!=CLOAK)
		Move.shldchg = 1;
	*stat = i;
	return;
}
