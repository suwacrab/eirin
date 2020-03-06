#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include <time.h>
#include <assert.h>

#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>

#include <math.h>

#ifndef KBASE_H
#define KBASE_H

// basics
#define INLINE static inline
#define SDL_SECOND (1000)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef u8 byte;
typedef u8 pix8;

/*	--	ranges	--	*/
INLINE u32 clamp(u32 x,u32 min,u32 max) // clamps x between min & max
{ return (x < min) ? min : (x > max) ? max : x; } 
INLINE bool in_range(u32 x,u32 min,u32 max) // returns if x is between min & max
{ return (x >= min) && (x <= max); }

/*	--	IO operations	--	*/
INLINE void fread_u8(u8 *pntr,FILE *f) // read little-endian u8 value from file
{ fread(pntr,sizeof(u8),1,f); }
INLINE void fread_u16(u16 *pntr,FILE *f) // read little-endian u16 value from file
{
	u8 b1; fread_u8(&b1,f); // read 1st byte
	u8 b2; fread_u8(&b2,f); // read 2nd byte
	(*pntr) = b1 | (b2<<8);
}
INLINE void fread_u32(u32 *pntr,FILE *f) // read little-endian u32 value from file
{
	u16 hw1; fread_u16(&hw1,f);
	u16 hw2; fread_u16(&hw2,f);
	(*pntr) = hw1 | (hw2<<16);
}

INLINE void fwrite_u8(u8 n,FILE *f) // write little-endian u8 value to file
{ fwrite(&n,sizeof(u8),1,f);}
INLINE void fwrite_u16(u16 n,FILE *f) // write little-endian u16 value to file
{ fwrite_u8(n&0xFF,f); fwrite_u8(n>>8,f); }
INLINE void fwrite_u32(u32 n,FILE *f) // write little-endian u32 value to file
{ fwrite_u16(n&0xFFFF,f); fwrite_u16(n>>16,f); }


/*	--	string types	--	*/
INLINE void str_upper(char *str,u32 len)
{ for(u32 i=0; i<len; i++) str[i] = toupper(str[i]); }
INLINE void str_lower(char *str,u32 len)
{ for(u32 i=0; i<len; i++) str[i] = tolower(str[i]); }

/*	--	fixed-point types	--	*/
typedef s32 FIXED;

#define inttofixed(n,s) ((n)<<(s))
#define fixedtoint(n,s) ((n)>>(s))
INLINE FIXED fix_mul(FIXED a,FIXED b,u32 dec)
{ return (a>>(dec>>1)) * (b>>(dec>>1)); }
INLINE FIXED fix_mul2(FIXED a,FIXED b,u32 dec)
{ return (a*b)>>dec; }

/*	--	vector types	--	*/
typedef struct { FIXED x,y,z; } VEC3;
typedef struct { FIXED x,y; } VEC2;

// -- setting
INLINE void vec3_set(VEC3 *a,s32 x,s32 y,s32 z)
{ a->x = x; a->y = y; a->z = z; }
INLINE void vec2_set(VEC2 *a,s32 x,s32 y)
{ a->x = x; a->y = y; }

// -- adding
INLINE void vec3_add(VEC3 *a,VEC3 *b)
{ a->x += b->x; a->y += b->y; a->z += b->z; }
INLINE void vec2_add(VEC2 *a,VEC2 *b)
{ a->x += b->x; a->y += b->y; }

// -- dividing
INLINE void vec3_div(VEC3 *a,VEC3 *b)
{ a->x /= b->x; a->y /= b->y; a->z /= b->z; }
INLINE void vec2_div(VEC2 *a,VEC2 *b)
{ a->x /= b->x; a->y /= b->y; }

// -- multiplying
INLINE void vec3_mul(VEC3 *a,VEC3 *b)
{ a->x *= b->x; a->y *= b->y; a->z *= b->z; }
INLINE void vec2_mul(VEC2 *a,VEC2 *b)
{ a->x *= b->x; a->y *= b->y; }

// -- multiplying (fixed point)
INLINE void vec3_mulf(VEC3 *a,VEC3 *b,u32 dec)
{ 
  a->x = fix_mul(a->x,b->x,dec);
  a->y = fix_mul(a->y,b->y,dec);
  a->z = fix_mul(a->z,b->z,dec);
}
INLINE void vec2_mulf(VEC2 *a,VEC2 *b,u32 dec)
{
	a->x = fix_mul(a->x,b->x,dec);
	a->y = fix_mul(a->y,b->y,dec);
}

// -- string conversion
INLINE void vec3_str(VEC3 *a,char *str)
{	sprintf(str,"[%d,%d,%d]",a->x,a->y,a->z);	}
INLINE void vec2_str(VEC2 *a,char *str)
{	sprintf(str,"[%d,%d]",a->x,a->y);	}

/*	-- framebuffer ops	--	*/
INLINE void fb_set_u4(u8 *m,u32 w,u32 x,u32 y,u32 d)
{
	u32 ind = (x>>1) + (y*(w>>1));
	u32 orig = m[ind];
	u32 b1 = d<<(4*(x&1)); // if x is odd, shift left.
	u32 b2 = orig&(0xF0>>(4*(x&1))); // if x is odd, use the other pixel.
	m[ind] = b1 | b2;
}

#endif
