#ifndef SHIP_H
#define SHIP_H
#include <SFML/Graphics.hpp>

const int dunXSize = 80;
const int dunYSize = 80;
const int minRooms = 7;
const int maxRooms = 20;

enum Tiles{
	EMPTY,
	FLOOR,
	BFLOOR,
	TWALLW,
	BTWALLW,
	TWALL,
	BTWALL,
	LWALL,
	BLWALL,
	RWALL,
	BRWALL,
	RBCORNWALL,
	BRBCORNWALL,
	LBCORNWALL,
	BLBCORNWALL,
	RTCORNWALL,
	BRTCORNWALL,
	LTCORNWALL,
	BLTCORNWALL,
	WALL,
	NODE
};

class Room{
	public:
	int xPos,yPos;
	int xSize,ySize;
	Room();
	void rndRoom();
	bool intersects(Room otherRoom);
};

class Ship{
	int numOfRooms;
	Room *roomList;
	public:
	int **data;
	Ship(bool serv);
	~Ship();
	void drawRoom();
	void drawMap(sf::RenderWindow *window,int screenx, int screeny);
};

#endif
