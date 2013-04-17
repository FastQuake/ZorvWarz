#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include "entity.h"

extern ShipEntity *serverShip;
void runServer();
std::string getMapData();

#endif