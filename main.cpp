#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include "ship.h"
#include "main.h"
#include "lighting.h"
#include "server.h"
#include "ai.h"

using namespace std;

EntityManager entities;
sf::RenderWindow window;
ShipEntity *ship;
Player *player;
Mob *player2;

Light *p1Light;
Light *p2Light;
LightManager lm;

AIManager aim;

sf::Thread *serverThread;
sf::Thread *clientThread;
sf::Mutex packetMutex;
sf::Mutex readyMutex;

vector<string> packetList;

bool ready = false;
bool doShutdown = false;

bool keyUp = false;
bool keyDown = false;
bool keyLeft = false;
bool keyRight = false;

void clientHandlePacket(string packetData);
void extractMap(string data);

string intToStr(int num){
	stringstream ss;
	ss << num;
	return ss.str();
} 

int charToInt(char num){
	int intnum;
	stringstream ss;
	ss << num;
	ss >> intnum;
	return intnum;
}

void addEntities(){
	ship = new ShipEntity(tilesFile);
	player = new Player(playerFile);
	player2 = new Mob(playerFile);
	player2->type = "player";
	entities.entityList.push_back(ship);
	entities.entityList.push_back(player);
	entities.entityList.push_back(player2);

	//Lights
	p1Light = new Light(100,100,300);
	p2Light = new Light(100,100,300);
	lm.lightList.push_back(p1Light);
	lm.lightList.push_back(p2Light);
}

void setup(){
	//Set seed based on time
	int seed = time(NULL);
	cout << "seed: " << seed << endl;
	srand(seed);
	addEntities();
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

	while(true){
		string ipAddress;

		cout << "1. Join game" << endl << "2. Host game" << endl << "Enter your selection: ";
		cin >> selection;
		clientThread = new sf::Thread(&runClient,selection);
		serverThread = new sf::Thread(&serverLoop);
		//mapMutex.lock();
		if(selection == "1"){
			clientThread->launch();
			break;
		}
		else if(selection == "2"){
			initServer();
			serverThread->launch();
			clientThread->launch();
			break;
		}else{
			cout << "Invalid selection, please try again." << endl << endl;
			continue;
		}
	}

	sf::sleep(sf::milliseconds(500));
	readyMutex.lock();

	window.create(sf::VideoMode(800,600),"Test");
	window.setFramerateLimit(60);
	sf::Event event;

	int oldx = 0;
	int oldy = 0;
	int oldrot = 0;
	
	int fps = 0;
	sf::Clock counter;
	counter.restart();

	sf::Font font;
	font.loadFromFile("data/PressStart2P.ttf");

	sf::Text fpsText;
	fpsText.setFont(font);

	while(window.isOpen()){
		//Get current FPS
		if(counter.getElapsedTime().asSeconds() > 1){
			counter.restart();
			fpsText.setString(intToStr(fps) + " " + intToStr(player->x) + " " + intToStr(player->y));
			fps = 0;
		} else {
			fps++;
		}
		//Get input
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
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
		}

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

		
		p1Light->x = player->x+16;
		p1Light->y = player->y+16;
		p1Light->update();

		p2Light->x = player2->x+16;
		p2Light->y = player2->y+16;
		p2Light->update();

		//Update all the entities
		entities.updateEntities(0);
		entities.collideEntities();

		//draw stuff
		window.clear();
		entities.drawEntities(&window,player->x-400,player->y-300); //Hardcoded screenx and screeny, may fix later
		lm.drawLights(&window,player->x-400,player->y-300);
		aim.drawNet(&window,player->x-400,player->y-300);
		window.draw(fpsText);
		window.display();
	}
	cleanup();
	return 0;
}

/** Thread to handle all client networking **/
void runClient(string selection){
	readyMutex.lock();
	ENetHost *client;
	ENetPeer *peer;
	ENetAddress address;
	ENetEvent event;

	string ipAddress;
	string packetData;
	stringstream ss;

	if(selection == "1"){
		cout << "Enter the IP address: ";
		//cin >> ipAddress;
		ipAddress = "127.0.0.1";
		enet_address_set_host(&address,ipAddress.c_str());
	}
	else if(selection == "2"){
		ipAddress = "127.0.0.1";
		enet_address_set_host(&address,ipAddress.c_str());
	}

	address.port = 1255;
	client = enet_host_create(NULL,1,2,0,0);
	peer = enet_host_connect(client,&address,2,0);

	if (peer == NULL){
		cout <<	"No available peers for initiating an ENet connection." << endl;
		exit (EXIT_FAILURE);
	}

		/* Wait up to 10 seconds for the connection attempt to succeed. */
	if (enet_host_service (client, &event, 10000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT){
		cout << "Connection to some.server.net:1234 succeeded." << endl;
	}
	else{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset (peer);
		cout << "Connection to some.server.net:1234 failed." << endl;
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
		while(enet_host_service(client,&event,1000) > 0){
			//cout << "something happen in da client" << endl;
			switch(event.type){
				case ENET_EVENT_TYPE_RECEIVE:
					ss << event.packet->data;
					clientHandlePacket(ss.str());
					ss.str("");
					ss.clear();
					break;
			}
		}
	}
}

/** Function to handle all client packet types **/
void clientHandlePacket(string packetData){
	stringstream ss;
	int packetType;
	int id;

	ss << packetData;
	ss >> packetType;

	switch(packetType){
		case scSpawn:
			break;
		case scJoinack:
			//Get player id and assign it to player
			ss >> id;
			player->ID = id;
			break;
		case scAttack:
			break;
		case scMove:
			//Get player x y rot
			int x,y,rot;
			ss >> id >> x >> y >> rot;
			//If self then move self
			if(id == 0){
				//cout << "MOVIN P1 TO " << x << " " << y << endl;
				player->x = x;
				player->y = y;
				player->rot = rot;
			} else { // else move player2 to wherever
				//cout << "MOVING P2 TO " << x << " " << y << endl;
				player2->x = x;
				player2->y = y;
				player2->rot = rot;
			}
			break;
		case scMap:
			//Get map data and apply it to game map
			string mapData;
			ss >> mapData;
			extractMap(mapData);
			readyMutex.unlock();
			break;
	}
}

/** Function to extract map data and assign it to the game map**/
void extractMap(string data){
	stringstream ss;
	char bleh;
	ss << data;

	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			ss >> bleh;
			ship->map->data[x][y] = charToInt(bleh);
		}
	}
	ship->getColBoxes();
	aim.init(ship);
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