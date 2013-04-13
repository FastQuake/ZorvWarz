#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include "ship.h"
#include "main.h"
using namespace std;

EntityManager entities;

//Ship bleh(tilesFile);
ShipEntity ship(tilesFile);
Player player(playerFile);

string intToStr(int num){
	stringstream ss;
	ss << num;
	return ss.str();
} 

void addEntities(){
	entities.entityList.push_back(&ship);
	entities.entityList.push_back(&player);
}

void setup(){
	//Set seed based on time
	int seed = time(NULL);
	cout << "seed: " << seed << endl;
	srand(seed);

	addEntities();
}

int main(int argc, char *argv[]){
	sf::RenderWindow window;
	window.create(sf::VideoMode(800,600),"Test");
	window.setFramerateLimit(60);

	setup();

	int x = window.getSize().x/2;
	int y = window.getSize().y/2;
	
	int fps = 0;
	sf::Clock counter;
	counter.restart();

	sf::Font font;
	font.loadFromFile("data/PressStart2P.ttf");

	sf::Text fpsText;
	fpsText.setFont(font);

	while(window.isOpen()){
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			} 
		}

		entities.updateEntities(0);

		window.clear();
		//bleh.drawMap(&window);
		entities.drawEntities(&window,player.x,player.y);
		window.draw(fpsText);
		window.display();

		if(counter.getElapsedTime().asMilliseconds() >= 1000){
			counter.restart();
			fpsText.setString(intToStr(fps));
			fps = 0;
		} else {
			fps++;
		}

		cout << player.x << " " << player.y << endl;
	}
	return 0;
}
