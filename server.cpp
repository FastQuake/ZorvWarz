#include "server.h"
#include "main.h"
#include <sstream>
#include <stdio.h>

using namespace std;

ShipEntity *serverShip;
ENetHost *server;
EntityManager serverEntities;
Mob p1;
Mob p2;

bool isp1 = true;
bool twoP = false;

void initServer(){
	serverShip = new ShipEntity(tilesFile);
	ENetAddress address;
	
	address.host = ENET_HOST_ANY;
	address.port = 1255;

	server = enet_host_create(&address,2,2,0,0);

	if(server == NULL){
		cout << "The server's broken" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "The server is working :)" << endl;

	//Attach the entites to the server's entity manager
	serverEntities.entityList.push_back(serverShip);
	serverEntities.entityList.push_back(&p1);
	serverEntities.entityList.push_back(&p2);
	//Assign player 1 and 2 their IDs
	p1.ID = 1;
	p2.ID = 2;
}

void serverLoop(){
	stringstream ss;
	ENetEvent event;
	while(!doShutdown){
		while (enet_host_service(server, &event, 1) > 0)
		{
			cout << "something happen" << endl;
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				cout << "A new client connected from " << event.peer->address.host << event.peer->address.port << "." << endl;
				if(isp1){
					p1.peer = event.peer;	
					isp1 = false;
				} else {
					p2.peer = event.peer;
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
		map = getMapData();
		cout << "Server map" << endl << map << endl << endl;
		packet = createPacket(scMap,map,ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer,0,packet);
		//get random floor tile
		vec = serverShip->getRandomFloorTile();
		switch(p1Orp2(peer)){
			case 1:
				//Send ackknowledge join from client and give client their ID
				packet = createPacket(scJoinack,intToStr(p1.ID),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				ss.str("");
				ss.clear();
				//Give client a random position on map that is a floor tile
				ss << 0 << " " << vec.x*32 << " " << vec.y*32 << " 0";
				packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				//Set player's position in server data
				p1.x = vec.x*32;
				p1.y = vec.y*32;
				break;
			case 2:
				//Send ackknowledge join from client and give client their ID
				packet = createPacket(scJoinack,intToStr(p2.ID),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				ss.str("");
				ss.clear();
				//Give client a random position on map that is a floor tile
				ss << 0 << " " << vec.x*32 << " " << vec.y*32 << " 0";
				packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				//Set player's position in server data
				p2.x = vec.x*32;
				p2.y = vec.y*32;
				break;
		}
		break;
	case csMove:
		int x;
		int y;
		int rot;
		ss >> x >> y >> rot;
		ss.str("");
		ss.clear();

		switch(p1Orp2(peer)){
			//Store player1 new position and relay it to player2
			case 1:
				p1.x = x;
				p1.y = y;
				p1.rot = rot;
				ss << p1.ID << " " << p1.x << " " << p1.y << " " << rot;
				if(twoP){
					packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(p2.peer,0,packet);
				}
				break;
			//Store player2 new position and relay it to player1
			case 2:
				p2.x = x;
				p2.y = y;
				p2.rot = rot;
				ss << p2.ID << " " << p2.x << " " << p2.y << " " << rot;
				packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(p1.peer,0,packet);
				break;
		}
		break;

	default:
		break;

	}
}

/** Get map data as string to send to client **/
string getMapData(){
	stringstream ss;
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			ss << serverShip->map->data[x][y];
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
	if(peer == p1.peer){
		return 1;
	} else {
		return 2;
	}
}
