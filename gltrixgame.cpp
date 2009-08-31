/*
 * gltrixgame.cpp
 *
 *  Created on: Aug 8, 2009
 *      Author: mits
 */
#include "gltrixgame.h"
#include "gltrixdraw.h"
#include <iostream>
using namespace std;

#include <cstdlib>

typedef struct TAGdynamic_t
{
	DynamicObj *dynamic;
	struct TAGdynamic_t *next;
} dynamic_t;

dynamic_t *headDyn,*tailDyn;


KeyMaster::KeyMaster()
{
	for (int i=0;i<maxKeys;i++)
	{
		keysPressed[i]=newPresses[i] = false;
	}
}

void KeyMaster::setKey(enum gltrixKeys key, bool down, bool newkey)
{
	keysPressed[key] = down;
	newPresses[key] = newkey;
}

void KeyMaster::setKeyDown(enum gltrixKeys key, bool down)
{
	if (keysPressed[key]&&down)
		newPresses[key] = false;
	else
	{
		keysPressed[key] = true;
		newPresses[key] = true;
	}
}

bool KeyMaster::isDown(enum gltrixKeys key)
{
	return keysPressed[key];
}

bool KeyMaster::isNewKey(enum gltrixKeys key)
{
	return newPresses[key];
}

const color_t Color::white = {255, 255, 255};
const color_t Color::red = {255, 0, 0};
const color_t Color::green = {0, 255, 0};
const color_t Color::blue = {0, 0, 255};
const color_t Color::black = {0, 0, 0};

Board::Board(int xsize, int ysize)
{
	xSize = xsize;
	ySize = ysize;
	for (int x;x<xSize;x++)
	{
		for (int y;y<ySize;y++)
		{
			blocks[x][y].occupied = false;
		}
	}
	blocks[1][4].occupied = true;
	blocks[1][4].color = Color::red;
	blocks[4][4].occupied = true;
	blocks[4][4].color = Color::green;
	blocks[3][4].occupied = true;
	blocks[3][4].color = Color::blue;
	blocks[2][4].occupied = true;
	blocks[2][4].color = Color::white;
	cout<<"init?"<<endl;
}

void Board::fillRandom(int maxPerCol)
{

}

void Board::blockAt(int x, int y)
{

}

void Board::attachBlock(int x, int y, color_t color)
{

}

void Board::render()
{
	for (int x=0;x<xSize;x++)
	{
		for (int y=0;y<ySize;y++)
		{
			if (blocks[x][y].occupied)
			{
				cout<<"skata"<<endl;
				setDrawPosition(x,y);
				drawCube(1,blocks[x][y].color);
				unsetDrawPosition(x,y);
			}
		}
	}
}

void Board::setLineCallBack(void (*f)())
{
	lineCallBack = f;
}

const bool Brick::types[numOfTypes][maxSize][maxSize]
     = {{{0,0,0,0},{0,0,0,0},{1,1,1,1},{0,0,0,0}},
		{{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
		{{0,1,0,0},{0,1,1,0},{0,0,1,0},{0,0,0,0}},
		{{0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0}},
		{{0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0}},
		{{0,0,1,0},{0,0,1,0},{0,1,1,0},{0,0,0,0}}};

Brick::Brick(int type, Board *board,int posx, int posy, color_t color)
{
	parentBoard = board;
	this->type = type;
	this->color = color;
	this->posx = posx;
	this->posy = posy;
	for (int i=0;i<maxSize;i++)
		for (int j=0;j<maxSize;j++)
				blocks[i][j] = types[type][i][j];
	ticksPerMove = 1000;
}

Brick::~Brick() { }

void Brick::moveLeft()
{

}

void Brick::moveRight()
{

}

void Brick::moveDown()
{

}

void Brick::rotate()
{

}

void Brick::speedDown()
{

}

void Brick::render()
{
	for (int x=0;x<maxSize;x++)
	{
		for (int y=0;y<maxSize;y++)
		{
			if (blocks[x][y])
			{
				setDrawPosition(posx+x-1,posy+y-1);
				drawCube(1,color);
				unsetDrawPosition(posx+x-1,posy+y-1);
				}
		}
	}

}

void Brick::update(uint32 ticksdiff)
{
//	printf("update!!\n");
//	printf("tickstilllastmove: %d, ticksdiff %d\n",ticksTillLastMove,ticksdiff);
	ticksTillLastMove += ticksdiff;
	if (ticksTillLastMove>=ticksPerMove)
	{
		posy--;
		ticksTillLastMove = ticksTillLastMove - ticksPerMove;
	}
}


void refreshGameState(uint32 ticksdiff)
{
	dynamic_t *current = headDyn;
	while(current!=NULL)
	{
		current->dynamic->update(ticksdiff);
		current = current->next;
	}
}


void addDynamic(DynamicObj *dyn)
{
	if (tailDyn!=NULL)
	{
		tailDyn->next = (dynamic_t *)malloc(sizeof(dynamic_t));
		tailDyn = tailDyn->next;
		tailDyn->dynamic = dyn;
		tailDyn->next = NULL;
	}
	else
	{
		headDyn = tailDyn = (dynamic_t *)malloc(sizeof(dynamic_t));
		tailDyn->next = NULL;
		tailDyn->dynamic = dyn;
	}
}

void removeDynamic(DynamicObj *dyn)
{
	if (headDyn!=NULL)
	{
		dynamic_t *temp = headDyn;
		if (dyn==headDyn->dynamic)
		{
			headDyn = headDyn->next;
			free(temp);
		}
		else
		{
			while(temp->dynamic!=dyn&&temp!=NULL)
			{
				temp = temp->next;
			}
			if (temp->dynamic==dyn)
			{

			}
		}
	}
}
