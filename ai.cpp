#include "ai.h"
#include "lighting.h"
#include "main.h"
#include "server.h"
#include <math.h>
#include <sstream>

Node::Node(sf::FloatRect nodeBox){
	this->nodeBox = nodeBox;

	int x = this->nodeBox.left+(this->nodeBox.width/2);
	int y = this->nodeBox.top+(this->nodeBox.height/2);
	this->middle = sf::Vector2f(x,y);
}

void Node::findNeighbors(threadArgs args){
	Node *thisNode = args.thisNode;
	vector<sf::FloatRect> targetColBoxes = args.collisionBoxes;

	for(int i=0;i<aim.nodeList.size();i++){
		float angle = atan2(aim.nodeList[i].middle.y-thisNode->middle.y,aim.nodeList[i].middle.x-thisNode->middle.x)*180/PI;
		//cout << "angle: " << angle << endl;
		bool hit = false;
		bool hitNode = false;
		for(int j=16;;j+=8){
			//cout << "radius: " << j << endl;
			for(int k=0;k<targetColBoxes.size();k++){	//Loop through all the collision objects
														//and check if they're in the path of the ray
				if(targetColBoxes[k].contains(LightManager::getCirclePoint(j,angle,thisNode->middle))){
					hit = true;
					break;
				}
			}
			if(hit){ //If there was an object blocking the "ray"
				//cout << "hitbreak" << endl;
				break;
			}
			//Confirm that the node was actually reached
			for(int k=0;k<aim.nodeList.size();k++){
				if(&aim.nodeList[k] == thisNode)
					continue;
				if(aim.nodeList[k].nodeBox.contains(LightManager::getCirclePoint(j,angle,thisNode->middle))){
					/*sf::Vector2f middle2(aim.nodeList[i].nodeBox.left+(aim.nodeList[i].nodeBox.width/2),
						aim.nodeList[i].nodeBox.top+(aim.nodeList[i].nodeBox.height/2));	
					cout << "pushing back node " << middle2.x << "," << middle2.y << "-" << thisNode->middle.x << "," << thisNode->middle.y << endl;*/
					thisNode->neighbors.push_back(&aim.nodeList[k]);
					hitNode = true;
					break;
				}
			}
			if(hitNode)
				break;
		}
	}
}

AIManager::AIManager(){ //This constructor does nothing. It's just here so that the object can be declared in a wide scope
						//as it can't be initialized until other stuff is initialized first.
}

void AIManager::init(ShipEntity *ship){ //This actually initializes the AI Manager
	//Thread objects
	sf::Thread *thread1;
	sf::Thread *thread2;
	sf::Thread *thread3;
	sf::Thread *thread4;

	int **level = ship->map->data;
	//Get all the nodes, put them in a vector
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			if(level[x][y] == NODE){
				cout << "pushing back to nodeList " << x << "," << y << endl;
				this->nodeList.push_back(Node(sf::FloatRect(x*32,y*32,32,32)));
			}
		}
	}
	bool first = true;
	for(int i=0;i<this->nodeList.size();i){
		//We need a struct to contain arguments, as SFML threading only allows you to give one argument to the function
		Node::threadArgs thread1Args;
		thread1Args.collisionBoxes = ship->collisionBoxes;
		//Initialize other thread argument objects with the same data
		Node::threadArgs thread2Args = thread1Args;
		Node::threadArgs thread3Args = thread1Args;
		Node::threadArgs thread4Args = thread1Args;
		if(first){	//Start the threads for the first time, only do this once as afterwards, 
					//threads will be restarted when they're finished.
			thread1Args.thisNode = &this->nodeList[i];
			thread1 = new sf::Thread(&Node::findNeighbors,thread1Args);
			cout << "finding neighbors1 " << i+1 << "/" << nodeList.size() << endl;
			thread1->launch();
			//i++ increments the node ID, so every thread works on a different node.
			//The thread's argument obejct is modified with the new node each time.
			i++;
			if(i<this->nodeList.size()){
				thread2Args.thisNode = &this->nodeList[i];
				thread2Args.collisionBoxes = ship->collisionBoxes;
				thread2 = new sf::Thread(&Node::findNeighbors,thread2Args);
				cout << "finding neighbors2 " << i+1 << "/" << nodeList.size() << endl;
				thread2->launch();
			}
			i++;
			if(i<this->nodeList.size()){
				thread3Args.thisNode = &this->nodeList[i];
				thread3Args.collisionBoxes = ship->collisionBoxes;
				thread3 = new sf::Thread(&Node::findNeighbors,thread3Args);		
				cout << "finding neighbors3 " << i+1 << "/" << nodeList.size() << endl;
				thread3->launch();
			}
			i++;
			if(i<this->nodeList.size()){
				thread4Args.thisNode = &this->nodeList[i];
				thread4Args.collisionBoxes = ship->collisionBoxes;
				thread4 = new sf::Thread(&Node::findNeighbors,thread4Args);		
				cout << "finding neighbors4 " << i+1 << "/" << nodeList.size() << endl;
				thread4->launch();
			}
		}
		first = false;

		thread1->wait(); //Wait for thread to end
		i++;	//Increment node number, and then start it again with that node number
				//Same goes for all other threads, which is why the code below is repetitive
		if(i<this->nodeList.size()){ 
			thread1Args.thisNode = &this->nodeList[i];
			thread1Args.collisionBoxes = ship->collisionBoxes;
			//cout << "Thread 1 finished; starting again." << endl;
			cout << "finding neighbors1 " << i+1 << "/" << nodeList.size() << endl;
			delete thread1; //Delete it, because it's using an old argument object
			thread1 = new sf::Thread(&Node::findNeighbors,thread1Args); //Recreate it with new arguments
			thread1->launch();
		}
		thread2->wait();
		i++;
		if(i<this->nodeList.size()){
			thread2Args.thisNode = &this->nodeList[i];
			thread2Args.collisionBoxes = ship->collisionBoxes;
			//cout << "Thread 2 finished; starting again." << endl;
			cout << "finding neighbors2 " << i+1 << "/" << nodeList.size() << endl;
			delete thread2;
			thread2 = new sf::Thread(&Node::findNeighbors,thread2Args);	
			thread2->launch();
		}
		thread3->wait();
		i++;
		if(i<this->nodeList.size()){
			thread3Args.thisNode = &this->nodeList[i];
			thread3Args.collisionBoxes = ship->collisionBoxes;
			//cout << "Thread 3 finished; starting again." << endl;
			cout << "finding neighbors3 " << i+1 << "/" << nodeList.size() << endl;
			delete thread3;
			thread3 = new sf::Thread(&Node::findNeighbors,thread3Args);	
			thread3->launch();
		}
		thread4->wait();
		i++;
		if(i<this->nodeList.size()){
			thread4Args.thisNode = &this->nodeList[i];
			thread4Args.collisionBoxes = ship->collisionBoxes;
			//cout << "Thread 4 finished; starting again." << endl;
			cout << "finding neighbors4 " << i+1 << "/" << nodeList.size() << endl;
			delete thread4;
			thread4 = new sf::Thread(&Node::findNeighbors,thread4Args);	
			thread4->launch();
		}
	}
}

/**
Draws the node net; useful for debugging.
*/
void AIManager::drawNet(sf::RenderWindow *screen, int screenx, int screeny){
	sf::Vector2f p1;
	sf::Vector2f p2;
	for(int i=0;i<this->nodeList.size();i++){
		p1 = this->nodeList[i].middle;
		p1.x -= screenx;
		p1.y -= screeny;
		for(int j=0;j<this->nodeList[i].neighbors.size();j++){
			p2 = this->nodeList[i].neighbors[j]->middle;
			p2.x -= screenx;
			p2.y -= screeny;
			sf::Vertex line[] = {p1,p2};
			line[0].color = sf::Color::Green;
			line[1].color = sf::Color::Green;
			screen->draw(line,2,sf::Lines);
		}
	}
}

Node *AIManager::findVisibleNode(sf::Vector2f relPoint, vector<sf::FloatRect> targetColBoxes){	
	for(int i=0;i<aim.nodeList.size();i++){
		float angle = atan2(aim.nodeList[i].middle.y-relPoint.y,aim.nodeList[i].middle.x-relPoint.x)*180/PI;
		//cout << "angle: " << angle << endl;
		bool hit = false;
		for(int j=16;;j+=16){
			//cout << "radius: " << j << endl;
			for(int k=0;k<targetColBoxes.size();k++){	//Loop through all the collision objects
														//and check if they're in the path of the ray
				if(targetColBoxes[k].contains(LightManager::getCirclePoint(j,angle,relPoint))){
					hit = true;
					break;
				}
			}
			if(hit){ //If there was an object blocking the "ray"
				//cout << "hitbreak" << endl;
				break;
			}
			//Confirm that the node was actually reached
			if(aim.nodeList[i].nodeBox.contains(LightManager::getCirclePoint(j,angle,relPoint))){
				/*sf::Vector2f middle2(aim.nodeList[i].nodeBox.left+(aim.nodeList[i].nodeBox.width/2),
					aim.nodeList[i].nodeBox.top+(aim.nodeList[i].nodeBox.height/2));	
				cout << "pushing back node " << middle2.x << "," << middle2.y << "-" << thisNode->middle.x << "," << thisNode->middle.y << endl;*/
				return &aim.nodeList[i];
			}
		}
	}
}

void AIManager::spawnMonsters(vector<Entity*> *entityList, int numMonsters){
	stringstream ss;
	for(int i=0;i<numMonsters;i++){
		sf::Vector2f floorTile = serverShip->getRandomFloorTile();
		Monster *newMonster = new Monster();
		newMonster->type = "monster";
		newMonster->x = floorTile.x*32.0f;
		newMonster->y = floorTile.y*32.0f;
		ss.str("");
		ss.clear();
		ss << newMonster->ID << " " << newMonster->type << " " << newMonster->x << " " << newMonster->y << " " << newMonster->rot;
		ENetPacket *spawnPacket = createPacket(scSpawn,ss.str(),ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(p1->peer,0,spawnPacket);
		if(p2->connected)
			enet_peer_send(p2->peer,0,spawnPacket);
		newMonster->buildPath();
		newMonster->pathTimer.restart();
		entityList->push_back(newMonster);
	}
}
