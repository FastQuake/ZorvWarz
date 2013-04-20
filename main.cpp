#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include "ship.h"
#include "main.h"
#include "server.h"

using namespace std;

EntityManager entities;
sf::RenderWindow window;
//Ship bleh(tilesFile);
ShipEntity *ship;
Player *player;

sf::Thread *serverThread;
sf::Thread *clientThread;
sf::Mutex mapMutex;

bool ready = false;
bool doShutdown = false;

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
	entities.entityList.push_back(ship);
	entities.entityList.push_back(player);
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
}

int main(int argc, char *argv[]){
	string selection;

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
		mapMutex.lock();
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
	
	window.create(sf::VideoMode(800,600),"Test");
	window.setFramerateLimit(60);
	sf::Event event;

	setup();
	mapMutex.unlock();

	while(!ready)
		continue;

	int x = window.getSize().x/2;
	int y = window.getSize().y/2;
	
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
			fpsText.setString(intToStr(fps));
			fps = 0;
		} else {
			fps++;
		}
		//Get input
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			} 
		}
		
		//Update all the entities
		entities.updateEntities(0);
		entities.collideEntities();

		//draw stuff
		window.clear();
		entities.drawEntities(&window,player->x-400,player->y-300); //Hardcoded screenx and screeny, may fix later
		window.draw(fpsText);
		window.display();


	}
	cleanup();
	return 0;
}

void runClient(string selection){
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
		while(enet_host_service(client,&event,33) > 0){
			cout << "something happen in da client" << endl;
			switch(event.type){
				case ENET_EVENT_TYPE_RECEIVE:
					ss << event.packet->data;
					packetData = ss.str().substr(2,string::npos);
					cout << "Client map" << endl << packetData << endl << endl;
					extractMap(packetData);
					ready = true;
					break;
			}
		}
	}
}

void extractMap(string data){
	stringstream ss;
	char bleh;
	ss << data;

	mapMutex.lock();
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			ss >> bleh;
			ship->map->data[x][y] = charToInt(bleh);

			//ss << data.at(x+(dunXSize*y));
			//ss >> ship->map->data[x][y];
			//ss.str("");
		}
	}
	ship->getColBoxes();
	mapMutex.unlock();
}
