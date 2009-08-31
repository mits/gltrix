/*
 * glamourlibgl.h
 *
 *  Created on: Aug 4, 2009
 *      Author: mits
 */

#ifndef GLAMOURLIBGL_H_
#define GLAMOURLIBGL_H_

#include "GL/gl.h"
#include "gltrixgame.h"

typedef struct TAGtexBitmap
{
	GLuint width;
	GLuint height;
	GLuint bpp;
	GLuint pitch;
	GLubyte *data;

} texBitmap_t;

void addRenderable(Renderable *ren);
void removeRenderable(Renderable *ren);


void reshape(int w, int h);
int initGL();
int loadGLTextures();
void buildFont();
void printgl(int x,int y,char *string);

int drawGLScene();
void toggleLighting();
void changeFilter();
void toggleBlending();

extern bool lighting;
extern bool blending;
extern bool twinkle;


extern GLfloat zoom;
extern GLfloat tilt;
extern GLfloat spin;
extern GLuint loop;


#endif /* GLAMOURLIBGL_H_ */
