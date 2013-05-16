#include <sstream>
#include "menu.h" 

bool inputIP = false;
bool loading = false;
string ipText = "";

sf::Texture pButTex;
sf::Sprite pButSprite;

sf::Texture jButTex;
sf::Sprite jButSprite;

sf::Texture mButTex;
sf::Sprite mButSprite;

sf::Texture bButTex;
sf::Sprite bButSprite;

sf::Texture bgTexture;
sf::Sprite bgSprite;

sf::Text ipSprite;
sf::Text loadText;
sf::Text enterIP;

sf::Clock bTimer;

void initMenu(){
	//Init all textures and sprites for main menu
	bgTexture.loadFromFile("data/textures/bg.png");
	bgSprite.setTexture(bgTexture);

	pButTex.loadFromFile("data/textures/play.png");
	pButSprite.setTexture(pButTex);

	pButSprite.setPosition(200,300);

	jButTex.loadFromFile("data/textures/join.png");
	jButSprite.setTexture(jButTex);

	bButTex.loadFromFile("data/textures/back.png");
	bButSprite.setTexture(bButTex);

	mButTex.loadFromFile("data/textures/multiplayer.png");
	mButSprite.setTexture(mButTex);
	
	mButSprite.setPosition(410, 300);

	bButSprite.setPosition(10,450);

	jButSprite.setPosition(300,450);

	ipSprite.setFont(font);
	ipSprite.setPosition(400,300);

	loadText.setFont(font);
	loadText.setPosition(400,300);

	enterIP.setFont(font);
	enterIP.setPosition(100,200);

	bTimer.restart();
}

void updateMenu(){
	//get mouse position
	enterIP.setString("Enter IP of server: ");
	sf::Vector2f mPos;
	mPos.x = mousePos.x;
	mPos.y = mousePos.y;
	//If we are the main part of the menu
	if(loading){
		if(!connecting){
			loading = false;
			inputIP = true;
			ipText = "Bad IP";
		}
		loadText.setString("Loading...");
		loadText.setOrigin(loadText.getGlobalBounds().width/2,
				loadText.getGlobalBounds().height/2);
		//cout << "LOADING" << endl;
		if(ready){
			cout << "CHANGING STATES" << endl;
			state = 1;
			sf::sleep(sf::milliseconds(500));
	}
	}else{
		if(!inputIP){
			if(mouseRight && bTimer.getElapsedTime().asMilliseconds() > 200){
				bTimer.restart();
				//If user presses play button
				if(pButSprite.getGlobalBounds().contains(mPos)){
					serverReady = false;
					mouseRight = false;
					IPad = "localhost";
					//initServer();
					serverThread->launch();
					clientThread->launch();
					sf::sleep(sf::milliseconds(500));
					loading = true;
					connecting = true;
					singleplayer = true;
					p2Timer.restart();
				}
				//If user presses Multi-Player button
				if(mButSprite.getGlobalBounds().contains(mPos)){
					serverReady = false;
					mouseRight = false;
					IPad = "localhost";
					//initServer();
					serverThread->launch();
					clientThread->launch();
					sf::sleep(sf::milliseconds(500));
					loading = true;
					connecting = true;
					singleplayer = false;
					p2Timer.restart();
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
					mouseRight = false;
					inputIP = false;
					IPad = ipText;
					clientThread->launch();
					sf::sleep(sf::milliseconds(500));
					loading = true;
				}
			}
			//Set the text for the string showing input
			ipSprite.setString(ipText);
			ipSprite.setOrigin(ipSprite.getGlobalBounds().width/2,
					ipSprite.getGlobalBounds().height/2);
		}
	}
}


void drawMenu(sf::RenderWindow *screen){
	//Draw background
	screen->draw(bgSprite);
	//Draw the main menu

	if(loading){
		screen->clear();
		screen->draw(loadText);
	}else {
		if(!inputIP){
			screen->draw(pButSprite);
			screen->draw(jButSprite);
			screen->draw(mButSprite);
		} else { //Draw the join menu
			screen->draw(ipSprite);
			screen->draw(enterIP);
			screen->draw(jButSprite);
			screen->draw(bButSprite);
		}
	}
}
