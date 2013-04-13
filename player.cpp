#include "entity.h" 

Player::Player(std::string playerTexture){
	type = "player";
	drawable = true;
	collides = true;
	readyToUpdate = true;
	alive = true;

	xVol = 0;
	yVol = 0;
	speed = 5;

	texture.loadFromFile(playerTexture);
	playerSprite.setTexture(texture);
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

	playerSprite.move(xVol,yVol);
	xVol = 0;
	yVol = 0;
	x = playerSprite.getPosition().x;
	y = playerSprite.getPosition().y;
}

void Player::onCollision(Entity Object){
}

void Player::draw(sf::RenderWindow *screen){
	screen->draw(playerSprite);
}
