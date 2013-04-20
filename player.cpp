#include <cmath>
#include "main.h"

Player::Player(std::string playerTexture){
	type = "player";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;

	xVol = 0;
	yVol = 0;
	x = 800/2; //Hardcoded screen size for x,y cause fight the power
	y = 600/2;
	speed = 5;

	texture.loadFromFile(playerTexture);
	playerSprite.setTexture(texture);
	playerSprite.setPosition(x,y);

	collisionBox.push_back(sf::FloatRect(x,y,32,32));
}

void Player::update(int framecount){
	if(keyUp && sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		yVol = -speed;
	} else if(keyDown && sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		yVol = speed;
	}
	if(keyLeft && sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		xVol = -speed;
	} else if(keyRight && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		xVol = speed;
	}

	x += xVol;
	y += yVol;
	collisionBox[0].left = x;
	collisionBox[0].top = y;
}

void Player::onCollision(Entity *object, sf::FloatRect otherBox){

	int xMag = (collisionBox[0].left+collisionBox[0].width) - (otherBox.left+otherBox.width);
	int yMag = (collisionBox[0].top+collisionBox[0].height) - (otherBox.top+otherBox.height);
	/*if(xMag == yMag){
		xMag++;
	}*/

	int xx = 0;
	int yy = 0;
	if(xMag != 0){
		xx = xMag/abs(xMag);
	}
	if(yMag != 0){
		yy = yMag/abs(yMag);
	}
	std::cout << xx << " " << yy << std::endl;
	std::cout << "VOLS: " << xVol << " " << yVol << std::endl;
	std::cout << "MAGS: " << xMag << " " << yMag << std::endl;
	while(collisionBox[0].intersects(otherBox)){
		if(abs(xMag) > abs(yMag)){
			std::cout << "DOIN X" << "\n";
			x += xx;
		} else {
			std::cout << "DOIN Y" << "\n";
			y += yy;
		}
		collisionBox[0].left = x;
		collisionBox[0].top = y;
	}
	if(abs(xMag) >= abs(yMag)){
		x += xx;
	} else {
		y += yy;
	}
	collisionBox[0].left = x;
	collisionBox[0].top = y;

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

	collisionBox.push_back(sf::FloatRect(x,y,32,32));
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

	collisionBox.push_back(sf::FloatRect(x,y,32,32));
}

void Mob::update(int framecount){
	collisionBox[0].left = x;
	collisionBox[0].top = y;
}

void Mob::draw(sf::RenderWindow *window,int screenx,int screeny){
	mobSprite.setPosition(x-screenx,y-screeny);
	window->draw(mobSprite);
}
