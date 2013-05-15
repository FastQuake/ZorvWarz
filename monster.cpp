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
	atEnd = false;

	xVel = 0.0;
	yVel = 0.0;
	x = 800/2; //Hardcoded screen size for x,y cause fight the power
	y = 600/2;
	speed = 128.0;
	health = 10;
	targetNodeNum = 0;

	collisionBoxes.push_back(sf::FloatRect(x,y,30,30));

	if(singleplayer)
		targetPlayer = 1;
	else
		targetPlayer = rand() % 2+1;
}

void Monster::onCollision(Entity *object, sf::FloatRect otherBox){
	//cout << "I GOT BOOM BOOM" << endl;
	if(object->type == "bullet" && object->alive){
		health--;
		cout << "health down: " << health << endl;
		Bullet bullet = *(Bullet*)object;
		object->alive = false;
		if(bullet.playerFired == 1 && health<=0)
			stats.p1Kills++;
		if(bullet.playerFired == 2 && health<=0)
			stats.p2Kills++;
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

void Monster::update(int framecount, float dTime){
	if(!atEnd){	
		if(pathTimer.getElapsedTime().asSeconds() >= 5)
			buildPath();
		stepPath(currentPath[targetNodeNum], dTime);
	}else{
		sf::FloatRect targetBox;
		if(targetPlayer == 1)
			targetBox = p1->collisionBoxes[0];
		else
			targetBox = p2->collisionBoxes[0];
		sf::Vector2f targetPos = sf::Vector2f(targetBox.left+targetBox.width/2,targetBox.top+targetBox.height/2);
		//cout << targetPos.x << " " << targetPos.y << endl;
		sf::Vector2f thisMiddle = sf::Vector2f(this->collisionBoxes[0].left+this->collisionBoxes[0].width/2,
			this->collisionBoxes[0].top+this->collisionBoxes[0].height/2);
		if(!AIManager::isVisible(thisMiddle,targetBox,serverShip->collisionBoxes))
			buildPath();
		stepTowards(targetPos, dTime);
	}
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
	enet_host_flush(server);
	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;
}

void Monster::buildPath(){
	//cout << "building path" << this->ID << endl;
	currentPath.clear();
	currentPath.swap(currentPath);
	atEnd = false;
	//We have AIManager aim, Mob *p1 and Mob *p2 available for use
	sf::Vector2f targetPos;
	vector<Node*> ignoreList;

	if(targetPlayer == 1)
		targetPos = sf::Vector2f(p1->x+16.0f,p1->y+16.0f);
	else
		targetPos = sf::Vector2f(p2->x+16.0f,p2->y+16.0f);

	Node *firstNode = aim.findVisibleNode(sf::Vector2f(x,y),serverShip->collisionBoxes);
	Node *destinationNode = aim.findVisibleNode(targetPos,serverShip->collisionBoxes);
	//cout << destinationNode->middle.x << " " << destinationNode->middle.y << endl;
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
	pathTimer.restart();
	targetNodeNum = 0;
}
	
void Monster::stepPath(Node* currentNode,float dTime){
	stepTowards(currentNode->middle, dTime);

	if(currentPath[currentPath.size()-1]->nodeBox.intersects(this->collisionBoxes[0])){
		cout << "path ended" << this->ID << endl;
		cout << "node position" << this->ID << " " << currentPath[currentPath.size()-1]->middle.x << " " << currentPath[currentPath.size()-1]->middle.y << endl;
		cout << "node position WITH THINGY" << this->ID << " " << currentPath[currentPath.size()-1]->nodeBox.left << " " << currentPath[currentPath.size()-1]->nodeBox.top << endl;
		cout << "mah crazy ass is at" << this->ID << " " << this->x+16 << " " << this->y+16 << endl;
		atEnd = true;
		return;
	}
	if(currentNode->nodeBox.intersects(this->collisionBoxes[0]))
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

void Monster::stepTowards(sf::Vector2f targetPos, float dTime){
	float ysign = 0.0f;
	float xsign = 0.0f;

	if(((y+16.0f)-targetPos.y) < -1.0f)
		ysign = 1.0f;
	else if(((y+16.0f)-targetPos.y) > 1.0f)
		ysign = -1.0f;
	else
		ysign = 0.0f;
		
	if(((x+16.0f)-targetPos.x) < -1.0f)
		xsign = 1.0f;
	else if(((x+16.0f)-targetPos.x) > 1.0f)
		xsign = -1.0f;
	else
		xsign = 0.0f;

	//cout << xsign << " " << ysign << endl;
	xVel = xsign*(speed*dTime);
	yVel = ysign*(speed*dTime);
	
//	cout << xVel << " " << yVel << endl;

	if(fabs(xVel) > 3.0f){
		xVel = xsign*3.0f; 
	}
	if(fabs(yVel) > 3.0f){
		yVel = ysign*3.0f;
	}

	this->x += xVel;
	this->y += yVel;
}
