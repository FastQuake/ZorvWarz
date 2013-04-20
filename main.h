#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <vector>
#include "entity.h"

extern Player *player;

const std::string tilesFile = "data/textures/tiles.png";
const std::string playerFile = "data/textures/player.png";

extern sf::RenderWindow window;
extern bool doShutdown;

extern std::vector<std::string> packetList;

void runClient(std::string selection);

#endif
