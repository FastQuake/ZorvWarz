#include <iostream>
#include <stdlib.h>
#include "ship.h"
using namespace std;

Ship::Ship(){
	//Make data array
	data = new int*[dunXSize];
	for(int i=0;i<dunXSize;i++){
		data[i] = new int[dunYSize];
	}
	//Set array to zero cause just in case
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			data[x][y] = EMPTY;
		}
	}

	//How many rooms to make
	numOfRooms = rand() % 5 + 6;
	roomList = new Room[numOfRooms];

	//Make sure no rooms are inside eachother
	for(int i=0;i<numOfRooms;i++){
		for(int j=0;j<numOfRooms;j++){
			//Cause problems if we don't temp store them
			Room room1 = roomList[i];
			Room room2 = roomList[j];
			if(j == i){
				continue;
			}
			//Check if the rooms are inside eachother
			if(room1.intersects(room2)){
				roomList[j].rndRoom();
				j--;
			}
			//Check to make sure the rooms are actually on the grid
			if(room2.xPos+room2.xSize > dunXSize ||
					room2.yPos+room2.ySize > dunYSize){
				roomList[j].rndRoom();
				j--;
			}
		}
	}

	//Place rooms on map
	for(int i=0;i<numOfRooms;i++){
		for(int y=0;y<roomList[i].ySize;y++){
			for(int x=0;x<roomList[i].xSize;x++){
				data[x+roomList[i].xPos][y+roomList[i].yPos] = FLOOR;
			}
		}
	}

	drawRoom();
}

Ship::~Ship(){
	for(int i=0;i<dunXSize;i++){
		delete[] data[i];
	}
	delete[] data;
	delete[] roomList;
}

void Ship::drawRoom(){
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			cout << data[x][y];
		}
		cout << endl;
	}
}

Room::Room(){
	rndRoom();
}

void Room::rndRoom(){
	//Set random (x,y) position
	xPos = rand() % dunXSize;
	yPos = rand() % dunYSize;

	//Make ship have mix size 4x4 and max 10x10
	xSize = rand() % 7 + 4;
	ySize = rand() % 7 + 4;
}

bool Room::intersects(Room &otherRoom){
	if(xPos+xSize < otherRoom.xPos) return false;
	if(xPos > otherRoom.xPos + otherRoom.xSize) return false;
	if(yPos + ySize < otherRoom.yPos) return false;
	if(yPos > otherRoom.yPos + otherRoom.ySize) return false;
	return true;
}
