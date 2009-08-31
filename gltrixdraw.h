/*
 * gltrixdraw.h
 *
 *  Created on: Aug 8, 2009
 *      Author: mits
 */

#ifndef GLTRIXDRAW_H_
#define GLTRIXDRAW_H_

#include "defs.h"

typedef struct TAGcolor_t
{
	uint8 r;
	uint8 g;
	uint8 b;
} color_t;

void setOffsetsFromSize(int x, int y);
void setDrawPosition(int x, int y);
void unsetDrawPosition(int x, int y);
void drawCube(float size, color_t color);

class Renderable
{
public:
	virtual void render() = 0;
};

class Camera
{
public:
	Camera();
	Camera(afloat x, afloat y, afloat z, afloat rotx, afloat roty, afloat rotz);
	//	~Camera();
	afloat getx();
	afloat gety();
	afloat getz();
	afloat getrotx();
	afloat getroty();
	afloat getrotz();
	void matrixMul44f(afloat *a, afloat *b, afloat *res);
	void setPosition(afloat x, afloat y, afloat z);
	void setRotation(afloat rotx, afloat roty, afloat rotz);
	void moveGlobal(afloat x, afloat y, afloat z);
	void moveLocal(afloat x, afloat y, afloat z);
	void rotateGlobal(afloat rad, afloat rx, afloat ry, afloat rz);
	void rotateRelX(afloat rotx);
	void rotateRelY(afloat roty);
	void rotateRelZ(afloat rotz);
	void rotateLocal(afloat rad, afloat rx, afloat ry, afloat rz);
	void updategl();
	void printMatrix(afloat *m);
	afloat matrix[16],mat[16];
private:
	afloat x, y, z;
	afloat rotx, roty, rotz;
	afloat aux[16],aux2[16];
};

extern Camera *theCamera;



#endif /* GLTRIXDRAW_H_ */
