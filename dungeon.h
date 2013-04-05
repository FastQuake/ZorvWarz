#ifndef DUNGEON_H
#define DUNGEON_H

const int dunXSize = 10;
const int dunYSize = 10;

enum Tiles{
	EMPTY,
	FLOOR,
	WALL,
	HALLWAY
};

class Dungeon {
	int data[dunXSize][dunYSize];
	public:
	Dungeon();	
};

#endif
