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
	csAttack,
	csTookHealth,
	csRequestEnd
};

//Server to client packet types
enum{
	scSpawn,
	scJoinack,
	scAttack,
	scMove,
	scMap,
	scDespawn,
	scChgLvl,
	scStats,
	scP2Dsc
};

struct Stats{
	int p1Score;
	int p1Kills;
	int p1HealthUsed;
	int p1ShotsFired;

	int p2Score;
	int p2Kills;
	int p2HealthUsed;
	int p2ShotsFired;
};

extern ShipEntity *serverShip;

void serverLoop();
void initServer();
std::string getMapData(bool newlines);
void handlePacket(string packetData, ENetPeer *peer);
void loadLevel(bool ai);
void despawnLevel();
int p1Orp2(ENetPeer *peer);
void sendSpawnPackets(ENetPeer *peer);
ENetPacket *createPacket(int packetType, string packetData, int packetFlag);

extern Mob *p1;
extern Mob *p2;
extern ENetHost *server;
extern sf::Mutex pathMutex;
extern Stats stats;

#endif
