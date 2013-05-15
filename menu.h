#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SFML/Graphics.hpp> 
#include "main.h"
#include "server.h"
using namespace std;

void initMenu();
void updateMenu();
void drawMenu(sf::RenderWindow *screen);

extern sf::Texture bgTexture;

#endif

