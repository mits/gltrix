/*
 * glamourlibgl.cpp
 *
 *  Created on: Aug 4, 2009
 *      Author: mits
 */

#include "GL/gl.h"
#include "GL/glu.h"
#include "gltrixgl.h"
#include "gltrixsdl2gl.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "defs.h"
using namespace std;

GLuint textures[3];
bool lighting;
bool blending;
bool twinkle;
GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
GLuint filter;
GLuint base,i;
afloat offx,offy,offz;

typedef struct TAGrenable_t
{
	Renderable *renable;
	struct TAGrenable_t *next;
} renable_t;

renable_t *headRen,*tailRen;

void reshape(int w, int h)
{
	glViewport(0, 0, (GLint) w, (GLint) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)w/(GLfloat)h,0.1f,1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int initGL()
{
	if (!loadGLTextures())
	{
		fprintf(stderr,"loadGLTextures() failed\n");
		return -1;
	}
	buildFont();
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT1);
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glDisable(GL_BLEND);
	blending = false;
	return true;
}

void printgl(int x, int y, char *string)
{
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,640,0,480,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(x,y,0);
	glListBase(base-32);
	glCallLists(strlen(string),GL_UNSIGNED_BYTE,string);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

int loadGLTextures()
{
	int status = false;
	texBitmap_t *image1,*image2;
	image1 = NULL;
	image1 = loadBMP("Star.bmp");
	image2 = NULL;
	image2 = loadBMP("Font.bmp");
	textures[0] = NULL;
	textures[1] = NULL;
	textures[2] = NULL;

	if (image1)
	{
		status = true;
		glGenTextures(3,&textures[0]);
		glBindTexture(GL_TEXTURE_2D,textures[0]);
		glTexImage2D(GL_TEXTURE_2D,0,3,image1->width,image1->height,0,GL_RGB,GL_UNSIGNED_BYTE,image1->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D,textures[1]);
		glTexImage2D(GL_TEXTURE_2D,0,3,image1->width,image1->height,0,GL_RGB,GL_UNSIGNED_BYTE,image1->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D,textures[2]);
		glTexImage2D(GL_TEXTURE_2D,0,3,image2->width,image2->height,0,GL_RGB,GL_UNSIGNED_BYTE,image2->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
//		glBindTexture(GL_TEXTURE_2D,textures[2]);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_NEAREST);
//		gluBuild2DMipmaps(GL_TEXTURE_2D,3,image1->width,image1->height,GL_RGB,GL_UNSIGNED_BYTE,image1->data);

		free(image1);
		free(image2);
		return 1;
	}
	else return 0;
}

void buildFont()
{
	afloat cx,cy;
	base = glGenLists(256);
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	for(i=0;i<256;i++)
	{
		cx = (float)(i%16)/16.0f;
		cy = (float)(i/16)/16.0f;
		glNewList(base+i,GL_COMPILE);
			glBegin(GL_QUADS);
				glTexCoord2f(cx,1-cy-0.0625f);
				glVertex2i(0,0);
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);
				glVertex2i(16,0);
				glTexCoord2f(cx+0.0625f,1-cy);
				glVertex2i(16,16);
				glTexCoord2f(cx,1-cy);
				glVertex2i(0,16);
			glEnd();
			glTranslated(16,0,0);
		glEndList();
	}
}


int drawGLScene()
{
	char string[50];
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//	sprintf(string,"pos: %f %f %f",theCamera->getx(),theCamera->gety(),theCamera->getz());
//	printgl(0,100,string);
//	sprintf(string,"rot: %f %f %f",theCamera->getrotx(),theCamera->getroty(),theCamera->getrotz());
//	printgl(0,50,string);
	sprintf(string,"%f %f %f %f",theCamera->mat[0],theCamera->mat[1],theCamera->mat[2],theCamera->mat[3]);
	printgl(0,200,string);
	sprintf(string,"%f %f %f %f",theCamera->mat[4],theCamera->mat[5],theCamera->mat[6],theCamera->mat[7]);
	printgl(0,150,string);
	sprintf(string,"%f %f %f %f",theCamera->mat[8],theCamera->mat[9],theCamera->mat[10],theCamera->mat[11]);
	printgl(0,100,string);
	sprintf(string,"%f %f %f %f",theCamera->mat[12],theCamera->mat[13],theCamera->mat[14],theCamera->mat[15]);
	printgl(0,50,string);

	glBindTexture(GL_TEXTURE_2D,textures[filter]);

	glLoadIdentity();
//	cout<<"drawglscene"<<endl;
	renable_t *current = headRen;
	theCamera->updategl();
	while (current!=NULL)
	{
		glPushMatrix();
		current->renable->render();
		glPopMatrix();
		current = current->next;
	}

	return true;
}

void toggleLighting()
{
	lighting = !lighting;
	if (lighting) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);
}

void changeFilter()
{
	filter = (filter+1) %3;
}

void toggleBlending()
{
	blending = !blending;
	if (blending)
	{
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

void drawCube(GLfloat size, color_t color)
{
	glColor4ub(color.r, color.g, color.b, 255);
	glBegin(GL_QUADS);
	//front
		glNormal3f(0.0f,0.0f,1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f*size, -1.0f*size, 1.0f*size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f*size, -1.0f*size, 1.0f*size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f*size, 1.0f*size, 1.0f*size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f*size, 1.0f*size, 1.0f*size);
	//back
		glNormal3f(0.0f,0.0f,-1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1.0f*size, -1.0f*size, -1.0f*size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f*size, 1.0f*size, -1.0f*size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-1.0f*size, 1.0f*size, -1.0f*size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size);
		//left
		glNormal3f(-1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f*size, 1.0f*size, -1.0f*size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-1.0f*size, 1.0f*size, 1.0f*size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-1.0f*size, -1.0f*size, 1.0f*size);
		//right
		glNormal3f(1.0f,0.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(1.0f*size, -1.0f*size, 1.0f*size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(1.0f*size, 1.0f*size, 1.0f*size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f*size, 1.0f*size, -1.0f*size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1.0f*size, -1.0f*size, -1.0f*size);
		//top
		glNormal3f(0.0f,1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f*size, 1.0f*size, 1.0f*size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f*size, 1.0f*size, -1.0f*size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f*size, 1.0f*size, -1.0f*size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1.0f*size, 1.0f*size, 1.0f*size);
		//bottom
		glNormal3f(0.0f,-1.0f,0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-1.0f*size, -1.0f*size, -1.0f*size);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-1.0f*size, -1.0f*size, 1.0f*size);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(1.0f*size, -1.0f*size, 1.0f*size);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(1.0f*size, -1.0f*size, -1.0f*size);
	glEnd();

}

void setOffsetsFromSize(int x, int y)
{
	offz=-(float)y/4;
	offx=offz*x/3;
	offy=offz*y/3;
	printf("%d %d %d\n",offx,offy,offz);
}

void setDrawPosition(int x, int y)
{
	//glLoadIdentity();
	glTranslatef(offx+2.0f*x,offy+2.0f*y,offz);
}

void unsetDrawPosition(int x, int y)
{
	//glLoadIdentity();
	glTranslatef(-offx-2.0f*x,-offy-2.0f*y,-offz);
}

void addRenderable(Renderable *ren)
{
	if (tailRen!=NULL)
	{
		tailRen->next = (renable_t *)malloc(sizeof(renable_t));
		tailRen = tailRen->next;
		tailRen->renable = ren;
		tailRen->next = NULL;
	}
	else
	{
		headRen = tailRen = (renable_t *)malloc(sizeof(renable_t));
		tailRen->next = NULL;
		tailRen->renable = ren;
	}
}

void removeRenderable(Renderable *ren)
{
	if (headRen!=NULL)
	{
		renable_t *temp = headRen;
		if (ren==headRen->renable)
		{
			headRen = headRen->next;
			free(temp);
		}
		else
		{
			while(temp->renable!=ren&&temp!=NULL)
			{
				temp = temp->next;
			}
			if (temp->renable==ren)
			{

			}
		}
	}
}
