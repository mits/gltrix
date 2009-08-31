/*
 * main.cpp
 *
 *  Created on: Aug 1, 2009
 *      Author: mits
 */

#include "GL/gl.h"
#include "GL/glu.h"
#include "SDL/SDL.h"
#include <cstdio>
#include <cstddef>
#include <cstdlib>


#include "gltrixgl.h"
#include "gltrixsdl.h"
#include "gltrixgame.h"





int main(int argc, char *argv[])
{
	Uint32 oldTicks,newTicks;
	KeyMaster *keyMaster  = new KeyMaster;
	Board *board = new Board(10,15);
	SDL_Init(SDL_INIT_VIDEO);

	screen = SDL_SetVideoMode(800, 600, 32, SDL_OPENGL|SDL_RESIZABLE);
	if ( ! screen ) {
		fprintf(stderr, "Couldn't init GL video mode: %s\n", SDL_GetError());
		SDL_Quit();
		exit(2);
	}
	SDL_WM_SetCaption("GLtrix", "gltrix");

	initGL();
	reshape(screen->w,screen->h);
	Brick *brick = new Brick(1,board,5,5,Color::green);
	Brick *brick2 = new Brick(2,board,8,8,Color::blue);
	Brick *brick3 = new Brick(3,board,0,0,Color::red);
	Brick *brick4 = new Brick(4,board,10,10,Color::white);
	theCamera = new Camera();
	theCamera->setPosition(0.0,0.0,-60);
	setOffsetsFromSize(15,15);
	addRenderable(brick);
	addRenderable(brick2);
	addRenderable(brick3);
	addRenderable(brick4);
	addRenderable(board);
	addDynamic(brick);
	oldTicks = newTicks = SDL_GetTicks();
	done = 0;
	while ( ! done )
	{
		oldTicks = newTicks;
		newTicks = SDL_GetTicks();
		reshape(800,600);
		processSDLEvents();
		processSDLKeystates(keyMaster);
		refreshGameState(newTicks-oldTicks);
	//if (keys[SDLK_g])
		drawGLScene();
		if (keyMaster->isDown(key_p))
		{
			//printf("pos: %f\t%f\t%f\n",theCamera->getx(),theCamera->gety(),theCamera->getz());
			//printf("rot: %f\t%f\t%f\n",theCamera->getrotx(),theCamera->getroty(),theCamera->getrotz());
		}
		SDL_GL_SwapBuffers();

	}

	SDL_Quit();
}
