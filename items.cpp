#include "entity.h" 
#include "server.h"
#include <sstream>

AmmoBox::AmmoBox(float x, float y){
	type = "box";
	drawable = true;
	collides = true;
	readyToUpdate = false;
	alive = true;

	this->x = x;
	this->y = y;

	ammoTex.loadFromFile("data/textures/ammo.png");
	box.setTexture(ammoTex);

	collisionBoxes.push_back(sf::FloatRect(x,y,20,20));
}

void AmmoBox::onCollision(Entity *object, sf::FloatRect otherbox){
	if(object->type == "player"){
		std::cout << "REMOVEING BOX" << std::endl;
		alive = false;
		collides = false;
	}
}

void AmmoBox::draw(sf::RenderWindow *screen, int screenx, int screeny){
	box.setPosition(x-screenx,y-screeny);
	screen->draw(box);
}

Stairs::Stairs(float x,float y, int type){
	this->type = "stairs";
	sType = type;
	drawable = true;
	collides = true;
	readyToUpdate = false;
	alive = true;

	this->x = x;
	this->y = y;

	sTex.loadFromFile("data/textures/stairs.png");
	sSprite.setTexture(sTex);

	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
}

void Stairs::onCollision(Entity *object, sf::FloatRect otherBox){
	if(sType == 0 && object->type == "player"){
		//DO SHIT TO LOAD NEXT LEVEL
		despawnLevel();
		//Tell client to prep for change level
		ENetPacket *packet = createPacket(scChgLvl,"",ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(p1->peer,0,packet);
		if(p2->connected)
			enet_peer_send(p2->peer,0,packet);
		enet_host_flush(server);
		loadLevel(true);
		//send client new map
		packet = createPacket(scMap,getMapData(false),ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(p1->peer,0,packet);
		if(p2->connected)
			enet_peer_send(p2->peer,0,packet);
		enet_host_flush(server);

		//give client new XY
		sf::Vector2f p1Pos = serverShip->getRandomFloorTile();
		sf::Vector2f p2Pos = serverShip->getRandomFloorTile();
		p1->x = p1Pos.x*32;
		p1->y = p1Pos.y*32;
		p2->x = p2Pos.x*32;
		p2->y = p2Pos.y*32;
		stringstream ss;
		ss << 0 << " " << p1->x << " " << p1->y << " " << 0;
		packet= createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(p1->peer,0,packet);

		ss.str("");
		ss.clear();
		if(p2->connected){
			ss << 0 << " " << p2->x << " " << p2->y << " " << 0;
			packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(p1->peer,0,packet);
		}
		enet_host_flush(server);
	}
}

void Stairs::draw(sf::RenderWindow *screen, int screenx, int screeny){
	sSprite.setPosition(x-screenx,y-screeny);
	screen->draw(sSprite);
}
