#include <iostream>
#include <stdlib.h>
#include "dungeon.h"
using namespace std;

Dungeon::Dungeon(){
	/* Allocate memory for map */
	data = new int*[dunXSize];
	for(int i=0;i<dunXSize;i++){
		data[i] = new int[dunYSize];
	}

	/* Set contents of map to zero just incase */
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			data[x][y] = EMPTY;
		}
	}

	/* Place first room on dungeon */
	placeRndRoom();
	placeRndHallway();

	/* Print out map data for debugging */
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			cout << data[x][y];
		}
		cout << endl;
	}
}

Dungeon::~Dungeon(){
	for(int i=0;i<dunXSize;i++){
		delete[] data[i];
	}
	delete[] data;
}

void Dungeon::placeRndRoom(){
	/* Get data of room */
	int xSize = rand() % 7 + 4; //Make room with min x size 4 max 10
	int ySize = rand() % 7 + 4; //Set min y size 4 max 10
	int xCord = rand() % dunXSize; //Get (X,Y) coords of room
	int yCord = rand() % dunYSize;

	while(!placeArea(xCord,yCord,xSize,ySize,FLOOR)){
		xCord = rand() % dunXSize; //Get (X,Y) coords of room
		yCord = rand() % dunYSize;	
	}
}

bool Dungeon::placeArea(int xCord, int yCord, int xSize, int ySize, int floor){
	/* Make sure room fits on map */
	while(true){
		if(xCord + xSize > dunXSize){
			//Can't place room
			return false;
		} else if(yCord + ySize > dunYSize){
			//Also can't place room
			return false;
		} else break;
	}		

	/* Place room onto map */
	for(int y=yCord;y<yCord+ySize;y++){
		for(int x=xCord;x<xCord+xSize;x++){
			if(y == yCord || y == yCord+ySize-1){
				data[x][y] = WALL;
			} else if(x == xCord || x == xCord+xSize-1){
				data[x][y] = WALL;
			} else {
				data[x][y] = floor;
			}
		}
	}	
	return true;
}

void Dungeon::placeRndHallway(){
	int xCord;
	int yCord;
	int xSize;
	int ySize;
	char dir;
	/* get postion */
	while(true){
		xCord = rand() % dunXSize;
		yCord = rand() % dunYSize;

		if(xCord == 0 || xCord >= dunXSize-1){
			continue;
		} else if(yCord == 0 || yCord >= dunYSize-1){
			continue;
		} else if((data[xCord-1][yCord] == WALL || data[xCord+1][yCord] == WALL) &&
				(data[xCord][yCord-1] == WALL || data[xCord][yCord+1] == WALL)){
			continue;
		} else break;
	}

	if(data[xCord-1][yCord] == EMPTY){
		dir = 'L';
		xSize = rand() % 4+4;
		ySize = 3;
		while(!placeArea(xCord-xSize,yCord-1,xSize,ySize,HALLWAY)){
			xSize = rand() % 4+4;
		}
	}else if(data[xCord+1][yCord] == EMPTY){
		dir = 'R';
		xSize = rand() % 4+4;
		ySize = 3;
		while(!placeArea(xCord,yCord-1,xSize,ySize,HALLWAY)){
			xSize = rand() % 4+4;
		}
	}else if(data[xCord][yCord-1] == EMPTY){
		dir = 'U';
		ySize = rand() % 4+4;
		xSize = 3;
		while(!placeArea(xCord-1,yCord-ySize,xSize,ySize,HALLWAY)){
			ySize = rand() % 4+4;
		}
	}else if(data[xCord][yCord+1] == EMPTY){
		dir = 'D';
		ySize = rand() % 4+4;
		xSize = 3;
		while(!placeArea(xCord-1,yCord,xSize,ySize,HALLWAY)){
			ySize = rand() % 4+4;
		}
	}

}
