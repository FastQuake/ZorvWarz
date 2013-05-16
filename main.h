#ifndef MAIN_H
#define MAIN_H
#include <iostream>
#include <vector>
#include "entity.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#define PI 3.14159265

extern Player *player;

const std::string tilesFile = "data/textures/tiles.png";
const std::string playerFile = "data/textures/player.png";
const std::string alienFile = "data/textures/alien.png";
const std::string lightFile = "data/textures/light.png";
const std::string bulletFile = "data/textures/bullet.png";

extern sf::Texture pTex;
extern sf::Texture tilesTex;
extern sf::Texture aTex;
extern sf::Texture lTex;
extern sf::Texture bTex;
extern sf::Texture sTex;
extern sf::Texture gTex;
extern sf::Texture ammoTex;
extern sf::Texture hTex;

extern sf::Texture bTex;

//extern sf::RenderWindow window;
extern sf::Clock frameTime;
//extern sf::Time dt;

extern sf::Thread *serverThread;
extern sf::Thread *clientThread;
extern sf::Mutex readyMutex;
extern bool ready;
extern bool serverReady;
extern bool twoPlayers;
extern bool connecting;
extern bool singleplayer;
extern bool loading;
extern bool inputIP;
extern sf::Mutex packetMutex;
extern sf::Clock p2Timer;

extern std::string IPad;
extern int FPS;
extern int state;
extern int seed;
extern int level;

extern bool doShutdown;
extern bool dead;

extern sf::Vector2i mousePos;

extern bool keyUp;
extern bool keyDown;
extern bool keyLeft;
extern bool keyRight;
extern bool mouseRight;
extern bool connectedOnce;

extern EntityManager entities;
extern ShipEntity *ship;

extern std::vector<std::string> packetList;

extern bool inputIP;
extern std::string ipText;

extern sf::Font font;
extern sf::Sound bulletSound;
extern sf::Sound clickSound;
extern sf::Sound pickupSound;
extern sf::Sound oofSound;
extern bool gameStateDone;

void runClient(std::string selection);
std::string intToStr(int num);
std::vector<sf::FloatRect> whatIntersectsBox(sf::FloatRect hitBox);
void killAll();

#endif
