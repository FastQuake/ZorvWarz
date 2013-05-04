#include "ai.h"
#include "lighting.h"
#include "main.h"

sf::Mutex neighborMutex;

Node::Node(sf::FloatRect nodeBox){
	this->nodeBox = nodeBox;

	int x = this->nodeBox.left+(this->nodeBox.width/2);
	int y = this->nodeBox.top+(this->nodeBox.height/2);
	
	this->hitBox.left = x-(radius);
	this->hitBox.top = y-(radius);
	this->hitBox.width = radius*2;
	this->hitBox.height = radius*2;
	this->middle = sf::Vector2f(x,y);
}

void Node::findNeighbors(threadArgs args){
	Node *thisNode = args.thisNode;
	vector<sf::FloatRect> collisionBoxes = args.collisionBoxes;
	int startAngle = args.startAngle;
	int endAngle = args.endAngle;

	vector<sf::FloatRect> targetColBoxes;
	for(int i=0;i<collisionBoxes.size();i++){
		if(thisNode->hitBox.intersects(collisionBoxes[i])){
			//cout << "pushing back targetbox " << i << endl;
			targetColBoxes.push_back(collisionBoxes[i]);
		}
	}

	for(int i=startAngle;i<endAngle;i++){
		for(int j=32;j<=radius;j++){
			bool hit = false;
			for(int k=0;k<targetColBoxes.size();k++){
				if(targetColBoxes[k].contains(LightManager::getCirclePoint(j,i,thisNode->middle))){
					hit = true;
					break;
					//cout << "hit" << endl;
				}
			}
			if(hit){
				//cout << "hitbreak" << endl;
				break;
			}
			for(int k=0;k<aim.nodeList.size();k++)
			{
				if(aim.nodeList[k].nodeBox.contains(LightManager::getCirclePoint(j,i,thisNode->middle))){
					bool seen = false;
					for(int l=0;l<thisNode->neighbors.size();l++)
						if(thisNode->neighbors[l] == &aim.nodeList[k])
							seen = true;
					if(seen)
						break;
					sf::Vector2f middle2(aim.nodeList[k].nodeBox.left+(aim.nodeList[k].nodeBox.width/2),
						aim.nodeList[k].nodeBox.top+(aim.nodeList[k].nodeBox.height/2));	
					cout << "pushing back node " << middle2.x << "," << middle2.y << "-" << thisNode->middle.x << "," << thisNode->middle.y << endl;
					neighborMutex.lock();
					thisNode->neighbors.push_back(&aim.nodeList[k]);
					neighborMutex.unlock();
					break;
				}
			}
		}
	}
}

AIManager::AIManager(){
}

void AIManager::init(ShipEntity *ship){
	int **level = ship->map->data;
	for(int y=0;y<dunYSize;y++){
		for(int x=0;x<dunXSize;x++){
			if(level[x][y] == NODE){
				cout << "pushing back to nodeList " << x << "," << y << endl;
				this->nodeList.push_back(Node(sf::FloatRect(x*32,y*32,32,32)));
			}
		}
	}
	for(int i=0;i<this->nodeList.size();i++){
		cout << "finding neighbors " << i << "/" << nodeList.size() << endl;
		Node::threadArgs thread1Args;
		thread1Args.collisionBoxes = ship->collisionBoxes;
		thread1Args.startAngle = 0;
		thread1Args.endAngle = 120;
		thread1Args.thisNode = &this->nodeList[i];

		Node::threadArgs thread2Args = thread1Args;
		thread2Args.startAngle = 120;
		thread2Args.endAngle = 240;

		Node::threadArgs thread3Args = thread1Args;
		thread3Args.startAngle = 240;
		thread3Args.endAngle = 360;

		sf::Thread *thread1 = new sf::Thread(&Node::findNeighbors,thread1Args);
		sf::Thread *thread2 = new sf::Thread(&Node::findNeighbors,thread2Args);
		sf::Thread *thread3 = new sf::Thread(&Node::findNeighbors,thread3Args);
		thread1->launch();
		thread2->launch();
		thread3->launch();

		thread1->wait();
		thread2->wait();
		thread3->wait();
	}
}

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