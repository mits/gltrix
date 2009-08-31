/*
 * glamourlibsdl.cpp
 *
 *  Created on: Aug 4, 2009
 *      Author: mits
 */

#include "SDL/SDL.h"
#include "gltrixsdl.h"
#include "gltrixgl.h"
#include "defs.h"

SDL_Surface *screen;
int done;
Uint8 *keys;


texBitmap_t *loadBMP(const char *filename)
{
	if (!filename)
	{
		fprintf(stderr,"filename?\n");
		return NULL;
	}
	SDL_Surface *image;
	image = SDL_LoadBMP(filename);
	if (image ==NULL)
	{
		fprintf(stderr,"could not load bmp\n");
		return NULL;
	}
	texBitmap_t *tbmp;
	tbmp = (texBitmap_t *)malloc(sizeof(texBitmap_t));
	tbmp->data = (GLubyte *)malloc(sizeof(GLubyte) * image->w * image->h * 3);
	tbmp->width = image->w;
	tbmp->height = image->h;
	tbmp->bpp = 3;
	tbmp->pitch = tbmp->width * tbmp->bpp;
	uint8 *tLine;
	uint8 *iLine;
	int x,y;
	iLine = (uint8 *)image->pixels;
	tLine = (uint8 *)&(tbmp->data[tbmp->pitch * (tbmp->height-1)]);

	for (y=0;y<image->h;y++)
	{

		for (x=0;x<image->pitch;x=x+3)
		{
			tLine[x] = iLine[x+2];
			tLine[x+1] = iLine[x+1];
			tLine[x+2] = iLine[x];
		}
		iLine += image->pitch;
		tLine -= tbmp->pitch;

	}
	SDL_FreeSurface(image);

	return tbmp;
}

void processSDLEvents()
{
	SDL_Event event;
	while ( SDL_PollEvent(&event) )
	{
		switch(event.type)
		{
			case SDL_VIDEORESIZE:
				screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 32, SDL_OPENGL|SDL_RESIZABLE);
				if (screen)
					reshape(screen->w, screen->h);
				else { /* Uh oh, we couldn't set the new video mode?? */; }
				break;
			case SDL_QUIT:
				done = 1;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					done =1;
					break;
				case SDLK_l:
					toggleLighting();
					break;
				case SDLK_f:
					changeFilter();
					break;
				case SDLK_b:
					toggleBlending();
					break;
				case SDLK_z:
					theCamera->rotateLocal(3.14159/3,1.0,0.0f,0.0f);
				default:
					break;
				}
		}
	}
}

void processSDLKeystates(KeyMaster *km)
{
	keys = SDL_GetKeyState(NULL);
	if ( keys[SDLK_ESCAPE] )
	{
		done = 1;
	}
	if ( keys[SDLK_UP] )
		km->setKeyDown(key_up, true);
	else
		km->setKeyDown(key_up, false);
	if ( keys[SDLK_DOWN] )
		km->setKeyDown(key_down, true);
	else
		km->setKeyDown(key_up, false);
	if ( keys[SDLK_LEFT] )
		km->setKeyDown(key_left, true);
	else
		km->setKeyDown(key_left, false);
	if ( keys[SDLK_RIGHT] )
		km->setKeyDown(key_right, true);
	else
		km->setKeyDown(key_right, false);
	if ( keys[SDLK_p] )
		{
			km->setKeyDown(key_p, true);
		}
	else
		km->setKeyDown(key_p, false);
	if (keys[SDLK_LEFT]) theCamera->moveLocal(-0.2f,0.0f,0.0f);
	if (keys[SDLK_RIGHT]) theCamera->moveLocal(0.2f,0.0f,0.0f);
	if (keys[SDLK_UP]) theCamera->moveLocal(0.0f,0.0f,-0.2f);
	if (keys[SDLK_DOWN]) theCamera->moveLocal(0.0f,0.0f,0.2f);
	if (keys[SDLK_PAGEUP]) theCamera->moveLocal(0.0f,0.2f,0.0f);
	if (keys[SDLK_PAGEDOWN]) theCamera->moveLocal(0.0f,-0.2f,0.0f);
	if (keys[SDLK_a]) theCamera->rotateLocal(0.01f,0.0,1.0,0.0);
	if (keys[SDLK_d]) theCamera->rotateLocal(-0.01f,0.0,1.0,0.0);
	if (keys[SDLK_w]) theCamera->rotateLocal(0.01f,1.0,0.0,0.0);
	if (keys[SDLK_s]) theCamera->rotateLocal(-0.01f,1.0,0.0,0.0);
	if (keys[SDLK_q]) theCamera->rotateLocal(0.01f,0.0,0.0,1.0);
	if (keys[SDLK_e]) theCamera->rotateLocal(-0.01f,0.0,0.0,1.0);
	if (keys[SDLK_g]) theCamera->rotateGlobal(0.01,0.0,1.0,0.0);
	if (keys[SDLK_j]) theCamera->rotateGlobal(-0.01,0.0,1.0,0.0);
	if (keys[SDLK_y]) theCamera->rotateGlobal(0.01,1.0,0.0,0.0);
	if (keys[SDLK_h]) theCamera->rotateGlobal(-0.01,1.0,0.0,0.0);

	/*		if ( keys[SDLK_z] )
	{
		if ( SDL_GetModState() & KMOD_SHIFT )
		{
			view_rotz -= 5.0;
		}
		else
		{
			view_rotz += 5.0;
		}
	}
		*/
}
