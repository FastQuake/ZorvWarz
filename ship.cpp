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
	bool recheck = false;
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
				recheck = true;
				//j--;
			}
			//Check to make sure the rooms are actually on the grid
			else if(room2.xPos+room2.xSize > dunXSize ||
					room2.yPos+room2.ySize > dunYSize){
				roomList[j].rndRoom();
				//j--;
				recheck = true;
			}
			if(recheck){
				j--;
				recheck = false;
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

	//Make hallways
	for(int i=0;i<numOfRooms-1;i++){
		Room room1 = roomList[i];
		Room room2 = roomList[i+1];
		//Find room 1 middle (x,y) cord
		int room1x = room1.xPos+(room1.xSize/2);
		int room1y = room1.yPos+(room1.ySize/2);  
		//Find room 2 middle(x,y) cord
		int room2x = room2.xPos+(room2.xSize/2);
		int room2y = room2.yPos+(room2.ySize/2);
		//Draw the y part of the hallway
		while(room1y != room2y){
			if(room1y < room2y){
				room1y++;
			}else if(room1y > room2y){
				room1y--;
			}
			data[room1x][room1y] = FLOOR;
		}
		//Draw the x part of the hallway
		while(room1x != room2x){
			if(room1x < room2x){
				room1x++;
			}else if(room1x > room2x){
				room1x--;
			}
			data[room1x][room1y] = FLOOR;
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

bool Room::intersects(Room otherRoom){
	if(xPos+xSize < otherRoom.xPos) return false;
	if(xPos > otherRoom.xPos + otherRoom.xSize) return false;
	if(yPos + ySize < otherRoom.yPos) return false;
	if(yPos > otherRoom.yPos + otherRoom.ySize) return false;
	return true;
}
