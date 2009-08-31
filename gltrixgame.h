/*
 * gltrixgame.h
 *
 *  Created on: Aug 8, 2009
 *      Author: mits
 */

#ifndef GLTRIXGAME_H_
#define GLTRIXGAME_H_

#include "defs.h"
#include "gltrixdraw.h"

enum gltrixKeys
{
	key_up = 0,
	key_down,
	key_left,
	key_right,
	key_p
};


class DynamicObj
{
public:
	virtual void update(uint32 ticksdiff) = 0;
};

class KeyMaster
{
public:
	KeyMaster();
	~KeyMaster();
	void setKey(gltrixKeys key, bool down, bool newkey);
	void setKeyDown(enum gltrixKeys key, bool down);
	bool isDown(gltrixKeys key);
	bool isNewKey(gltrixKeys key);

private:
	static const int maxKeys = 128;
	bool keysPressed[128];
	bool newPresses[128];
};

class Color
{
public:
	static const color_t white,red,green,blue,black;
};

class Board : public Renderable
{
public:
	Board(int xsize, int ysize);
	~Board();
	void fillRandom(int maxPerCol);
	void blockAt(int x, int y);
	void attachBlock(int x, int y, color_t color= Color::white);
	void render();
	void setLineCallBack(void (*f)());
private:
	typedef struct TAGblock_t
	{
		bool occupied;
		color_t color;
	} block_t;
	void (*lineCallBack)();
	int xSize;
	int ySize;
	block_t blocks[50][30];
};

class Brick : public Renderable, public DynamicObj
{
public:
	Brick(int type, Board *board, int posx, int posy, color_t color = Color::white);
	~Brick();
	void moveLeft();
	void moveRight();
	void moveDown();
	void rotate();
	void speedDown();
	void update(uint32 ticksdiff);
	void render();
private:
	Board *parentBoard;
	int type;
	static const int numOfTypes = 6;
	static const int maxSize = 4;
	static const bool types[numOfTypes][maxSize][maxSize];
	bool blocks[maxSize][maxSize];
	int posx;
	int posy;
	color_t color;
	uint32 ticksTillLastMove;
	uint32 ticksPerMove;
	int speed;
};

void addDynamic(DynamicObj *dyn);
void removeDynamic(DynamicObj *dyn);

void refreshGameState(uint32 ticksdiff);

#endif /* GLTRIXGAME_H_ */
