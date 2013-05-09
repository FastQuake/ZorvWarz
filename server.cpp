#include "server.h"
#include "main.h"
#include "ai.h"
#include <sstream>
#include <stdio.h>

using namespace std;

ShipEntity *serverShip;
ENetHost *server;
EntityManager serverEntities;
AIManager aim;
Mob *p1;
Mob *p2;
Mob *testMonster;

bool isp1 = true;
bool twoP = false;

void sendSpawnPackets(ENetPeer *peer);

void initServer(){
	serverShip = new ShipEntity(tilesFile);
	aim.init(serverShip);
	ENetAddress address;
	
	address.host = ENET_HOST_ANY;
	address.port = 1255;

	server = enet_host_create(&address,2,2,0,0);

	if(server == NULL){
		cout << "The server's broken" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "The server is working :)" << endl;

	p1 = new Mob(1);
	p2 = new Mob(2);
	testMonster = new Mob(3);

	//Attach the entites to the server's entity manager
	serverEntities.entityList.push_back(serverShip);
	serverEntities.entityList.push_back(p1);
	serverEntities.entityList.push_back(p2);
	serverEntities.entityList.push_back(testMonster);

	p1->type = "player";
	p2->type = "player";
	testMonster->type = "monster";
	sf::Vector2i floorTile = serverShip->getRandomFloorTile();
	testMonster->x = floorTile.x*32;
	testMonster->y = floorTile.y*32;
}

void serverLoop(){
	stringstream ss;
	ENetEvent event;
	while(!doShutdown){
		while (enet_host_service(server, &event, 10) > 0)
		{
			//cout << "something happen" << endl;
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				cout << "A new client connected from " << event.peer->address.host << event.peer->address.port << "." << endl;
				if(isp1){
					p1->peer = event.peer;	
					isp1 = false;
				} else {
					p2->peer = event.peer;
					twoP = true;
				}
				handlePacket("0",event.peer);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				ss << event.packet->data;
				handlePacket(ss.str(),event.peer);
				ss.str("");
				ss.clear();
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy (event.packet);
        
				break;
       
			case ENET_EVENT_TYPE_DISCONNECT:
				cout << event.peer->data << " disconected." << endl;
				/* Reset the peer's client information. */
				event.peer -> data = NULL;
			}
		}

		//Handle entities
		serverEntities.updateEntities(0);
		serverEntities.collideEntities();
	}
	delete p1;
	delete p2;
}

/** Function to handle all server packets */
void handlePacket(string packetData, ENetPeer *peer){
	stringstream ss;
	string packetTypeStr;
	int packetType;
	ENetPacket *packet;
	string map;

	//Vector to store random tile position
	sf::Vector2i vec;

	ss << packetData;
	ss >> packetType;

	switch(packetType){
	case csLogin:
		cout << "Login packet received." << endl;

		//Send map data to client
		map = getMapData(false);
		cout << "Server map" << endl << getMapData(true) << endl << endl;
		packet = createPacket(scMap,map,ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer,0,packet);
		//get random floor tile
		vec = serverShip->getRandomFloorTile();
		switch(p1Orp2(peer)){
			case 1:
				//Send acknowledge join from client and give client their ID
				packet = createPacket(scJoinack,intToStr(p1->ID),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				ss.str("");
				ss.clear();
				//Give client a random position on map that is a floor tile
				ss << 0 << " " << vec.x*32 << " " << vec.y*32 << " 0";
				packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				//Set player's position and other info in server data
				p1->x = vec.x*32;
				p1->y = vec.y*32;
				p1->peer = peer;
				p1->connected = true;
				//Send the client spawn packets for all entities, and send the other player a spawn packet if they're connected
				sendSpawnPackets(peer);
				if(p2->connected){
					ss.str("");
					ss.clear();
					ss << p1->ID << " " << p1->type << " " << p1->x << " " << p1->y << " " << p1->rot;
					packet = createPacket(scSpawn,ss.str(),ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(p2->peer,0,packet);
					enet_host_flush(server);
				}
				break;
			case 2:
				//Send acknowledge join from client and give client their ID
				packet = createPacket(scJoinack,intToStr(p2->ID),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				ss.str("");
				ss.clear();
				//Give client a random position on map that is a floor tile
				ss << 0 << " " << vec.x*32 << " " << vec.y*32 << " 0";
				packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				//Set player's position and other info in server data
				p2->x = vec.x*32;
				p2->y = vec.y*32;
				p2->peer = peer;
				p2->connected = true;
				//Send the client spawn packets for all entities, and send the other player a spawn packet if they're connected
				sendSpawnPackets(peer);
				if(p1->connected){
					ss.str("");
					ss.clear();
					ss << p2->ID << " " << p2->type << " " << p2->x << " " << p2->y << " " << p2->rot;
					packet = createPacket(scSpawn,ss.str(),ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(p1->peer,0,packet);
					enet_host_flush(server);
				}
				break;
		}
		break;
	case csMove:
		float x;
		float y;
		float rot;
		ss >> x >> y >> rot;
		ss.str("");
		ss.clear();

		switch(p1Orp2(peer)){
			//Store player1 new position and relay it to player2
			case 1:
				p1->x = x;
				p1->y = y;
				p1->rot = rot;
				ss << p1->ID << " " << p1->x << " " << p1->y << " " << rot;
				//cout << ss.str() << endl;
				if(twoP){
					packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(p2->peer,0,packet);
				}
				break;
			//Store player2 new position and relay it to player1
			case 2:
				p2->x = x;
				p2->y = y;
				p2->rot = rot;
				ss << p2->ID << " " << p2->x << " " << p2->y << " " << rot;
				cout << ss.str() << endl;
				packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(p1->peer,0,packet);
				break;
		}
		break;

	default:
		break;

	}
}

/** Get map data as string to send to client **/
string getMapData(bool newlines){
	stringstream ss;
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			ss << serverShip->map->data[x][y];
			if(newlines)
				if(x == dunXSize-1)
					ss << endl;
		}
	}
	
	return ss.str();
}

/** Create ENet Packet using packetTypes and strings instead of unsigned char* **/
ENetPacket *createPacket(int packetType, string packetData, int packetFlag){
	stringstream ss;
	ENetPacket *packet;

	ss << packetType << " " << packetData;
	packet = enet_packet_create(ss.str().c_str(),ss.str().length(),packetFlag);
	return packet;
}

/** Check if peer is player1 or player2 **/
int p1Orp2(ENetPeer *peer){
	if(peer == p1->peer){
		return 1;
	} else {
		return 2;
	}
}

/**Send spawn packets for every entity on the server to the given peer **/
void sendSpawnPackets(ENetPeer *peer){
	Entity *thisEntity;
	ENetPacket *packet;
	stringstream ss;
	for(int i=0;i<serverEntities.entityList.size();i++){
		ss.str("");
		ss.clear();
		thisEntity = serverEntities.entityList[i];
		if((p1Orp2(peer) == 1 && thisEntity == p1) || (p1Orp2(peer) == 2 && thisEntity == p2))
			continue;
		ss << thisEntity->ID << " " << thisEntity->type << " " << thisEntity->x << " " << thisEntity->y << " " << thisEntity->rot;
		cout << ss.str() << endl;
		packet = createPacket(scSpawn,ss.str(),ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer,0,packet);
		enet_host_flush(server);
	}
}
