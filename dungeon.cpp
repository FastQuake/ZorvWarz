#include <iostream>
#include <stdlib.h>
#include "dungeon.h"
using namespace std;

Dungeon::Dungeon(){
	/* Place first room */
	int xSize = rand() % 7 + 4; //Make room with min x size 4 max 10
	int ySize = rand() % 7 + 4; //Set min y size 4 max 10
	int xCord = rand() % dunXSize; //Get (X,Y) coords of room
	int yCord = rand() % dunYSize;

	/* Make sure room fits on map */
	while(true){
		if(xCord + xSize > dunXSize){
			//Can't place room
			xCord = rand() % dunXSize;
		} else if(yCord + ySize > dunYSize){
			//Also can't place room
			yCord = rand() % dunYSize;
		} else break;
	}

	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			data[x][y] = 0;
		}
	}

	/* Place room onto map */
	for(int y=yCord;y<yCord+ySize;y++){
		for(int x=xCord;x<xCord+xSize;x++){
			if(y == yCord || y == yCord+ySize-1){
				data[x][y] = WALL;
			} else if(x == xCord || x == xCord+xSize-1){
				data[x][y] = WALL;
			} else {
				data[x][y] = FLOOR;
			}
		}
	}

	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			cout << data[x][y] << " ";
		}
		cout << endl;
	}
}
