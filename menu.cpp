#include <sstream>
#include "menu.h" 

bool inputIP = false;
string ipText = "";

sf::Texture pButTex;
sf::Sprite pButSprite;

sf::Texture jButTex;
sf::Sprite jButSprite;

sf::Texture bButTex;
sf::Sprite bButSprite;

sf::Texture bgTexture;
sf::Sprite bgSprite;

sf::Text ipSprite;

sf::Clock bTimer;

void initMenu(){
	//Init all textures and sprites for main menu
	bgTexture.loadFromFile("data/textures/bg.png");
	bgSprite.setTexture(bgTexture);

	pButTex.loadFromFile("data/textures/play.png");
	pButSprite.setTexture(pButTex);

	pButSprite.setPosition(300,300);

	jButTex.loadFromFile("data/textures/join.png");
	jButSprite.setTexture(jButTex);

	bButTex.loadFromFile("data/textures/back.png");
	bButSprite.setTexture(bButTex);

	bButSprite.setPosition(10,450);

	jButSprite.setPosition(300,450);

	ipSprite.setFont(font);
	ipSprite.setPosition(400,300);

	bTimer.restart();
}

void updateMenu(){
	//get mouse position
	sf::Vector2f mPos;
	mPos.x = sf::Mouse::getPosition(window).x;
	mPos.y = sf::Mouse::getPosition(window).y;
	//If we are the main part of the menu
	if(!inputIP){
		if(mouseRight && bTimer.getElapsedTime().asMilliseconds() > 200){
			bTimer.restart();
			//If user presses play button
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
			//If user presses join button
			if(jButSprite.getGlobalBounds().contains(mPos)){
				cout << "HIT JOIB BUTTON" << endl;
				inputIP = true;
			}
		}
	} else { //If were at the join part of the menu
		if(mouseRight && bTimer.getElapsedTime().asMilliseconds() > 200){
			bTimer.restart();
			//If user presses back button
			if(bButSprite.getGlobalBounds().contains(mPos)){
				inputIP = false;
			}
			//If user presses join button
			if(jButSprite.getGlobalBounds().contains(mPos)){
				state = 1;
				inputIP = false;
				IPad = ipText;
				clientThread->launch();
				sf::sleep(sf::milliseconds(500));
				readyMutex.lock();
			}
		}
		//Set the text for the string showing input
		ipSprite.setString(ipText);
		ipSprite.setOrigin(ipSprite.getGlobalBounds().width/2,
				ipSprite.getGlobalBounds().height/2);
	}
}


void drawMenu(sf::RenderWindow *screen){
	//Draw background
	screen->draw(bgSprite);
	//Draw the main menu
	if(!inputIP){
		screen->draw(pButSprite);
		screen->draw(jButSprite);
	} else { //Draw the join menu
		screen->draw(ipSprite);
		screen->draw(jButSprite);
		screen->draw(bButSprite);
	}
}
