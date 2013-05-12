#include <cmath>
#include "main.h"

sf::Clock bClock;

Player::Player(std::string playerTexture){
	type = "player";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;

	xVel = 0.0;
	yVel = 0.0;
	x = 800/2; //Hardcoded screen size for x,y cause fight the power
	y = 600/2;
	speed = 400.0;

	pTexture.loadFromFile(playerTexture);
	gTexture.loadFromFile("data/textures/gun.png");
	playerSprite.setTexture(pTexture);
	playerSprite.setPosition(x,y);
	gun.setTexture(gTexture);
	gun.setOrigin(-8,5);
	gun.setPosition(x+16,y+16);

	collisionBoxes.push_back(sf::FloatRect(x,y,30,30));
	bClock.restart();
}

void Player::update(int framecount){
	//float dTime = frameTime.getElapsedTime().asMilliseconds()/1000.0f;
	float dTime = 1.0f/FPS;
	if(keyUp && sf::Keyboard::isKeyPressed(sf::Keyboard::W)){
		yVel = -speed*dTime;
	} else if(keyDown && sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
		yVel = speed*dTime;
	}
	if(keyLeft && sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
		xVel = -speed*dTime;
	} else if(keyRight && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
		xVel = speed*dTime;
	}

	//std::cout << dTime <<  " xVel: " << xVel << " yVel: " << yVel << std::endl;

	x += xVel;
	y += yVel;
	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;

	//Get mouse rotation
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	mousePos.x -= 400;
	mousePos.y -= 300;

	rot = atan2((float)mousePos.y,(float)mousePos.x) * (180/3.14);

	if(bClock.getElapsedTime().asMilliseconds() > 100){
		bClock.restart();
	}

	//Shoot stuff
	if(mouseRight && bClock.getElapsedTime().asMilliseconds() < 100){
		entities.entityList.push_back(new Bullet(x+16,y+16,rot));
	}
}

void Player::onCollision(Entity *object, sf::FloatRect otherBox){
	if(object->type == "bullet"){
		return;
	}
	collideWall(otherBox);
}

void Player::draw(sf::RenderWindow *screen, int screenx,int screeny){
	xVel = 0;
	yVel = 0;
	gun.setRotation(rot);
	//std::cout << "rot : " << gun.getRotation() << std::endl;
	if(gun.getRotation() > 90 && gun.getRotation() < 270){
		gun.setScale(1,-1);
	} else {
		gun.setScale(1,1);
	}
	screen->draw(playerSprite);
	screen->draw(gun);
}

Mob::Mob(int id){
	type = "mob";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;
	connected = false;
	this->ID = id;
	
	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
}

Mob::Mob(std::string textureFile, int id){
	type = "mob";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;
	this->ID = id;

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


Bullet::Bullet(float x, float y, float rot){
	type = "bullet";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;

	this->x = x;
	this->y = y;
	this->rot = rot;

	sBullet.setTexture(bTex);	
	sBullet.setOrigin(5,5);
	sBullet.rotate(rot);

	//Get velocity
	vel.x = cos((rot*(3.14f/180.0f)))*500.0f;
	vel.y = sin((rot*(3.14f/180.0f)))*500.0f;

	collisionBoxes.push_back(sBullet.getGlobalBounds());

}

void Bullet::update(int framecount){
	float dTime = 1.0f/FPS;
	x += vel.x * dTime;
	y += vel.y * dTime;

	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;
}

void Bullet::onCollision(Entity *object, sf::FloatRect otherBox){
	if(object->type == "map"){
		alive = false;
	}
}

void Bullet::draw(sf::RenderWindow *screen, int screenx, int screeny){
	sBullet.setPosition(x-screenx,y-screeny);
	screen->draw(sBullet);
}
