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
	int numOfRooms;
	Room *roomList;
	sf::Texture tiles;
	public:
	int **data;
	Ship(std::string tileFile);
	~Ship();
	void drawRoom();
	void drawMap(sf::RenderWindow *window);
};

#endif
