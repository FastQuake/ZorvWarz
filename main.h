#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <vector>
#include "entity.h"
#include <SFML/Graphics.hpp>

#define PI 3.14159265

extern Player *player;

const std::string tilesFile = "data/textures/tiles.png";
const std::string playerFile = "data/textures/player.png";
const std::string alienFile = "data/textures/alien.png";
const std::string lightFile = "data/textures/light.png";
const std::string bulletFile = "data/textures/bullet.png";

extern sf::Texture bTex;

extern sf::RenderWindow window;
extern sf::Clock frameTime;
extern sf::Time dt;

extern sf::Thread *serverThread;
extern sf::Thread *clientThread;
//extern sf::Mutex readyMutex;
extern bool ready;
extern bool serverReady;
extern sf::Mutex packetMutex;

extern std::string IPad;
extern int FPS;
extern int state;

extern bool doShutdown;

extern sf::Vector2i mousePos;

extern bool keyUp;
extern bool keyDown;
extern bool keyLeft;
extern bool keyRight;
extern bool mouseRight;

extern EntityManager entities;
extern ShipEntity *ship;

extern std::vector<std::string> packetList;

extern bool inputIP;
extern std::string ipText;

extern sf::Font font;

void runClient(std::string selection);
std::string intToStr(int num);
std::vector<sf::FloatRect> whatIntersectsBox(sf::FloatRect hitBox);

#endif
