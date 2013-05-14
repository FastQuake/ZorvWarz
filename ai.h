#ifndef AI_H
#define AI_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "entity.h"

using namespace std;

class Node{
public:
	struct threadArgs{
	Node *thisNode;
	vector<sf::FloatRect> collisionBoxes;
	};

	sf::FloatRect nodeBox;
	sf::Vector2f middle;
	vector<Node*> neighbors;

	Node(sf::FloatRect nodeBox);
	static void findNeighbors(threadArgs args);
};

class AIManager{
public:
	vector<Node> nodeList;
	
	AIManager();
	void init(ShipEntity *ship);
	void connectNodes(ShipEntity ship);
	void drawNet(sf::RenderWindow *screen, int screenx, int screeny);
	static void spawnMonsters(vector<Entity*> *entityList, int numMonsters);
	static bool isVisible(sf::Vector2f startPoint, sf::FloatRect targetBox, vector<sf::FloatRect> targetColBoxes);
	Node *findVisibleNode(sf::Vector2f relPoint, vector<sf::FloatRect> collisionBoxes);
};

extern AIManager aim;

#endif
