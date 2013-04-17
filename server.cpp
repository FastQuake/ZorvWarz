#include "server.h"
#include "main.h"
#include <sstream>
#include <stdio.h>

using namespace std;

ShipEntity *serverShip;
ENetHost *server;

enum{
	csLogin,
	csMove,
	csAttack
};

enum{
	scSpawn,
	scJoinack,
	scAttack,
	scMove,
	scMap
};

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
}

void serverLoop(){
	
	ENetEvent event;
	while(true){
		while (enet_host_service(server, &event, 1) > 0)
		{
			cout << "something happen" << endl;
			//if(window.isOpen() == false){
			//	break;
			//}
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				cout << "A new client connected from " << event.peer->address.host << event.peer->address.port << "." << endl;
				/* Store any relevant client information here. */
				//event.peer -> data = "Client information";
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf ("A packet of length %u containing %s was received from %s on channel %u.\n",
						event.packet -> dataLength,
						event.packet -> data,
						event.peer -> data,
						event.channelID);
				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy (event.packet);
        
				break;
       
			case ENET_EVENT_TYPE_DISCONNECT:
				printf ("%s disconected.\n", event.peer -> data);
				/* Reset the peer's client information. */
				event.peer -> data = NULL;
			}
		}
	}
}

void handlePacket(string packetData, ENetPeer *peer){
	stringstream ss;
	string packetTypeStr;
	int packetType;
	ENetPacket *packet;
	string map;

	ss << packetData;
	ss >> packetType;

	switch(packetType){
	case csLogin:
		cout << "fuck the you man login aasshole imatroll" << endl;
		map = getMapData();
		packet = enet_packet_create(map.c_str(),map.length()+1,ENET_PACKET_FLAG_RELIABLE);
		break;
	case csMove:
		float x;
		float y;
		float rot;

		ss >> x >> y >> rot;
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
