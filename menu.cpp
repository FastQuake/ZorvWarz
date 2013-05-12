#include "menu.h" 


sf::Texture pButTex;
sf::Sprite pButSprite;

sf::Texture jButTex;
sf::Sprite jButSprite;

sf::Texture bgTexture;
sf::Sprite bgSprite;

void initMenu(){
	bgTexture.loadFromFile("data/textures/bg.png");
	bgSprite.setTexture(bgTexture);

	pButTex.loadFromFile("data/textures/play.png");
	pButSprite.setTexture(pButTex);

	pButSprite.setPosition(300,300);

	jButTex.loadFromFile("data/textures/join.png");
	jButSprite.setTexture(jButTex);

	jButSprite.setPosition(300,450);
}

void updateMenu(){
	sf::Vector2f mPos;
	mPos.x = sf::Mouse::getPosition(window).x;
	mPos.y = sf::Mouse::getPosition(window).y;
	if(mouseRight){
		if(pButSprite.getGlobalBounds().contains(mPos)){
			cout << "HIT PLAY BUTTON" << endl;
			state = 1;
			IPad = "localhost";
			initServer();
			serverThread->launch();
			clientThread->launch();
			sf::sleep(sf::milliseconds(500));
			readyMutex.lock();
		}
		if(jButSprite.getGlobalBounds().contains(mPos)){
			cout << "HIT JOIB BUTTON" << endl;
		}
	}
}


void drawMenu(sf::RenderWindow *screen){
	screen->draw(bgSprite);
	screen->draw(pButSprite);
	screen->draw(jButSprite);
}
