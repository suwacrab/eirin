#include "eirin.h"

/*	--	init functions	--	*/
void eirin_initPAL4(eirin *yago)
{
	size_t fbsize = sizeof(u8) * ((yago->w>>1) * yago->h);
	yago->m = malloc(fbsize);
}
void eirin_initPAL8(eirin *yago)
{
	size_t fbsize = sizeof(u8) * (yago->w*yago->h);
	yago->m = malloc(fbsize);
}
void eirin_initRGB5A1(eirin *yago)
{
	size_t fbsize = sizeof(u16) * (yago->w*yago->h);
	yago->m = malloc(fbsize);
}
void eirin_initRGB565(eirin *yago)
{
	size_t fbsize = sizeof(u16) * (yago->w*yago->h);
	yago->m = malloc(fbsize);
}
void eirin_initRGB8(eirin *yago)
{
	size_t fbsize = (sizeof(u8)*3) * (yago->w*yago->h);
	yago->m = malloc(fbsize);
}

eirin_initfunc *eirin_initmodes[5] = {
	&eirin_initPAL4,&eirin_initPAL8,
	&eirin_initRGB5A1,&eirin_initRGB565,
	&eirin_initRGB8
};

/*	--	main funcs	--	*/
eirin *eirin_init(eirin *yago,u32 w,u32 h,eirin_pixelfmt fmt)
{
	yago->w = w;
	yago->h = h;
	yago->fmt = fmt;
	yago->palette = NULL;
	
	eirin_initmodes[fmt](yago);
	return yago;
}

eirin *eirin_loadimg(eirin *yago,const char *fname,eirin_pixelfmt fmt)
{	
	// vars n shit
	SDL_Surface *loadimg = NULL; // loaded img
	SDL_Surface *convimg = NULL; // converted img
	// loadin
	loadimg = IMG_Load(fname);
	if(!loadimg)
	{ printf("couldn't load img '%s': %s\n",fname,IMG_GetError()); }	

	if(fmt == EIRIN_PIXELFMT_RGB565)
	{
		SDL_PixelFormat sdlfmt = { 
			NULL,16,2, // pal,bpp,bytes
			3,2,3,0, // loss (rgba)
			11,5,0,0, // shift (rgba)
			31<<11,63<<5,31,0, // masks (rgba)
			0,0x00 // colorkey,alpha
		};
		convimg = SDL_ConvertSurface(loadimg,&sdlfmt,0);
		u32 w = convimg->w;
		u32 h = convimg->h;
		// ???
		eirin_init(yago,w,h,fmt);
		u16 *convpix = (u16*)convimg->pixels;
		u16 *yagopix = (u16*)yago->m;
		for(u32 i=0; i<(w*h); i++)
		{
			u8 r,g,b;
			SDL_GetRGB(convpix[i],&sdlfmt,&r,&g,&b);
			yagopix[i] = RGB565(r>>3,g>>2,b>>3);
		}
	}
	
	printf("loaded image '%s'! [%p]\n",fname,yago);
	// return the loaded img
	return yago;
}
