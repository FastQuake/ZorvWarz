#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <vector>
#include "entity.h"

extern Player *player;

const std::string tilesFile = "data/textures/tiles.png";
const std::string playerFile = "data/textures/player.png";
const std::string lightFile = "data/textures/light.png";

extern sf::RenderWindow window;
extern bool doShutdown;

extern bool keyUp;
extern bool keyDown;
extern bool keyLeft;
extern bool keyRight;

extern ShipEntity *ship;

extern std::vector<std::string> packetList;

void runClient(std::string selection);
std::string intToStr(int num);

#endif
