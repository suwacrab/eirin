#include "kbase.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#ifndef EIRIN_H
#define EIRIN_H

// rgb inlines & defs
INLINE u16 RGB5A1(u32 r,u32 g,u32 b) { return r | (g<<5) | (b<<10) | 0x8000; }
INLINE u16 RGB565(u32 r,u32 g,u32 b) { return r | (g<<5) | (b<<11); }
INLINE u32 RGBA32(u32 r,u32 g,u32 b,u32 a) { return r | (g<<8) | (b<<16) | (a<<24); }

typedef u16 RGB16;
typedef u32 RGB32;

/*	--	enums	--	*/
typedef enum eirin_pixelfmt
{
	EIRIN_PIXELFMT_PAL4, // 16-color palette, first pixel == first nybble
	EIRIN_PIXELFMT_PAL8, // 256-color palette, each pixel == 1 byte
	EIRIN_PIXELFMT_RGB5A1, // 16-bit high clr (1BBBBBGGGGGRRRRR in bits)
	EIRIN_PIXELFMT_RGB565, // 16-bit high clr (BBBBBGGGGGGRRRRR in bits)
	EIRIN_PIXELFMT_RGB8 // 24-bit true color (BBBBBBBBGGGGGGGGRRRRRRRR in bits)
} eirin_pixelfmt;

/*	--	structs	--	*/
typedef struct eirin
{
	u32 w,h; // width,height
	eirin_pixelfmt fmt; // pixel format
	void *palette; // color palette
	void *m; // pixel matrix
} eirin;

typedef void eirin_initfunc(eirin*);

/*	--	main functions	--	*/
extern eirin *eirin_init(eirin *yago,u32 w,u32 h,eirin_pixelfmt fmt);
eirin *eirin_loadimg(eirin *yago,char *fname,eirin_pixelfmt fmt);

/*	--	init functions	--	*/
extern eirin_initfunc *eirin_initmodes[5];
extern void eirin_initPAL4(eirin *yago);
extern void eirin_initPAL8(eirin *yago); 
extern void eirin_initRGB5A1(eirin *yago);
extern void eirin_initRGB565(eirin *yago);
extern void eirin_initRGB8(eirin *yago);

#endif
