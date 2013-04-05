#ifndef DUNGEON_H
#define DUNGEON_H

const int dunXSize = 80;
const int dunYSize = 20;

enum Tiles{
	EMPTY,
	FLOOR,
	WALL,
	HALLWAY
};

class Dungeon {
	//int data[dunXSize][dunYSize];
	int **data;
	public:
	Dungeon();	
	~Dungeon();
	private:
	void placeRndRoom();
};

#endif
