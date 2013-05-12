#include <iostream>
#include <stdlib.h>
#include "ship.h"
#include "entity.h"
#include "main.h"

using namespace std;

Ship::Ship(string tileFile){
	//Load texture data
	tiles.loadFromFile(tileFile);
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
	numOfRooms = rand() % (maxRooms-minRooms+1) + minRooms;
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
			else if(room2.xPos+room2.xSize > dunXSize-1 ||
					room2.yPos+room2.ySize > dunYSize-1){
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

		int lastType = -1;
		int type;
		//Draw the y part of the hallway
		while(room1y != room2y){
			if(room1y < room2y){
				room1y++;
				type = 1;
			}else if(room1y > room2y){
				room1y--;
				type = -1;
			}
			if(data[room1x][room1y] == FLOOR && lastType == EMPTY){
				data[room1x][room1y] = NODE;
				lastType = NODE;
			} else if(data[room1x][room1y] == EMPTY && lastType == FLOOR){
				data[room1x][room1y-type] = NODE;
				data[room1x][room1y] = FLOOR;
				lastType = NODE;
			}else if(data[room1x][room1y] == EMPTY){
				data[room1x][room1y] = FLOOR;
				lastType = EMPTY;
			} else {
				lastType = FLOOR;
			}
		}
		data[room1x][room1y] = NODE;
		lastType = NODE;
		//Draw the x part of the hallway
		while(room1x != room2x){
			if(room1x < room2x){
				room1x++;
				type = 1;
			}else if(room1x > room2x){
				room1x--;
				type = -1;
			}
			if(data[room1x][room1y] == FLOOR && lastType == EMPTY){
				data[room1x][room1y] = NODE;
				lastType = NODE;
			} else if(data[room1x][room1y] == EMPTY && lastType == FLOOR){
				data[room1x-type][room1y] = NODE;
				data[room1x][room1y] = FLOOR;
				lastType = NODE;
			}else if(data[room1x][room1y] == EMPTY){
				data[room1x][room1y] = FLOOR;
				lastType = EMPTY;
			} else {
				lastType = FLOOR;
			}
		}
	}

	//Make walls
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			if(data[x][y] == FLOOR || data[x][y] == NODE){
				//Left
				if(data[x-1][y] == EMPTY){
					data[x-1][y] = LWALL;
				}
				//Right
				if(data[x+1][y] == EMPTY){
					data[x+1][y] = RWALL;
				}
				//Up
				if(data[x][y-1] == EMPTY){
					data[x][y-1] = TWALL;
				}
				//Down
				if(data[x][y+1] == EMPTY){
					data[x][y+1] = WALL;
				}
				/*//Left-Down
				if(data[x-1][y-1] == EMPTY){
					data[x-1][y-1] = LTCORNWALL;
				}
				//Left-Up
				else if(data[x-1][y+1] == EMPTY){
					data[x-1][y+1] = LBCORNWALL;
				}
				//Right-Down
				else if(data[x+1][y-1] == EMPTY){
					data[x+1][y-1] = RTCORNWALL;
				}
				//Right-Up
				else if(data[x+1][y+1] == EMPTY){
					data[x+1][y+1] = RBCORNWALL;
				}*/
			}
		}
	}
	//Make walls
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			if(data[x][y] == FLOOR || data[x][y] == NODE){
				//Left-Down
				if(data[x-1][y-1] == EMPTY){
					data[x-1][y-1] = LTCORNWALL;
				}
				//Left-Up
				else if(data[x-1][y+1] == EMPTY){
					data[x-1][y+1] = LBCORNWALL;
				}
				//Right-Down
				else if(data[x+1][y-1] == EMPTY){
					data[x+1][y-1] = RTCORNWALL;
				}
				//Right-Up
				else if(data[x+1][y+1] == EMPTY){
					data[x+1][y+1] = RBCORNWALL;
				}
			}
		}
	}

	//Try to make 100 random bloody tiles
	for(int i=0;i<500;i++){
		sf::Vector2i pos;
		pos.x = rand()%dunXSize;
		pos.y = rand()%dunYSize;
		if(data[pos.x][pos.y] % 2 != 0 && data[pos.x][pos.y] != NODE && data[pos.x][pos.y] != WALL){
			data[pos.x][pos.y]++;
		}
	}

	/*
	//debug room
	data[0][0] = WALL;*/

	//drawRoom();
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

void Ship::drawMap(sf::RenderWindow *window,int screenx, int screeny){
	sf::Sprite mapTile;
	mapTile.setTexture(tiles);
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			 if(data[x][y] == 0){
				continue;
			 } else if((x*32+32) >= (screenx) && (x*32) <= (screenx+800)&&
					 (y*32+32) >= (screeny) && (y*32) <= (screeny+600)){
				 int xx = (data[x][y] % 4) * 32;
				 int yy = (data[x][y] / 4) * 32; 
				 if(data[x][y] == NODE){
					 xx = 32;
					 yy = 0;
				 }
				 mapTile.setTextureRect(sf::IntRect(xx,yy,32,32));
				 mapTile.setPosition(x*32,y*32);
				 mapTile.move(-screenx,-screeny);
				 window->draw(mapTile);
			 }
		}
	}
}

Room::Room(){
	rndRoom();
}

void Room::rndRoom(){
	//Set random (x,y) position
	xPos = rand() % (dunXSize-1) + 1;
	yPos = rand() % (dunYSize-1) + 1;

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

ShipEntity::ShipEntity(string tilesFile){
	type = "map";
	drawable = true;
	collides = true;
	readyToUpdate = false;
	alive = true;

	x = 0;
	y = 0;

	map = new Ship(tilesFile);

	getColBoxes();
}

ShipEntity::~ShipEntity(){
	delete map;
}

void ShipEntity::getColBoxes(){
	collisionBoxes.clear();
	//Get all collision boxes
	for(int x=0;x<dunXSize;x++){
		for(int y=0;y<dunYSize;y++){
			if(map->data[x][y] > 2 && map->data[x][y] < 20){
				//cout << "pushing back collisionBoxes " << x << "," << y << endl;
				collisionBoxes.push_back(sf::FloatRect(x*32,y*32,32,32));
			}
		}
	}
}

void ShipEntity::draw(sf::RenderWindow *screen, int screenx, int screeny){
	map->drawMap(screen,screenx,screeny);
}

sf::Vector2i ShipEntity::getRandomFloorTile(){
	sf::Vector2i vec;
	int x;
	int y;
	while(true){
		x = rand() % dunXSize;
		y = rand() % dunYSize;
		if(map->data[x][y] == FLOOR){
			break;
		}
	}
	vec.x = x;
	vec.y = y;
	return vec;
}
