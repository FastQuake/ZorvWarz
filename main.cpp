#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include "ship.h"
using namespace std;

int main(int argc, char *argv[]){
	//Set seed based on time
	int seed = time(NULL);
	cout << "seed: " << seed << endl;
	srand(seed);
	//create level for testing
	Ship bleh("data/textures/tiles.png");

	sf::RenderWindow window;
	window.create(sf::VideoMode(800,600),"Test");
	window.setFramerateLimit(60);

	int x = window.getSize().x/2;
	int y = window.getSize().y/2;
	
	sf::View view = window.getView();

	while(window.isOpen()){
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			} else if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::Up){
					y -= 32;
				}
				else if(event.key.code == sf::Keyboard::Down){
					y += 32;
				}
				else if(event.key.code == sf::Keyboard::Left){
					x -= 32;
				}
				else if(event.key.code == sf::Keyboard::Right){
					x += 32;
				}
				else if(event.key.code == sf::Keyboard::PageUp){
					view.zoom(0.9);
				}
				else if(event.key.code == sf::Keyboard::PageDown){
					view.zoom(1.1);
				}
			}
		}

		view.setCenter(x,y);
		window.setView(view);

		window.clear();
		bleh.drawMap(&window);
		window.display();
	}
	return 0;
}
