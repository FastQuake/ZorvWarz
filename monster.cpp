#include "entity.h"
#include "ai.h"
#include "server.h"
#include "main.h"
#include <cmath>
#include <sstream>

using namespace std;

Monster::Monster(){
	type = "monster";
	drawable = false;
	collides = true;
	readyToUpdate = true;
	alive = true;

	xVel = 0.0;
	yVel = 0.0;
	x = 800/2; //Hardcoded screen size for x,y cause fight the power
	y = 600/2;
	speed = 128.0;
	health = 10;
	targetNodeNum = 0;

	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
}

void Monster::onCollision(Entity *object, sf::FloatRect otherBox){
	//cout << "I GOT BOOM BOOM" << endl;
	if(object->type == "bullet" && object->alive){
		health--;
		cout << "health down: " << health << endl;
		object->alive = false;
		return;
	} else if(object->type == "monster"){
		return;
	}else if(object->type == "player"){
		return;
	}else if(object->type == "box"){
		return;
	}else{
		collideWall(otherBox);
	}
}

void Monster::update(int framecount,float dTime){
	/*currentPath.clear();
	currentPath.resize(0);
	this->buildPath(1);*/
	stepPath(currentPath[targetNodeNum], dTime);
	stringstream ss;
	ss << this->ID << " " << this->x << " " << this->y << " 0";
	//cout << ss.str() << endl;
	ENetPacket *movePacket = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_UNSEQUENCED);
	if(p1->connected)
		enet_peer_send(p1->peer,0,movePacket);
	if(p2->connected)
		enet_peer_send(p2->peer,0,movePacket);
	if(health <= 0){
		this->alive = false;
		ENetPacket *despawnPacket = createPacket(scDespawn,intToStr(this->ID),ENET_PACKET_FLAG_RELIABLE);
		if(p1->connected)
			enet_peer_send(p1->peer,0,despawnPacket);
		if(p2->connected)
			enet_peer_send(p2->peer,0,despawnPacket);
	}


	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;
}

void Monster::buildPath(){
	//We have AIManager aim, Mob *p1 and Mob *p2 available for use
	sf::Vector2f targetPos;
	vector<Node*> ignoreList;
	int player = 0;

	if(singleplayer)
		player = 1;
	else
		player = rand() % 2+1;

	if(player == 1)
		targetPos = sf::Vector2f(p1->x,p1->y);
	else
		targetPos = sf::Vector2f(p2->x,p2->y);

	Node *firstNode = aim.findVisibleNode(sf::Vector2f(x,y),serverShip->collisionBoxes);
	Node *destinationNode = aim.findVisibleNode(targetPos,serverShip->collisionBoxes);
	currentPath.push_back(firstNode);
	ignoreList.push_back(firstNode);

	bool pathComplete = false;
	while(!pathComplete){
		Node *closestNode; //Neighbor node closest to destination
		float xdiff = 0;
		float ydiff = 0;
		float distance;
		bool ignored = false;

		vector<Node*> neighbors = currentPath.back()->neighbors;
		for(int i=0,min=9999,dead=0;i<neighbors.size();i++){
			ignored = false;
			if(std::find(ignoreList.begin(),ignoreList.end(),neighbors[i]) != ignoreList.end())
				ignored = true;
			if(ignored){
				dead++;
				if(dead == neighbors.size()){
					ignoreList.push_back(currentPath.back());
					currentPath.pop_back();
					break;
				}
				continue;
			}

			xdiff = abs(neighbors[i]->middle.x-destinationNode->middle.x);
			ydiff = abs(neighbors[i]->middle.y-destinationNode->middle.y);
			distance = sqrt(pow(xdiff,2.0f)+pow(ydiff,2.0f));
			if(distance<min){
				min = distance;
				closestNode = neighbors[i];
			}
		}
		if(std::find(ignoreList.begin(),ignoreList.end(),closestNode) == ignoreList.end()){
			currentPath.push_back(closestNode);
			ignoreList.push_back(currentPath.back());
		}
		if(currentPath.back() == destinationNode)
			pathComplete = true;
	}
	cout << currentPath.size();
}
	
void Monster::stepPath(Node* currentNode,float dTime){
	//float dTime = 1.0f/FPS;
	float ysign = 0.0f;
	float xsign = 0.0f;

	if(((y+16.0f)-currentNode->middle.y) < -0.1)
		ysign = 1.0f;
	else if(((y+16.0f)-currentNode->middle.y) > 0.1)
		ysign = -1.0f;
	else
		ysign = 0.0f;
		
	if(((x+16.0f)-currentNode->middle.x) < -0.1)
		xsign = 1.0f;
	else if(((x+16.0f)-currentNode->middle.x) > 0.1)
		xsign = -1.0f;
	else
		xsign = 0.0f;

	//cout << xsign << " " << ysign << endl;
	xVel = xsign*(speed*dTime);
	yVel = ysign*(speed*dTime);

	if(abs(xVel) > 3){
		xVel = (xVel/abs(xVel)) * 3; 
	}
	if(abs(yVel) > 3){
		yVel = (yVel/abs(yVel))*3;
	}
		
	this->x += xVel;
	this->y += yVel;

	if(targetNodeNum+1 >= currentPath.size())
		return;
	if(currentNode->nodeBox.contains(this->x+16.0f,this->y+16.0f))
		this->targetNodeNum++;
}

void Monster::drawPath(sf::RenderWindow *screen, int screenx, int screeny){
	sf::Vector2f p1;
	sf::Vector2f p2;
	sf::RectangleShape rectangle;
	rectangle.setSize(sf::Vector2f(32.0f,32.0f));
	rectangle.setPosition(currentPath[0]->nodeBox.left-screenx,currentPath[0]->nodeBox.top-screeny);
	screen->draw(rectangle);

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
			line[0].color = sf::Color::Green;
			line[1].color = sf::Color::Red;
			screen->draw(line,2,sf::Lines);
		}
	}
}
