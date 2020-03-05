#include "kbase.h"
#include "eirin.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define WIDTH (320)
#define HEIGHT (240)

// structs
typedef GLfloat vert3f[3];

// const gl vars
const vert3f quad[4] = {
	{-1.0,-1.0, 0.0}, // top L
	{ 1.0,-1.0, 0.0}, // top R
	{ 1.0, 1.0, 0.0}, // bot R
	{-1.0, 1.0, 0.0} // bot L
};
// sdl vars
u32 frames = 0;
SDL_Surface *window = NULL;
uint8_t *keystate = NULL;
bool quit = false;
// asset vars
void **img_bank = NULL;
GLuint gltex[0x10];
// fumcs
void init_sdl();
void init_gl();
void init_asset();

void update();
void draw();

void checkquit(bool *quitflag);

// main func
int main(void)
{
	// init
	printf("initializing...\n");
	init_sdl();
	init_gl();
	init_asset();
	printf("finished initializing!\n");
	// main
	while(!quit)
	{
		update();
		draw();
		SDL_Delay( SDL_SECOND/60 );
		frames++;
	}
	// exit
	return 0;
}

// other funcs
void init_sdl()
{
	// init sdl
	s32 imgflags = IMG_INIT_PNG;
	IMG_Init(imgflags);
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{ printf("couldn't init SDL: '%s\n'\n",SDL_GetError()); exit(-1); }
	// init gl attributes (RGB555,16 bit z-buffer,double-buffered)
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE,5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE,5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE,5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE,16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER,1 );
	// set video flags
	s32 sdlflags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
	window = SDL_SetVideoMode(WIDTH,HEIGHT,16,sdlflags);
	if(window == NULL)
	{ printf("couldn't init opengl: '%s\n'\n",SDL_GetError()); exit(-1); }
}

void init_gl()
{
	float ratio = (float)WIDTH / (float)HEIGHT;
	// shading
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT); // flat shading
	// culling
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	// clear color & viewport
	glClearColor( .0,.0,.0,.0 );
	glViewport(0,0,WIDTH,HEIGHT);
	// depth shit
	glClearDepth( 1.0f );
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 60.0,ratio,1.0,1024.0 );
}

void init_asset()
{
	// allocating
	img_bank = malloc(sizeof(void*)*0x10);
	img_bank[0] = malloc(sizeof(RGB16)*(0x10*0x10));
	// test image loadin
	RGB16 *testimg = (RGB16*)img_bank[0];
	for(u32 y=0; y<0x10; y++)
	{
		for(u32 x=0; x<0x10; x++)
		{
			u32 r = x<<4; // 255, if x==16.
			u32 g = 0xFF; // 255.
			u32 b = y<<4; // 255, if y == 16.
			RGB16 clr = RGB565(r>>3,g>>3,b>>3);
			testimg[x + (y*0x10)] = clr;
		}
	}

	printf("%04X\n",testimg[0]);
	// GL image loading
	glGenTextures(2,gltex);
	glBindTexture(GL_TEXTURE_2D,gltex[0]); // set texture to 0
	glTexImage2D(
		GL_TEXTURE_2D,0, // target,level
		GL_RGB,0x10,0x10, // internal fmt,size
		0,GL_RGB,GL_UNSIGNED_SHORT_5_6_5, // border,pixel fmt,type
		testimg // data
	);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // min
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // mag
	
	// file image loadin
	eirin loadimg; eirin_loadimg(
		&loadimg,"gfx/player-proto.png",EIRIN_PIXELFMT_RGB565
	);
	
	glBindTexture(GL_TEXTURE_2D,gltex[1]);
	glTexImage2D(
		GL_TEXTURE_2D,0, // target,level
		GL_RGB5,loadimg.w,loadimg.h, // internal fmt,size
		0,GL_RGB,GL_UNSIGNED_SHORT_5_6_5, // border,pixel fmt,type
		loadimg.m // data
	);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // min
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // mag
	
}

void update()
{
	// update key state, check quit
	SDL_PumpEvents();
	keystate = SDL_GetKeyState(NULL);
	checkquit(&quit);
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	// moving & rotating
	float ftime = (float)frames;
	float z = -3.0f;
	glTranslatef(0,0,z );
	glRotatef(ftime*.2,1.0f,0.0f,0.0f);
	glRotatef(ftime,0.0f,0.0f,1.0f);
	
	// quad vars
	GLubyte white[4] = { 0xFF,0xFF,0xFF,0xFF };
	GLubyte red[4] = { 0xFF,0x00,0x00,0xFF };
	
	// quad drawin
	//glColor4ubv(white);
	glBindTexture(GL_TEXTURE_2D,gltex[1]);
	for(u32 i=0; i<2; i++)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f); glVertex3fv((GLfloat*)&quad[0]);
			glTexCoord2f(1.0f,0.0f); glVertex3fv((GLfloat*)&quad[1]);
			glTexCoord2f(1.0f,1.0f); glVertex3fv((GLfloat*)&quad[2]);
			glTexCoord2f(0.0f,1.0f); glVertex3fv((GLfloat*)&quad[3]);
		glEnd();
		glRotatef(180.0f, 1.0f,0,0);
	}
	
	glPopMatrix();
	SDL_GL_SwapBuffers();
}

void checkquit(bool *quitflag)
{
	bool do_quit = false;
	do_quit = keystate[SDLK_ESCAPE];
	*quitflag = do_quit;
}
