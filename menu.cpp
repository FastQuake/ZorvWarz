#include <sstream>
#include "menu.h" 

bool inputIP = false;
string ipText = "";

sf::Texture pButTex;
sf::Sprite pButSprite;

sf::Texture jButTex;
sf::Sprite jButSprite;

sf::Texture bgTexture;
sf::Sprite bgSprite;

sf::Text ipSprite;

void initMenu(){
	bgTexture.loadFromFile("data/textures/bg.png");
	bgSprite.setTexture(bgTexture);

	pButTex.loadFromFile("data/textures/play.png");
	pButSprite.setTexture(pButTex);

	pButSprite.setPosition(300,300);

	jButTex.loadFromFile("data/textures/join.png");
	jButSprite.setTexture(jButTex);

	jButSprite.setPosition(300,450);

	ipSprite.setFont(font);
	ipSprite.setPosition(400,300);
}

void updateMenu(){
	sf::Vector2f mPos;
	mPos.x = sf::Mouse::getPosition(window).x;
	mPos.y = sf::Mouse::getPosition(window).y;
	if(!inputIP){
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
				inputIP = true;
			}
		}
	} else {
		if(mouseRight){
			if(jButSprite.getGlobalBounds().contains(mPos)){
				state = 1;
				inputIP = false;
				IPad = ipText;
				clientThread->launch();
				sf::sleep(sf::milliseconds(500));
				readyMutex.lock();
			}
		}
		ipSprite.setString(ipText);
		ipSprite.setOrigin(ipSprite.getGlobalBounds().width/2,
				ipSprite.getGlobalBounds().height/2);
	}
}


void drawMenu(sf::RenderWindow *screen){
	screen->draw(bgSprite);
	if(!inputIP){
		screen->draw(pButSprite);
		screen->draw(jButSprite);
	} else {
		screen->draw(ipSprite);
		screen->draw(jButSprite);
	}
}
