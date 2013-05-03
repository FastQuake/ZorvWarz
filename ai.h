#ifndef AI_H
#define AI_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "entity.h"

using namespace std;

class Node{
public:
	sf::FloatRect nodeBox;
	vector<Node*> neighbors;

	Node(sf::FloatRect nodeBox);
	void findNeighbors(vector<sf::FloatRect> collisionBox);

private:
	static const int radius = 1024;
	sf::FloatRect hitBox;
};

class AIManager{
public:
	vector<Node> nodeList;
	
	AIManager();
	void init(ShipEntity *ship);
	void connectNodes(ShipEntity ship);
};

extern AIManager aim;

#endif