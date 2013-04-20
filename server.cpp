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

void handlePacket(string packetData, ENetPeer *peer);
int p1Orp2(ENetPeer *peer);
ENetPacket *createPacket(int packetType, string packetData, int packetFlag);

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
	serverEntities.entityList.push_back(serverShip);
	serverEntities.entityList.push_back(&p1);
	serverEntities.entityList.push_back(&p2);
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
				/*printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
						event.packet -> dataLength,
						event.packet -> data,
						event.peer -> data,
						event.channelID);*/
				ss << event.packet->data;
				handlePacket(ss.str(),event.peer);
				ss.str("");
				ss.clear();
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy (event.packet);
        
				break;
       
			case ENET_EVENT_TYPE_DISCONNECT:
				printf ("%s disconected.\n", event.peer -> data);
				/* Reset the peer's client information. */
				event.peer -> data = NULL;
			}
		}

		//Handle entities
		serverEntities.updateEntities(0);
		serverEntities.collideEntities();

		//cout << "SERVER XY: " << p1.x << " " << p1.y << endl;
	}
}

void handlePacket(string packetData, ENetPeer *peer){
	stringstream ss;
	string packetTypeStr;
	int packetType;
	ENetPacket *packet;
	string map;

	int x;
	int y;
	int rot;

	sf::Vector2i vec;

	ss << packetData;
	ss >> packetType;

	switch(packetType){
	case csLogin:
		cout << "Login packet received." << endl;
		map = getMapData();
		cout << "Server map" << endl << map << endl << endl;
		packet = createPacket(scMap,map,ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer,0,packet);
		//get random floor tile
		vec = serverShip->getRandomFloorTile();
		switch(p1Orp2(peer)){
			case 1:
				packet = createPacket(scJoinack,intToStr(p1.ID),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				ss.str("");
				ss.clear();
				ss << 0 << " " << vec.x*32 << " " << vec.y*32 << " 0";
				packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				p1.x = vec.x*32;
				p1.y = vec.y*32;
				break;
			case 2:
				packet = createPacket(scJoinack,intToStr(p2.ID),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				ss.str("");
				ss.clear();
				ss << 0 << " " << vec.x*32 << " " << vec.y*32 << " 0";
				packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
				enet_peer_send(peer,0,packet);
				p2.x = vec.x*32;
				p2.y = vec.y*32;
				break;
		}
		break;
	case csMove:
		/*int x;
		int y;
		int rot;*/
		ss >> x >> y >> rot;
		ss.str("");
		ss.clear();

		//cout << "GOT MOVE PACKET WITH " << x << " " << y << endl;
		cout << "PACKET DATA: " << packetData << endl;

		switch(p1Orp2(peer)){
			case 1:
				p1.x = x;
				p1.y = y;
				p1.rot = rot;
				ss << p1.ID << " " << p1.x << " " << p1.y << " " << rot;
				cout << "SENDING SS: " << ss.str() << endl;
				if(twoP){
					packet = createPacket(scMove,ss.str(),ENET_PACKET_FLAG_RELIABLE);
					enet_peer_send(p2.peer,0,packet);
				}
				break;
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

string getMapData(){
	stringstream ss;
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			ss << serverShip->map->data[x][y];
		}
	}
	
	return ss.str();
}

ENetPacket *createPacket(int packetType, string packetData, int packetFlag){
	stringstream ss;
	ENetPacket *packet;

	ss << packetType << " " << packetData;
	packet = enet_packet_create(ss.str().c_str(),ss.str().length(),packetFlag);
	return packet;
}

int p1Orp2(ENetPeer *peer){
	if(peer == p1.peer){
		return 1;
	} else {
		return 2;
	}
}
