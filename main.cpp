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
ShipEntity *ship;
Player *player;

string intToStr(int num){
	stringstream ss;
	ss << num;
	return ss.str();
} 

void addEntities(){
	ship = new ShipEntity(tilesFile);
	player = new Player(playerFile);
	entities.entityList.push_back(ship);
	entities.entityList.push_back(player);
}

void setup(){
	//Set seed based on time
	int seed = time(NULL);
	cout << "seed: " << seed << endl;
	srand(seed);

	addEntities();
}

void cleanup(){
	delete ship;
	delete player;
}

int main(int argc, char *argv[]){
	sf::RenderWindow window;
	window.create(sf::VideoMode(800,600),"Test");
	window.setFramerateLimit(60);
	sf::Event event;

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
		//Get current FPS
		if(counter.getElapsedTime().asSeconds() > 1){
			counter.restart();
			fpsText.setString(intToStr(fps));
			fps = 0;
		} else {
			fps++;
		}
		//Get input
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			} 
		}
		
		//Update all the entities
		entities.updateEntities(0);
		entities.collideEntities();

		//draw stuff
		window.clear();
		entities.drawEntities(&window,player->x-400,player->y-300); //Hardcoded screenx and screeny, may fix later
		window.draw(fpsText);
		window.display();


	}
	cleanup();
	return 0;
}
