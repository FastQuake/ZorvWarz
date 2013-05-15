#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "ship.h"
#include "main.h"
#include "lighting.h"
#include "server.h"
#include "ai.h"
#include "menu.h"
#include "endgame.h"
#include <enet/enet.h>

using namespace std;

EntityManager entities;
sf::RenderWindow window;
ShipEntity *ship;
Player *player;
PMob *player2;

Light *p1Light;
Light *p2Light;
LightManager lm;

sf::Thread *serverThread;
sf::Thread *clientThread;
sf::Mutex packetMutex;
sf::Mutex readyMutex;

sf::Texture bTex;
sf::Font font;

sf::Sprite bulletSprite;
sf::Text bulletText;

sf::Sound bulletSound;
sf::SoundBuffer bulletBuffer;
sf::Sound clickSound;
sf::SoundBuffer clickBuffer;
sf::Sound pickupSound;
sf::SoundBuffer pickupBuffer;

sf::Sound bgSound;
sf::SoundBuffer bgBuffer;

sf::RectangleShape p1Health;

string IPad;

sf::Vector2i mousePos;

sf::Clock frameTime;
int FPS = 60;

sf::Time dt;

vector<string> packetList;

Stats clientStats;

bool twoPlayers = false;
bool singleplayer = false;
bool ready = false;
bool serverReady = true;
bool connecting = false;
bool doShutdown = false;

bool keyUp = false;
bool keyDown = false;
bool keyLeft = false;
bool keyRight = false;
bool mouseRight = false;

int state = 0; //0 = main menu 1=game
int seed = 0;

void clientHandlePacket(string packetDatak, ENetPeer *peer);
void extractMap(string data);

sf::Texture blackBgTex;
sf::Sprite blackBgSprite;

string intToStr(int num){
	stringstream ss;
	ss << num;
	return ss.str();
}

string floatToStr(float num){
	stringstream ss;
	ss << num;
	return ss.str();
}

void replaceChar(string *str,char a, char b){
	for(int i=0;i<str->length();i++){
		if(str->at(i) == a){
			str->at(i) = b;
		}
	}
}

int charToInt(char num){
	int intnum;
	stringstream ss;
	ss << num;
	ss >> intnum;
	return intnum;
}

void killAll(){
	for(int i=0;i<entities.entityList.size();i++){
		Entity *thisEntity = entities.entityList[i];
		if(thisEntity->type == "player" || thisEntity->type == "map")
			continue;
		//entities.removeByID(thisEntity->ID);
		entities.removeByRef(thisEntity);
		i = 0;
	}
}

void addEntities(){
	ship = new ShipEntity(tilesFile, false);
	player = new Player(playerFile);

	entities.entityList.push_back(ship);
	entities.entityList.push_back(player);

	//Lights
	p1Light = new Light(100,100,350,sf::Color::White);
	p1Light->type = "spot";
	p2Light = new Light(100,100,350,sf::Color::White);
	p2Light->type = "spot";
	lm.lightList.push_back(p1Light);
	//lm.lightList.push_back(p2Light);
}

void setup(){
	//Set seed based on time
	seed = time(NULL);
	cout << "seed: " << seed << endl;
	srand(seed);
	addEntities();

	bTex.loadFromFile(bulletFile);
	blackBgTex.loadFromFile("data/textures/black.png");
	blackBgSprite.setTexture(blackBgTex);

	bulletText.setFont(font);
	bulletText.setPosition(650,600-50);

	p1Health.setSize(sf::Vector2f(100,10));
	p1Health.setPosition(10,10);
	p1Health.setFillColor(sf::Color::Red);
	p1Health.setOutlineThickness(1);
	p1Health.setOutlineColor(sf::Color(50,50,50));
	p1Health.setScale(2,2);

	bulletSprite.setTexture(bTex);
	bulletSprite.setScale(10,10);
	bulletSprite.rotate(270);
	bulletSprite.setPosition(600,600-30);

	bulletBuffer.loadFromFile("data/audio/bullet.wav");
	bulletSound.setBuffer(bulletBuffer);

	clickBuffer.loadFromFile("data/audio/click.wav");
	clickSound.setBuffer(clickBuffer);

	pickupBuffer.loadFromFile("data/audio/pickup.wav");
	pickupSound.setBuffer(pickupBuffer);

	bgBuffer.loadFromFile("data/audio/bg.wav");
	bgSound.setBuffer(bgBuffer);
	bgSound.setLoop(true);

	initMenu();
	initEndScreen();
}

void cleanup(){
	doShutdown = true;
	//We want to wait for the threads to shutdown before deleting them
	serverThread->wait();
	clientThread->wait();

	delete ship;
	delete player;
	delete serverThread;
	delete clientThread;

	delete p1Light;
	delete p2Light;
}

int main(int argc, char *argv[]){
	string selection;
	setup();

	if (enet_initialize() != 0){
		cout << "An error occurred while initializing ENet." << endl;
        return EXIT_FAILURE;
	}
    atexit (enet_deinitialize);

	//Threads for client and server networking
	clientThread = new sf::Thread(&runClient,selection);
	serverThread = new sf::Thread(&serverLoop);

	window.create(sf::VideoMode(800,600),"Test");
	window.setFramerateLimit(60);
	sf::Event event;

	//Points to check if the player has moved
	float oldx = 0;
	float oldy = 0;
	float oldrot = 0;
	
	//Games current fps
	int fps = 0;

	//Frame time counter
	sf::Clock counter;
	counter.restart();

	sf::Clock dtClock;
	dtClock.restart();

	//Game's default font
	font.loadFromFile("data/PressStart2P.ttf");

	sf::Text fpsText;
	sf::Text waitingText;
	fpsText.setFont(font);
	waitingText.setFont(font);
	waitingText.setPosition(400,300);

	while(window.isOpen()){
		frameTime.restart();
		//Get current FPS
		if(counter.getElapsedTime().asSeconds() > 1){
			counter.restart();
			//fpsText.setString("FPS: "+intToStr(fps)+" BULLETS: "+intToStr(player->bullets));
			FPS = fps;
			fps = 0;
		} else {
			fps++;
		}
		fpsText.setString("FPS: "+intToStr(FPS)+ "\nXY: "+floatToStr(player->x) + " " + 
			floatToStr(player->y));
		//Get input
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			} 
			if(event.type == sf::Event::MouseMoved){
				mousePos.x = event.mouseMove.x;
				mousePos.y = event.mouseMove.y;
			}
			if(event.type == sf::Event::KeyReleased){
				if(event.key.code == sf::Keyboard::W){
					keyUp = false;
				}
				if(event.key.code == sf::Keyboard::S){
					keyDown = false;
				}
				if(event.key.code == sf::Keyboard::A){
					keyLeft = false;
				}
				if(event.key.code == sf::Keyboard::D){
					keyRight = false;
				}
			}
			if(event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::W){
					keyUp = true;
				}
				if(event.key.code == sf::Keyboard::S){
					keyDown = true;
				}
				if(event.key.code == sf::Keyboard::A){
					keyLeft = true;
				}
				if(event.key.code == sf::Keyboard::D){
					keyRight = true;
				}
				if(event.key.code == sf::Keyboard::Escape){
					window.close();
				}
			}
			if(event.type == sf::Event::MouseButtonPressed &&
					event.mouseButton.button == sf::Mouse::Left){
				mouseRight = true;
			}
			if(event.type == sf::Event::MouseButtonReleased &&
					event.mouseButton.button == sf::Mouse::Left){
				mouseRight = false;
			}
			if(event.type == sf::Event::TextEntered && inputIP){
				if(event.text.unicode == '\b'){
					if(ipText.size() > 0)
						ipText.erase(ipText.size()-1);
				}else if(event.text.unicode != '\r'){
					ipText += event.text.unicode;
				}
			}
		}


		//If main menu state
		if(state == 0){
			updateMenu();
			window.clear();
			drawMenu(&window);
			window.display();
		}

		//If in game state
		else if(state == 1){
			//Check if player has moved, if they did move send create packet with
			//new player location and rotation
			if(player->x != oldx || player->y != oldy ||player->rot != oldrot){
				oldx = player->x;
				oldy = player->y;
				oldrot = player->rot;
				stringstream ss;
				ss << csMove << " " << player->x << " " << player->y << " " << player->rot;
				//cout << "SENDING: " << ss.str() << endl;
				packetMutex.lock();
				packetList.push_back(ss.str());
				packetMutex.unlock();

				ss.str("");
				ss.clear();
			}
			

			//Update all the entities
			if(singleplayer || (!singleplayer && twoPlayers))
				entities.updateEntities(fps,dt.asSeconds());
			entities.collideEntities();

			p1Light->x = player->x+16;
			p1Light->y = player->y+16;
			p1Light->rot = player->rot;
			p1Light->update();

			if(twoPlayers){
				p2Light->x = player2->x+16;
				p2Light->y = player2->y+16;
				p2Light->rot = player2->rot;
				p2Light->update();
			}

			bulletText.setString("x"+intToStr(player->bullets));
			p1Health.setSize(sf::Vector2f(player->health*10,10));

			//draw stuff
			window.clear();
			entities.drawEntities(&window,player->x-400,player->y-300); //Hardcoded screenx and screeny, may fix later
			lm.drawLights(&window,player->x-400,player->y-300);
			aim.drawNet(&window,player->x-400,player->y-300);
			pathMutex.lock();
			//testMonster->drawPath(&window,player->x-400,player->y-300);
			pathMutex.unlock();

			//window.draw(fpsText);
			window.draw(bulletSprite);
			window.draw(bulletText);
			window.draw(p1Health);
			if(!twoPlayers && !singleplayer){
					waitingText.setString("Waiting for Player 2");
					waitingText.setOrigin(waitingText.getGlobalBounds().width/2,
				waitingText.getGlobalBounds().height/2);

					window.draw(blackBgSprite);
					window.draw(waitingText);
				}

			window.draw(fpsText);
			window.display();
		}else if(state == 2){
			updateEndScreen(clientStats);
			window.clear();
			drawEndScreen(&window);
			window.display();
		}

		dt = dtClock.restart();
	}
	cleanup();
	return 0;
}

/** Thread to handle all client networking **/
void runClient(string selection){
	while(true){
		readyMutex.lock();
		if(serverReady)
			break;
		readyMutex.unlock();
	}
	connecting = true;
	cout << "STARTING CLIENT" << endl;
	ENetHost *client;
	ENetPeer *peer;
	ENetAddress address;
	ENetEvent event;

	string packetData;
	stringstream ss;

	enet_address_set_host(&address,IPad.c_str());

	address.port = 1255;
	client = enet_host_create(NULL,1,2,0,0);
	peer = enet_host_connect(client,&address,2,0);

	if (peer == NULL){
		cout <<	"No available peers for initiating an ENet connection." << endl;
		exit (EXIT_FAILURE);
	}

		/* Wait up to 10 seconds for the connection attempt to succeed. */
	if (enet_host_service (client, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT){
		cout << "Connection to " << IPad << " suceeded" << endl;
		connecting = true;
	}
	else{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		connecting = false;
		enet_peer_reset(peer);
		cout << "Connection to " << IPad << " failed" << endl;
		return;
	}

	while(!doShutdown){
		//Loop through all packets and send them to server
		packetMutex.lock();
		for(int i=0;i<packetList.size();i++){
			//cout << "SENDING PACKET: " << packetList[i] << endl;
			ENetPacket *packet;
			packet = enet_packet_create(packetList[i].c_str(),packetList[i].length(),
					ENET_PACKET_FLAG_RELIABLE);
			enet_peer_send(peer,0,packet);
			enet_host_flush(client);
		}
		packetList.clear();
		packetMutex.unlock();
		while(enet_host_service(client,&event,10) > 0){
			//cout << "something happen in da client" << endl;
			switch(event.type){
				case ENET_EVENT_TYPE_RECEIVE:
					ss << event.packet->data;
					clientHandlePacket(ss.str(),peer);
					ss.str("");
					ss.clear();
					break;
			}
		}
	}
}

/** Function to handle all client packet types **/
void clientHandlePacket(string packetData, ENetPeer *peer){
	stringstream ss;
	string type;
	string mapData;
	int packetType;
	int id;

	Entity *guy;

	float x,y,rot;

	ss << packetData;
	ss >> packetType;

	switch(packetType){
		case scSpawn:
			ss >> id;
			ss >> type;
			ss >> x;
			ss >> y;
			ss >> rot;
			if(type == "player"){
				cout << "GOT P2 SPAWN" << endl;
				player2 = new PMob(playerFile, id);
				player2->x = x;
				player2->y = y;
				player2->rot = rot;
				player2->type = "player";
				twoPlayers = true;
				entities.entityList.push_back(player2);
				lm.lightList.push_back(p2Light);
			}else if(type == "box"){
				entities.entityList.push_back(new AmmoBox(x,y));
				entities.entityList.back()->ID = id;
			}else if(type == "hbox"){
				entities.entityList.push_back(new HealthBox(x,y));
				entities.entityList.back()->ID = id;
			}else if (type == "stairs"){
				entities.entityList.push_back(new Stairs(x,y,1));
				entities.entityList.back()->ID = id;
			}else{
				Mob *monster = new Mob(alienFile,id);
				monster->x = x;
				monster->y = y;
				monster->rot = rot;
				monster->type = "monster";
				cout << "SPAWNING MONSTER ID " << monster->ID << endl;
				entities.entityList.push_back(monster);
			}
			break;
		case scDespawn:
			ss >> id;
			cout << "DESPAWNING " << id << endl;
			guy = entities.getByID(id);
			if(guy->type == "monster"){
				guy->health = -1;
			} else {
				entities.removeByID(id);
			}
			break;
		case scJoinack:
			//Get player id and assign it to player
			cout << "I AM NOW JOINED!" << endl;
			ss >> id;
			player->ID = id;
			break;
		case scAttack:
			bulletSound.play();
			entities.entityList.push_back(new Bullet(
						player2->x+16,player2->y+16,player2->rot,2));
			entities.entityList.back()->ID = idCounter + 1000;
			break;
		case scMove:
			//Get player x y rot
			float x,y,rot;
			ss >> id >> x >> y >> rot;
			//If self then move self
			if(id == 0){
				//cout << "MOVIN P1 TO " << x << " " << y << endl;
				player->x = x;
				player->y = y;
				player->rot = rot;
			}else if(twoPlayers && id == player2->ID){ // else move player2 to wherever
				//cout << "MOVING P2 TO " << x << " " << y << endl;
				if(player2->x != x || player2->y != y){
					player2->animTimer.restart();
				}
				player2->x = x;
				player2->y = y;
				player2->rot = rot;
			}else{
				for(int i=0;i<entities.entityList.size();i++){
					if(id == entities.entityList[i]->ID){
						if(entities.entityList[i]->x != x ||
								entities.entityList[i]->y != y){
							entities.entityList[i]->animTimer.restart();
						}
						entities.entityList[i]->x = x;
						entities.entityList[i]->y = y;
						entities.entityList[i]->rot = rot;
						break;
					}
				}
			}
			break;
		case scMap:
			//Get map data and apply it to game map
			enet_peer_timeout(peer,ENET_PEER_TIMEOUT_LIMIT,
					ENET_PEER_TIMEOUT_MINIMUM,ENET_PEER_TIMEOUT_MAXIMUM);
			ss >> mapData;
			extractMap(mapData);
			ready = true;
			bgSound.play();
			break;
		case scChgLvl:
			enet_peer_timeout(peer,100000,100000,100000);
			state = 0;
			loading = true;
			ready = false;
			killAll();
			break;
		case scStats:
			ss >> clientStats.p1Score;
			ss >> clientStats.p1Kills;
			ss >> clientStats.p1HealthUsed;
			ss >> clientStats.p1ShotsFired;
			if(!singleplayer){
				ss >> clientStats.p2Score;
				ss >> clientStats.p2Kills;
				ss >> clientStats.p2HealthUsed;
				ss >> clientStats.p2ShotsFired;
			}
			state = 2;
			break;
	}
}

/** Function to extract map data and assign it to the game map**/
void extractMap(string data){
	replaceChar(&data,',',' ');
	stringstream ss;
	int bleh;
	ss << data;

	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			ss >> bleh;
			ship->map->data[x][y] = bleh;//charToInt(bleh);
		}
	}
	ship->getColBoxes();

	sf::Vector2f pos = ship->getRandomFloorTile();
	player->x = pos.x*32;
	player->y = pos.y*32;
	p1Light->x = player->x;
	p1Light->y = player->y;
	player->update(0,0);
	p1Light->update();

	if(twoPlayers){
		pos = ship->getRandomFloorTile();
		player2->x = pos.x*32;
		player2->y = pos.y*32;
		p2Light->x = player->x;
		p2Light->y = player->y;
		player->update(0,0);
		p2Light->update();
	}
}

vector<sf::FloatRect> whatIntersectsBox(sf::FloatRect hitBox){
	vector<sf::FloatRect> intersectors;

	for(int i=0;i<ship->collisionBoxes.size();i++){
		if(hitBox.intersects(ship->collisionBoxes[i])){
			//cout << "pushing back targetbox " << i << endl;
			intersectors.push_back(ship->collisionBoxes[i]);
		}
	}
	return intersectors;
}
