#include <cmath>
#include "main.h"

Player::Player(std::string playerTexture){
	type = "player";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;

	xVol = 0.0;
	yVol = 0.0;
	x = 800/2; //Hardcoded screen size for x,y cause fight the power
	y = 600/2;
	speed = 200.0;

	texture.loadFromFile(playerTexture);
	playerSprite.setTexture(texture);
	playerSprite.setPosition(x,y);

	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
}

void Player::update(int framecount){
	//float dTime = frameTime.getElapsedTime().asMilliseconds()/1000.0f;
	float dTime = 1.0f/FPS;
	if(keyUp && sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		yVol = -speed*dTime;
	} else if(keyDown && sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		yVol = speed*dTime;
	}
	if(keyLeft && sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		xVol = -speed*dTime;
	} else if(keyRight && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		xVol = speed*dTime;
	}

	//std::cout << dTime <<  " Xvol: " << xVol << " YVol: " << yVol << std::endl;

	x += xVol;
	y += yVol;
	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;
}

void Player::onCollision(Entity *object, sf::FloatRect otherBox){
	collideWall(otherBox);
}

void Player::draw(sf::RenderWindow *screen, int screenx,int screeny){
	xVol = 0;
	yVol = 0;
	screen->draw(playerSprite);
}

Mob::Mob(){
	type = "mob";
	drawable = false;
	collides = true;
	readyToUpdate = true;
	alive = true;

	x = 0;
	y = 0;

	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
}

Mob::Mob(std::string textureFile){
	type = "mob";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;

	x = 0;
	y = 0;

	texture.loadFromFile(textureFile);
	mobSprite.setTexture(texture);
	mobSprite.setPosition(0,0); //Hardcoded screen size, may fix later

	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
}

void Mob::update(int framecount){
	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;
}

void Mob::draw(sf::RenderWindow *window,int screenx,int screeny){
	mobSprite.setPosition(x-screenx,y-screeny);
	window->draw(mobSprite);
}
