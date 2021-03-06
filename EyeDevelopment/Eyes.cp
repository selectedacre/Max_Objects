#include <string.h>
#include "maxincludes.h"
#include "Eyes.h"

#include "imDisplayUtils.h"

extern void *Eyes_class;

void *
Eyes_new(Symbol *s,
         short ac,
         Atom *av)
{
	Eyes *x;

	//post ("Eyes Class new");
	x = (Eyes *) newobject (Eyes_class);
	class_noinlet ((messlist*)Eyes_class);
	// allocate the iMax object.
	x->eyes = new iMAX (x,s,ac,av);
    strcpy (x->name,"eyes");
	x->sendQelem = qelem_new(x, (method)Eyes_sendTick);	// create qelems
    x->sendsQelem = qelem_new(x, (method)Eyes_sendsTick);	// create qelems

	return (x);
}

void
Eyes_free(Eyes *x)
{
	//post ("Eyes free");
	// deallocate the iMax Object.
	qelem_free ((qelem*)x->sendQelem);	// create qelems
	qelem_free ((qelem*)x->sendsQelem);	// create qelems

	delete x->eyes;
}

void
Eyes_bang(Eyes *x)
{	
	if (!x || !x->eyes) return;
	x->eyes->dobang ();
}

void
Eyes_int(Eyes *x,
         long inInt)
{	
	if (!x || !x->eyes) return;
	x->eyes->doint(inInt);
}

void
Eyes_float(Eyes *x,
           float inFloat)
{	
	if (!x || !x->eyes) return;
	x->eyes->dofloat (inFloat);
}

void
Eyes_dump(Eyes *x,
          Symbol *s,
          short ac,
          Atom *av)
{
	if (!x || !x->eyes) return;
	x->eyes->dodump (s,ac,av);
}	

void
Eyes_size(Eyes *x,
          long inx,
          long iny,
          long inxsize,
          long inysize)
{
	if (!x || !x->eyes) return;
	if (inxsize < 1) inxsize = 1;
	else if (inxsize > 99) inxsize = 99;
	if (inysize < 1) inysize = 1;
	else if (inysize > 99) inysize = 99;
	if (inx < 1) inx = 1;
	else if (inx > 99) inx = 99;
	if (iny < 1) iny = 1;
	else if (iny > 99) iny = 99;
	Atom	argv[8];
	SetArgLong(0, argv, inx);
	SetArgLong(1, argv, iny);
	SetArgLong(2, argv, inxsize);
	SetArgLong(3, argv, inysize);
	SetArgLong(4, argv, x->eyes->m_xorig);
	SetArgLong(5, argv, x->eyes->m_yorig);
	SetArgLong(6, argv, x->eyes->m_xsizeorig);
	SetArgLong(7, argv, x->eyes->m_ysizeorig);
	defer_low(x, (method)Eyesresize, NULL, 8, argv);
}	

void Eyes_xsize(Eyes *x,
                long inxsize)
{
	if (!x || !x->eyes) return;
	if (inxsize < 1) inxsize = 1;
	else if (inxsize > 99) inxsize = 99;
	Atom	argv[8];
	SetArgLong(0, argv, x->eyes->m_xper);
	SetArgLong(1, argv, x->eyes->m_yper);
	SetArgLong(2, argv, inxsize);
	SetArgLong(3, argv, x->eyes->m_ysizeper);
	SetArgLong(4, argv, x->eyes->m_xorig);
	SetArgLong(5, argv, x->eyes->m_yorig);
	SetArgLong(6, argv, x->eyes->m_xsizeorig);
	SetArgLong(7, argv, x->eyes->m_ysizeorig);
	defer_low(x, (method)Eyessetxsize, NULL, 8, argv);
}	

void Eyes_ysize(Eyes *x,
                long inysize)
{
	if (!x || !x->eyes) return;
	if (inysize < 1) inysize = 1;
	else if (inysize > 99) inysize = 99;
	Atom	argv[8];
	SetArgLong(0, argv, x->eyes->m_xper);
	SetArgLong(1, argv, x->eyes->m_yper);
	SetArgLong(2, argv, x->eyes->m_xsizeper);
	SetArgLong(3, argv, inysize);
	SetArgLong(4, argv, x->eyes->m_xorig);
	SetArgLong(5, argv, x->eyes->m_yorig);
	SetArgLong(6, argv, x->eyes->m_xsizeorig);
	SetArgLong(7, argv, x->eyes->m_ysizeorig);
	defer_low(x, (method)Eyessetysize, NULL, 8, argv);
}	

void
Eyes_x(Eyes *x,
       long inx)
{
	if (!x || !x->eyes) return;
	if (inx < 1) inx = 1;
	else if (inx > 99) inx = 99;
	Atom	argv[8];
	SetArgLong(0, argv, inx);
	SetArgLong(1, argv, x->eyes->m_yper);
	SetArgLong(2, argv, x->eyes->m_xsizeper);
	SetArgLong(3, argv, x->eyes->m_ysizeper);
	SetArgLong(4, argv, x->eyes->m_x);
	SetArgLong(5, argv, x->eyes->m_y);
	SetArgLong(6, argv, x->eyes->m_xsize);
	SetArgLong(7, argv, x->eyes->m_ysize);
	defer_low(x, (method)Eyessetxloc, NULL, 8, argv);
}	

void
Eyes_y(Eyes *x,
       long iny)
{
	if (!x || !x->eyes) return;
	if (iny < 1) iny = 1;
	else if (iny > 99) iny = 99;
	Atom	argv[8];
	SetArgLong(0, argv, x->eyes->m_xper);
	SetArgLong(1, argv, iny);
	SetArgLong(2, argv, x->eyes->m_xsizeper);
	SetArgLong(3, argv, x->eyes->m_ysizeper);
	SetArgLong(4, argv, x->eyes->m_x);
	SetArgLong(5, argv, x->eyes->m_y);
	SetArgLong(6, argv, x->eyes->m_xsize);
	SetArgLong(7, argv, x->eyes->m_ysize);
	defer_low(x, (method)Eyessetyloc, NULL, 8, argv);
}	

void
Eyes_sensor(Eyes *x,
            Symbol *s,
            short ac,
            Atom *av)
{
	if (!x || !x->eyes) return;
	x->eyes->dosensormap (s,ac,av);
}	

void
Eyes_anything(Eyes *x,
              Symbol *s,
              short ac,
              Atom *av)
{
	if (!x || !x->eyes) return;
	x->eyes->doamessage (s,ac,av);
}	

void
Eyes_assist(Eyes *x,
            void *b,
            long m,
            long a,
            char *s)
{
	if (!x || !x->eyes) return;
	x->eyes->doassist (b,m,a,s);
}

void
Eyes_readyTick(Eyes *x)
{
	if (!x || !x->eyes) return;
}

/* a quelm that looks to see if an image can be sent after processing.*/
void
Eyes_sendTick(Eyes *x)
{
	if (!x || !x->eyes) return;
	//post ("send qelem");
	x->eyes->sendImage ();
	x->eyes->sendResult ();
	qelem_unset(x->sendQelem);
}

/* a quelm that looks to see if an image can be sent after processing.*/
void
Eyes_sendsTick(Eyes *x)
{
	if (!x || !x->eyes) return;
	//post ("send qelem");
	x->eyes->sendImages ();
	x->eyes->sendResults ();
	qelem_unset(x->sendsQelem);
}

/*#P newex 196 205 44 196617 Eyes;*/

void
Eyes_psave(Eyes *x,
           void *dest)
{
	if (!x || !x->eyes) return;
	Atom av[100];
	short ac;
	ac = 0;
	
	SetArgSym (ac,av,gensym("#N"));ac++;
	SetArgSym (ac,av,gensym(x->myName ()));ac++;
	SetArgSym (ac,av,gensym("_binbuf_"));ac++;//if this exists, then we are restoring from a binbuf.
	SetArgLong (ac,av,PSTART);ac++;// 10 parameters are saved in the binbuf.
	SetArgLong (ac,av,x->eyes->displayWindpub->w_x1);ac++;
	SetArgLong (ac,av,x->eyes->displayWindpub->w_y1);	ac++;
	SetArgLong (ac,av,x->eyes->displayWindpub->w_x2-x->eyes->displayWindpub->w_x1);ac++;
	SetArgLong (ac,av,x->eyes->displayWindpub->w_y2-x->eyes->displayWindpub->w_y1);ac++;
	SetArgLong (ac,av,x->eyes->mHidden);ac++;
	SetArgLong (ac,av,x->eyes->mKeepAspect);ac++;
	SetArgLong (ac,av,x->eyes->mFull);ac++;
	SetArgLong (ac,av,x->eyes->mOnOff);ac++;
	SetArgFloat (ac,av,x->eyes->scale);ac++;
	if (x->eyes->op) {
	    SetArgSym (ac,av,gensym(x->eyes->op->myName ()));ac++;
	}
	else {
	    SetArgSym (ac,av,gensym("display"));ac++;
	}
	// must save what is written on the object as well.
	x->eyes->dosave (ac,av);
	
	binbuf_insert ((binbuf *)dest,NULL,ac,av);
/*	post ("save it::::::%d %d %d %d   hide:%d ka:%d full:%d on:%d scale:%f",			x->eyes->displayWindpub->w_x1,
			x->eyes->displayWindpub->w_y1,
			x->eyes->displayWindpub->w_x2-x->eyes->displayWindpub->w_x1,
			x->eyes->displayWindpub->w_y2-x->eyes->displayWindpub->w_y1,
			x->eyes->mHidden,x->eyes->mKeepAspect,x->eyes->mFull,x->eyes->mOnOff,x->eyes->scale);
 */
}

void
Eyes_dblclick(Eyes *x)
{
	if (!x) return;
	post ("double click");
	defer(x, (method)show_hide_Windows, NULL, 0, NULL);
}

void
Eyes_click(Eyes *x,
           Point pt,
           short dblClick,
           short modifiers)
{
	if (!x) return;
	if (x->eyes->doClick (pt, dblClick, modifiers)) {
    	if (x->eyes->maximage) { // original image?
    		post ("(x,y)=(%4d,%4d)",
    				(int)((double)pt.h/(double)(x->eyes->imageRect.bottom -
    				    x->eyes->imageRect.top )*(double)x->eyes->maximage->im->ysize),
    				(int)((double)pt.v/(double)(x->eyes->imageRect.right  -
    				    x->eyes->imageRect.left)*(double)x->eyes->maximage->im->xsize)
    			 );
    	}
	}
}

// Eyes_class messages
void
Eyes_show(Eyes *x)
{
	if (!x) return;
	defer_low(x,(method) ShowDisplay, NULL, 0, NULL);
}

void
Eyes_hide(Eyes *x)
{
	if (!x) return;
	defer_low(x,(method) HideDisplay, NULL, 0, NULL);
}

void
Eyes_dsize(Eyes *x,
           long width,
           long height)
{
	if (!x) return;
	Atom	argv[2];

	doDisplaySizeCheck(width,height)

	// defer call ChangeOutputDimensions
	SetArgLong(0, argv, width);
	SetArgLong(1, argv, height);
	defer_low(x, (method)ChangeDisplayDimensions, NULL, 2, argv);
}

void
Eyes_scale(Eyes *x,
           float inscale)
{
	if (!x) return;
	if (inscale > 6) inscale = 6;
	else if (inscale < 0) inscale = 0;

	x->eyes->scale = 1.0/(float)inscale;
}

void
Eyes_loc(Eyes *x,
         long width,
         long height)
{
	if (!x) return;
	Atom	av[2];
	doLocationCheck(width,height)
	// defer call ChangeOutputDimensions
	SetArgLong(0, av, width);
	SetArgLong(1, av, height);

	defer_low(x, (method)ChangeDisplayPosition, NULL, 2, av);
}

void
Eyes_windowresize(Eyes *x,
                  short hsize,
                  short vsize)
{
   // post ("window resized!!!");
    if (x->eyes->mKeepAspect) {
        // make sure the window stays in the right proportions.
        // check y and scale x accordingly.
        Eyes_dsize (x,(int)( (double) vsize * (double) x->eyes->ix->img->xsize/ (double) x->eyes->ix->img->ysize) ,vsize);
    }
}

void
Eyes_update(Eyes *x)
{
	if (!x) return;
}

void
Eyes_aspect(Eyes *x)
{
	if (!x) return;
    HideDisplay (x);
   	Eyes_dsize (x,x->eyes->ix->img->xsize,x->eyes->ix->img->ysize);
	ShowDisplay (x);
	x->eyes->mKeepAspect = true;
}

void
Eyes_no_aspect(Eyes *x) // fill & no aspect.
{
	if (!x) return;
	x->eyes->mKeepAspect = false;
}

void
Eyes_full(Eyes *x)
{
	if (!x) return;
	x->eyes->mFull = true;
}

void
Eyes_window(Eyes *x)
{
	if (!x) return;
	x->eyes->mFull = false;
}

/*
        Defered actions.
 */
void
ChangeDisplayPosition(Eyes *x,
                      Symbol* temp,
                      int ac,
                      Atom* av)
{
	if (!x || !x->eyes || !x->eyes->backGWorld) return;

	OSErr	anErr;
	short	width, height;

	// get width, height args
	width = (short) ArgIfLongOrFloat(0,av);
	height = (short) ArgIfLongOrFloat(1,av);
	MoveWindow((WindowPtr) &x->eyes->displayWindpub->w_gp, width, height, FALSE);
	EraseRect (&x->eyes->backGWorld->portRect);
}

void
ChangeDisplayDimensions(Eyes *x,
                        Symbol* temp,
                        int ac,
                        Atom* av)
{
	if (!x || !x->eyes || !x->eyes->backGWorld) return;
	OSErr	anErr;
	short	width, height;

	// get width, height args
	width = (short) ArgIfLongOrFloat(0,av);
	height = (short) ArgIfLongOrFloat(1,av);
	// resize windpub window, copy dimensions into windpub's internal values
	SizeWindow((WindowPtr) &x->eyes->displayWindpub->w_gp, width, height, FALSE);
	x->eyes->displayWindpub->w_x2 = x->eyes->displayWindpub->w_x1+x->eyes->displayWindpub->w_gp.portRect.right;
	x->eyes->displayWindpub->w_y2 = x->eyes->displayWindpub->w_y1+x->eyes->displayWindpub->w_gp.portRect.bottom;
	EraseRect (&x->eyes->backGWorld->portRect);

	// HideDisplay (x);
	// ShowDisplay (x);
}

void
ShowDisplay(Eyes *x)
{
	if (!x) return;
	x->eyes->mHidden = false;
	wind_vis ((Wind*) x->eyes->displayWindpub);
}

void
HideDisplay(Eyes *x)
{
	if (!x) return;
	x->eyes->mHidden = true;
	wind_invis ((Wind*) x->eyes->displayWindpub);
}

void
show_hide_Windows(Eyes *x)
{
	if (!x) return;
	x->eyes->mHidden = x->eyes->mHidden == false;
	if (x->eyes->mHidden) {
		HideDisplay (x);
	}
	else {
		ShowDisplay (x);
	}
}

/*
extern void
resizeimage(Eyes *x);

void
resizeimage(Eyes *x)
{
	if (!x) return;\
	    post ("resize image to:%d %d %d %d",x->eyes->maximage->m_x,x->eyes->maximage->m_y,x->eyes->maximage->m_xsize,x->eyes->maximage->m_ysize);
		x->eyes->imagefree ();
		// initialize this objects image according to its window settings.
		// maximage is the incomming image.
		x->eyes->imageinit (x->eyes->maximage->m_x,x->eyes->maximage->m_y,x->eyes->maximage->m_xsize,x->eyes->maximage->m_ysize);
		x->eyes->allocate_offbounds ();
}
*/

void
Eyesresize(Eyes *x,
           Symbol *s,
           int ac,
           Atom *av)
{
	// post ("size image");
	long inx, iny, inxsize, inysize;
	inx = (long) ArgLong(0,av);
	iny = (long) ArgLong(1,av);
	inxsize = (long) ArgLong(2,av);
	inysize = (long) ArgLong(3,av);
	x->eyes->set_window_size (inx,iny,inxsize,inysize);

	inx = (long) ArgLong(4,av);
	iny = (long) ArgLong(5,av);
	inxsize = (long) ArgLong(6,av);
	inysize = (long) ArgLong(7,av);
	x->eyes->imageinit (inx,iny,inxsize,inysize);
	x->eyes->allocate_offbounds ();
}

void
Eyessetxloc(Eyes *x,
            Symbol *s,
            int ac,
            Atom *av)
{
	// post ("xloc image");
	long inx, iny, inxsize, inysize;
	inx = (long) ArgLong(0,av);
	iny = (long) ArgLong(1,av);
	inxsize = (long) ArgLong(2,av);
	inysize = (long) ArgLong(3,av);
	x->eyes->set_window_size (inx,iny,inxsize,inysize);
	
	inx = (long) ArgLong(4,av);
	iny = (long) ArgLong(5,av);
	inxsize = (long) ArgLong(6,av);
	inysize = (long) ArgLong(7,av);
	x->eyes->im->set (inx,iny,inxsize,inysize);
	x->eyes->ix->set (x->eyes->im);
	x->eyes->allocate_offbounds ();
}

void
Eyessetyloc(Eyes *x,
            Symbol *s,
            int ac,
            Atom *av)
{
	// post ("size image");
	long inx, iny, inxsize, inysize;
	inx = (long) ArgLong(0,av);
	iny = (long) ArgLong(1,av);
	inxsize = (long) ArgLong(2,av);
	inysize = (long) ArgLong(3,av);
	x->eyes->set_window_size (inx,iny,inxsize,inysize);

	inx = (long) ArgLong(4,av);
	iny = (long) ArgLong(5,av);
	inxsize = (long) ArgLong(6,av);
	inysize = (long) ArgLong(7,av);
	x->eyes->im->set (inx,iny,inxsize,inysize);
	x->eyes->ix->set (x->eyes->im);
	x->eyes->allocate_offbounds ();
}

void
Eyessetxsize(Eyes *x,
             Symbol *s,
             int ac,
             Atom *av)
{
	// post ("xsize image");
	long inx, iny, inxsize, inysize;
	inx = (long) ArgLong(0,av);
	iny = (long) ArgLong(1,av);
	inxsize = (long) ArgLong(2,av);
	inysize = (long) ArgLong(3,av);
	x->eyes->set_window_size (inx,iny,inxsize,inysize);

	inx = (long) ArgLong(4,av);
	iny = (long) ArgLong(5,av);
	inxsize = (long) ArgLong(6,av);
	inysize = (long) ArgLong(7,av);
	x->eyes->imageinit (inx,iny,inxsize,inysize);
	x->eyes->allocate_offbounds ();

}

void
Eyessetysize(Eyes *x,
             Symbol *s,
             int ac,
             Atom *av)
{
	// post ("ysize image");
	long inx, iny, inxsize, inysize;
	inx = (long) ArgLong(0,av);
	iny = (long) ArgLong(1,av);
	inxsize = (long) ArgLong(2,av);
	inysize = (long) ArgLong(3,av);
	x->eyes->set_window_size (inx,iny,inxsize,inysize);

	inx = (long) ArgLong(4,av);
	iny = (long) ArgLong(5,av);
	inxsize = (long) ArgLong(6,av);
	inysize = (long) ArgLong(7,av);
	x->eyes->imageinit (inx,iny,inxsize,inysize);
	x->eyes->allocate_offbounds ();
}

void
Eyes_image(Eyes *x,
           Symbol *s,
           short ac,
           Atom *av)
{
	if (!x || !x->eyes) return;
	post ("eyes image:%d",x->eyes->mOnOff);
	x->eyes->doimage (s,ac,av);
	if (x->eyes->mOnOff)
		defer (x,(method)EyesprocessImage,NULL,0,NULL);
}	

void
Eyes_images(Eyes *x,
            Symbol *s,
            short ac,
            Atom *av)
{
	if (!x || !x->eyes) return;
	post ("eyes imageS:%d",x->eyes->mOnOff);
	x->eyes->doimages (s,ac,av);
	if (x->eyes->mOnOff)
	    defer (x,(method)EyesprocessImages,NULL,0,NULL);
}	

void
EyesprocessImage(Eyes *x,
                 Symbol *s,
                 short ac,
                 Atom av)
{
	if (!x || !x->eyes) return;
    post ("process image  %u",x->eyes);
	if (x->eyes->processImage ()) {
	    if (x->sendQelem) {
	        qelem_set(x->sendQelem);
	    }
    }
}

void
EyesprocessImages(Eyes *x,
                  Symbol *s,
                  short ac,
                  Atom av)
{
	if (!x || !x->eyes) return;
    post ("process imageS  %u",x->eyes);
	if (x->eyes->processImage ()) {
	    if (x->sendsQelem) {
	        qelem_set(x->sendsQelem);
	    }
    }
}

