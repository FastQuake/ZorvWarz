#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include "entity.h"

enum{
	csLogin,
	csMove,
	csAttack
};

enum{
	scSpawn,
	scJoinack,
	scAttack,
	scMove,
	scMap
};

extern ShipEntity *serverShip;
void serverLoop();
void initServer();
std::string getMapData();

#endif
