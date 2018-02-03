#include "copyright.h"

/* $Header: XVisUtil.c,v 11.8 87/09/11 08:15:44 toddb Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#include <stdio.h>
#include "Xlibint.h"
#include "Xutil.h"
/*
 *	This procedure returns a list of visual information structures
 *	that match the specified attributes given in the visual information 
 *	template.
 *
 *	If no visuals exist that match the specified attributes, a NULL is
 *	returned.
 *
 *	The choices for visual_info_mask are:
 *
 *		VisualNoMask
 *		VisualIDMask
 *		VisualScreenMask
 *		VisualDepthMask
 *		VisualClassMask
 *		VisualRedMaskMask
 *		VisualGreenMaskMask
 *		VisualBlueMaskMask
 *		VisualColormapSizeMask
 *		VisualBitsPerRGBMask
 *		VisualAllMask
 */

XVisualInfo *XGetVisualInfo( dpy, visual_info_mask, 
	visual_info_template, nitems)
Display *dpy;
register long visual_info_mask; 
register XVisualInfo *visual_info_template;
int *nitems;	/* RETURN */
{

  register Visual *vp;
  register Depth *dp;
  Screen *sp;
  int ii,screen_s,screen_e,total,count;
  register XVisualInfo *vip,*vip_base;

  /* NOTE: NO HIGH PERFORMING CODE TO BE FOUND HERE */

  LockDisplay(dpy);

  /* ALLOCATE THE ORIGINAL BUFFER; REALLOCED LATER OF OVERFLOW OCCURS;
     FREED AT END IF NO VISUALS ARE FOUND */

  count = 0;
  total = 10;
  vip_base = vip = (XVisualInfo *)Xmalloc(sizeof(XVisualInfo)*total);

  /* DETERMINE IF WE DO ALL SCREENS OR ONLY ONE */

  screen_s = 0;
  screen_e = dpy->nscreens;
  if (visual_info_mask & VisualScreenMask)
    {
      screen_s = visual_info_template->screen;
      screen_e = screen_s + 1;
    }

  /* LOOP THROUGH SCREENS */

  for (ii=screen_s; ii<screen_e; ii++)
    {
      sp = (Screen *)(&dpy->screens[ii]);

      /* LOOP THROUGH DEPTHS */

      for (dp=sp->depths; dp < (sp->depths + sp->ndepths); dp++)
        {
          if ((visual_info_mask & VisualDepthMask) &&
	      (dp->depth != visual_info_template->depth)) continue;

          /* LOOP THROUGH VISUALS */

          for (vp=dp->visuals; vp<(dp->visuals + dp->nvisuals); vp++)
            {
              if ((visual_info_mask & VisualIDMask) &&
                (vp->visualid != visual_info_template->visualid)) continue;
              if ((visual_info_mask & VisualClassMask) &&
                (vp->class != visual_info_template->class)) continue;
              if ((visual_info_mask & VisualRedMaskMask) &&
                (vp->red_mask != visual_info_template->red_mask)) continue;
              if ((visual_info_mask & VisualGreenMaskMask) &&
                (vp->green_mask != visual_info_template->green_mask)) continue;
              if ((visual_info_mask & VisualBlueMaskMask) &&
                (vp->blue_mask != visual_info_template->blue_mask)) continue;
              if ((visual_info_mask & VisualColormapSizeMask) &&
                (vp->map_entries != visual_info_template->colormap_size)) continue;
              if ((visual_info_mask & VisualBitsPerRGBMask) &&
                (vp->bits_per_rgb != visual_info_template->bits_per_rgb)) continue;

              /* YEA!!! WE FOUND A GOOD ONE */
 
              if (count+1 > total)
                {
                  total += 10;
                  vip_base = (XVisualInfo *)Xrealloc(vip_base,sizeof(XVisualInfo)*total);
                  vip = &vip_base[count];
                }

              count++;

              vip->visual = vp;
              vip->visualid = vp->visualid;
              vip->screen = ii;
              vip->depth = dp->depth;
              vip->class = vp->class;
              vip->red_mask = vp->red_mask;
              vip->green_mask = vp->green_mask;
              vip->blue_mask = vp->blue_mask;
              vip->colormap_size = vp->map_entries;
              vip->bits_per_rgb = vp->bits_per_rgb;

              vip++;

            } /* END OF LOOP ON VISUALS */
          
        } /* END OF LOOP ON DEPTHS */

    } /* END OF LOOP ON SCREENS */

  UnlockDisplay(dpy);

  if (count)
    {
      *nitems = count;
      return vip_base;
    }


  Xfree(vip_base);

  *nitems = 0;

  return NULL;

}


/*
 *	This procedure will return the visual information for a visual 
 *      that matches the specified depth and class for a screen.  Since 
 *	multiple visuals may exist that match the specified depth and 
 *	class, which visual chosen is undefined.
 *
 *	If a visual is found, True is returned as the function value,
 *	otherwise False is returned.
 */

Status XMatchVisualInfo( dpy, screen, depth, class, visual_info)
	Display *dpy;
	int screen;
	int depth;
	int class;
	XVisualInfo *visual_info; /* RETURNED */
{

  Visual *vp;
  Depth *dp;
  Screen *sp;
  int ii,jj;

  LockDisplay(dpy);

  sp = (Screen *)(&dpy->screens[screen]);

  dp = sp->depths;

  for (ii=0; ii < sp->ndepths; ii++)
    {

    /* LOOK THROUGH DEPTHS FOR THE WANTED DEPTH */

    if (dp->depth == depth)
      {
        vp = dp->visuals;

        /* LOOK THROUGH VISUALS FOR THE WANTED CLASS */

        for (jj=0; jj<dp->nvisuals; jj++)
          {
            if (vp->class == class) 
              {
		visual_info->visual = vp;
		visual_info->visualid = vp->visualid;
		visual_info->screen = screen;
		visual_info->depth = depth;
		visual_info->class = vp->class;
		visual_info->red_mask = vp->red_mask;
		visual_info->green_mask = vp->green_mask;
		visual_info->blue_mask = vp->blue_mask;
		visual_info->colormap_size = vp->map_entries;
		visual_info->bits_per_rgb = vp->bits_per_rgb;
                UnlockDisplay(dpy);
                return True;
              }
            vp++;
          }
      }

    dp++;

    }

  UnlockDisplay(dpy);

  return False;

}
