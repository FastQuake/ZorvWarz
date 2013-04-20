#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include "entity.h"

extern Player *player;

const std::string tilesFile = "data/textures/tiles.png";
const std::string playerFile = "data/textures/player.png";
void runClient(std::string selection);

extern sf::RenderWindow window;
extern bool doShutdown;

#endif
