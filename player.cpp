#include <cmath>
#include "entity.h"

Player::Player(std::string playerTexture){
	type = "player";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;

	xVol = 0;
	yVol = 0;
	x = 800/2;
	y = 600/2;
	speed = 5;

	texture.loadFromFile(playerTexture);
	playerSprite.setTexture(texture);
	playerSprite.setPosition(x,y); //Hardcoded screen size, may fix later

	collisionBox.push_back(sf::FloatRect(x,y,28,28));
}

void Player::update(int framecount){
	bool keyUp = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	bool keyDown = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	bool keyLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	bool keyRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

	if(keyUp){
		yVol = -speed;
	} else if(keyDown){
		yVol = speed;
	}
	if(keyLeft){
		xVol = -speed;
	} else if(keyRight){
		xVol = speed;
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::M)){
		collides = !collides;
	}


	//playerSprite.move(xVol,yVol);
	//x += xVol;
	//y += yVol;
	//xVol = 0;
	//yVol = 0;

	//collisionBox[0].left = x;
	//collisionBox[0].top = y;
	x += xVol;
	y += yVol;
	collisionBox[0].left = x;
	collisionBox[0].top = y;
}

void Player::onCollision(Entity *object, sf::FloatRect otherBox){
	int xx = 0;
	int yy = 0;
	if(xVol != 0){
		xx = -xVol/abs(xVol);
	}
	if(yVol != 0){
		yy = -yVol/abs(yVol);
	}
	std::cout << xx << " " << yy << std::endl;
	std::cout << "VOLS: " << xVol << " " << yVol << std::endl;
	while(collisionBox[0].intersects(otherBox)){
		x += xx;
		y += yy;
		collisionBox[0].left = x;
		collisionBox[0].top = y;
	}
	x += xx;
	y += yy;
	collisionBox[0].left = x;
	collisionBox[0].top = y;

}

void Player::draw(sf::RenderWindow *screen, int screenx,int screeny){
	//x += xVol;
	//y += yVol;
	xVol = 0;
	yVol = 0;
	//collisionBox[0].left = x;
	//collisionBox[0].top = y;
	screen->draw(playerSprite);
}
