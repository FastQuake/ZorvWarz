#include "entity.h"
#include "ai.h"
#include "server.h"
#include <math.h>

using namespace std;

Monster::Monster(){
	type = "monster";
	drawable = false;
	collides = true;
	readyToUpdate = true;
	alive = true;

	xVol = 0.0;
	yVol = 0.0;
	x = 800/2; //Hardcoded screen size for x,y cause fight the power
	y = 600/2;
	speed = 200.0;

	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
}

void Monster::onCollision(Entity *object, sf::FloatRect otherBox){
	collideWall(otherBox);
}

void Monster::update(int framecount){
	/*currentPath.clear();
	currentPath.resize(0);
	this->buildPath(1);*/
}

void Monster::buildPath(int player){
	//We have AIManager aim, Mob *p1 and Mob *p2 available for use
	sf::Vector2f targetPos;

	if(player == 1)
		targetPos = sf::Vector2f(p1->x,p1->y);
	else
		targetPos = sf::Vector2f(p2->x,p2->y);

	Node *firstNode = aim.findVisibleNode(sf::Vector2f(x,y),serverShip->collisionBoxes);
	Node *destinationNode = aim.findVisibleNode(targetPos,serverShip->collisionBoxes);
	currentPath.push_back(firstNode);

	bool pathComplete = false;
	while(!pathComplete){
		Node *closestNode; //Neighbor node closest to destination
		float xdiff = 0;
		float ydiff = 0;
		float distance;
		vector<Node*> neighbors = currentPath.back()->neighbors;
		for(int i=0,min=9999;i<neighbors.size();i++){
			xdiff = abs(neighbors[i]->middle.x-destinationNode->middle.x);
			ydiff = abs(neighbors[i]->middle.y-destinationNode->middle.y);
			distance = sqrt(pow(xdiff,2.0f)+pow(ydiff,2.0f));
			if(distance<min){
				min = distance;
				closestNode = neighbors[i];
			}
		}
		currentPath.push_back(closestNode);
		if(currentPath.back() == destinationNode)
			pathComplete = true;
	}
	cout << currentPath.size();
}
	
void Monster::drawPath(sf::RenderWindow *screen, int screenx, int screeny){
	sf::Vector2f p1;
	sf::Vector2f p2;

	for(int i=0;i<currentPath.size();i){
		p1 = currentPath[i]->middle;
		p1.x -= screenx;
		p1.y -= screeny;
		i++;

		if(i<currentPath.size()){
			p2 = currentPath[i]->middle;
			p2.x -= screenx;
			p2.y -= screeny;
			sf::Vertex line[] = {p1,p2};
			line[0].color = sf::Color::Red;
			line[1].color = sf::Color::Red;
			screen->draw(line,2,sf::Lines);
		}
	}
}