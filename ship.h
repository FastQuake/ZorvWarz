#ifndef SHIP_H
#define SHIP_H
#include <SFML/Graphics.hpp>
#include <vector>

const int dunXSize = 80;
const int dunYSize = 20;

enum Tiles{
	EMPTY,
	FLOOR,
	WALL,
	DOOR
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
	int **data;
	int numOfRooms;
	Room *roomList;
	public:
	Ship();
	~Ship();
	void drawRoom();
};

#endif
