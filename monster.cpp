#include "entity.h"
#include "ai.h"

using namespace std;

Monster::Monster(string monsterTexture){
	type = "monster";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;

	xVol = 0.0;
	yVol = 0.0;
	x = 800/2; //Hardcoded screen size for x,y cause fight the power
	y = 600/2;
	speed = 200.0;

	texture.loadFromFile(monsterTexture);
	monsterSprite.setTexture(texture);
	monsterSprite.setPosition(x,y);

	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
}

void Monster::onCollision(Entity *object, sf::FloatRect otherBox){
	collideWall(otherBox);
}

void Monster::update(int framecount){

}

void Monster::draw(sf::RenderWindow *screen, int screenx,int screeny){
	xVol = 0;
	yVol = 0;
	screen->draw(monsterSprite);
}

void Monster::buildPath(){

}