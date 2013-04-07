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
	Ship bleh;

	sf::RenderWindow window;
	window.create(sf::VideoMode(800,600),"Test");
	window.setFramerateLimit(60);

	while(window.isOpen()){
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}
		}

		window.clear();
		window.display();
	}
	return 0;
}
