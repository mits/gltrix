/*
 * glamourlibsdl.h
 *
 *  Created on: Aug 4, 2009
 *      Author: mits
 */

#ifndef GLAMOURLIBSDL_H_
#define GLAMOURLIBSDL_H_

#include "gltrixgl.h"
#include "SDL/SDL.h"
#include "gltrixsdl2gl.h"
#include "gltrixgame.h"

extern SDL_Surface *screen;
extern int done;
extern Uint8 *keys;



void processSDLEvents();
void processSDLKeystates(KeyMaster *km);

#endif /* GLAMOURLIBSDL_H_ */
