#ifndef ENDGAME_H
#define ENDGAME_H

#include <SFML\Graphics.hpp>
#include "server.h"

void initEndScreen();
void updateEndScreen(Stats *endStats);
void drawEndScreen(sf::RenderWindow *screen);

#endif