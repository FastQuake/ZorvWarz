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
	speed = 200.0;

	bullets = 30;

	frame = 0;
	state = 0;

	pTexture.loadFromFile(playerTexture);
	gTexture.loadFromFile("data/textures/gun.png");
	playerSprite.setTexture(pTexture);
	playerSprite.setPosition(x,y);
	playerSprite.setTextureRect(sf::IntRect(frame*32,state*32,32,32));
	gun.setTexture(gTexture);
	gun.setOrigin(-8,5);
	gun.setPosition(x+16,y+16);

	collisionBoxes.push_back(sf::FloatRect(x,y,30,30));
	bClock.restart();
}

void Player::update(int framecount){
	//float dTime = frameTime.getElapsedTime().asMilliseconds()/1000.0f;
	//float dTime = 1.0f/FPS;
	float dTime = dt.asSeconds();
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

	if(xVel !=0 || yVel !=0){
		if(framecount % 5 == 0){
			if(frame < 2){
				frame++;
			} else {
				frame = 0;
			}
		}
	}else{
		frame = 0;
	}

	x += xVel;
	y += yVel;
	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;

	//Get mouse rotation
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	mousePos.x -= 400+16;
	mousePos.y -= 300+16;

	rot = atan2((float)mousePos.y,(float)mousePos.x) * (180/3.14);

	//Shoot stuff
	if(mouseRight && bClock.getElapsedTime().asMilliseconds() > 100 &&
			bullets > 0){
		bClock.restart();
		entities.entityList.push_back(new Bullet(x+16,y+16,rot));
		bullets--;
		packetMutex.lock();
		packetList.push_back("2");
		packetMutex.unlock();
	}
}

void Player::onCollision(Entity *object, sf::FloatRect otherBox){
	if(object->type == "bullet"){
		return;
	}else if(object->type == "player"){
		return;
	}else if(object->type == "monster"){
		return;
	}else if(object->type == "box"){
		bullets += 30;
		object->alive = false;
		object->collides = false;
	}else {
		collideWall(otherBox);
	}
}

void Player::draw(sf::RenderWindow *screen, int screenx,int screeny){
	xVel = 0;
	yVel = 0;
	playerSprite.setTextureRect(sf::IntRect(frame*32,state*32,32,32));
	gun.setRotation(rot);
	rot = gun.getRotation();
	//std::cout << "rot : " << gun.getRotation() << std::endl;
	if(gun.getRotation() > 90 && gun.getRotation() < 270){
		gun.setScale(1,-1);
		playerSprite.setScale(-1,1);
		playerSprite.setPosition(400+32,300);
	} else {
		gun.setScale(1,1);
		playerSprite.setScale(1,1);
		playerSprite.setPosition(400,300);
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
	health = 0;

	x = 0;
	y = 0;

	frame = 0;
	state = 0;

	texture.loadFromFile(textureFile);
	mobSprite.setTexture(texture);
	mobSprite.setPosition(0,0); //Hardcoded screen size, may fix later
	mobSprite.setTextureRect(sf::IntRect(frame*32,state*32,32,32));

	collisionBoxes.push_back(sf::FloatRect(x,y,32,32));
	animTimer.restart();
}

void Mob::update(int framecount){

	//If zombie id dead, prep for death animation
	if(health == -1){
		health = 1;
		frame = 0;
		state = 1;
	}
	if(animTimer.getElapsedTime().asMilliseconds() < 100 || health == 1){
		if(framecount % 5 == 0){
			//If zombie is dead kill it after death animation
			if(frame == 2 && health == 1){
				alive = false;
			}
			if(frame < 2){
				frame++;
			} else {
				frame = 0;
			}
		}
	} else {
		frame = 0;
	}
	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;
}

void Mob::draw(sf::RenderWindow *window,int screenx,int screeny){
	mobSprite.setPosition(x-screenx,y-screeny);
	mobSprite.setTextureRect(sf::IntRect(frame*32,state*32,32,32));
	window->draw(mobSprite);
}

PMob::PMob(std::string textureFile, int id)
	:Mob(textureFile, id)
{
	type = "player";

	gTex.loadFromFile("data/textures/gun.png");
	gSprite.setTexture(gTex);
	gSprite.setOrigin(-8,5);
	gSprite.setPosition(x+16,y+16);
}

void PMob::draw(sf::RenderWindow *window, int screenx, int screeny){
	mobSprite.setPosition(x-screenx,y-screeny);
	mobSprite.setTextureRect(sf::IntRect(frame*32,state*32,32,32));
	gSprite.setPosition(x-screenx+16,y-screeny+16);
	gSprite.setRotation(rot);
	//std::cout << "rot : " << gun.getRotation() << std::endl;
	if(gSprite.getRotation() > 90 && gSprite.getRotation() < 270){
		gSprite.setScale(1,-1);
		mobSprite.setScale(-1,1);
		mobSprite.move(32,0);
	} else {
		gSprite.setScale(1,1);
		mobSprite.setScale(1,1);
	}
	window->draw(mobSprite);
	window->draw(gSprite);
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
	sBullet.setOrigin(9/2,2/2);
	sBullet.rotate(rot);
	sBullet.scale(2,2);

	//Get velocity
	vel.x = cos((rot*(3.14f/180.0f)))*500.0f;
	vel.y = sin((rot*(3.14f/180.0f)))*500.0f;

	//std::cout << "MOVING AT " << vel.x/20 << " " << vel.y/20 << std::endl;

	this->x += (vel.x/20.0);
	this->y += (vel.y/20.0);

	collisionBoxes.push_back(sBullet.getGlobalBounds());

}

void Bullet::update(int framecount){
	//float dTime = 1.0f/FPS;
	//std::cout << "FRAME: " << framecount << " X: " << x << " Y: " << y << std::endl;
	float dTime = dt.asSeconds();
	x += (vel.x * dTime);
	y += (vel.y * dTime);

	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;
}

void Bullet::onCollision(Entity *object, sf::FloatRect otherBox){
	if(object->type == "map"){
		alive = false;
	} if(object->type == "monster"){
		alive = false;
	}
}

void Bullet::draw(sf::RenderWindow *screen, int screenx, int screeny){
	sBullet.setPosition(x-screenx,y-screeny);
	screen->draw(sBullet);
}
