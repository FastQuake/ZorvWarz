#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include "entity.h"

extern ShipEntity *serverShip;
void serverLoop();
void initServer();
std::string getMapData();

#endif