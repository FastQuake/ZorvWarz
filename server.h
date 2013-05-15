#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <enet/enet.h>
#include "entity.h"
using namespace std;

//Client to server packet types
enum{
	csLogin,
	csMove,
	csAttack
};

//Server to client packet types
enum{
	scSpawn,
	scJoinack,
	scAttack,
	scMove,
	scMap,
	scDespawn,
	scChgLvl
};

extern ShipEntity *serverShip;

void serverLoop();
void initServer();
std::string getMapData(bool newlines);
void handlePacket(string packetData, ENetPeer *peer);
void loadLevel(bool ai);
void despawnLevel();
int p1Orp2(ENetPeer *peer);
ENetPacket *createPacket(int packetType, string packetData, int packetFlag);

extern Mob *p1;
extern Mob *p2;
extern ENetHost *server;
extern sf::Mutex pathMutex;

#endif
