#include "ai.h"
#include "lighting.h"

Node::Node(sf::FloatRect nodeBox){
	this->nodeBox = nodeBox;

	int x = nodeBox.left+(this->nodeBox.width/2);
	int y = this->nodeBox.top+(this->nodeBox.height/2);
	
	this->hitBox.left = x-(radius);
	this->hitBox.top = y-(radius);
	this->hitBox.width = radius*2;
	this->hitBox.height = radius*2;
}

void Node::findNeighbors(vector<sf::FloatRect> collisionBoxes){
	vector<sf::FloatRect> targetColBoxes;
	for(int i=0;i<collisionBoxes.size();i++){
		if(this->hitBox.intersects(collisionBoxes[i])){
			//cout << "pushing back targetbox " << i << endl;
			targetColBoxes.push_back(collisionBoxes[i]);
		}
	}	

	sf::Vector2f middle(this->nodeBox.left+(this->nodeBox.width/2),
		this->nodeBox.top+(this->nodeBox.height/2));	
	for(int i=0;i<360;i++){
		for(int j=32;j<=radius;j++){
			bool hit = false;
			for(int k=0;k<targetColBoxes.size();k++){
				if(targetColBoxes[k].contains(LightManager::getCirclePoint(j,i,middle))){
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
				if(aim.nodeList[k].nodeBox.contains(LightManager::getCirclePoint(j,i,middle))){
					bool seen = false;
					for(int l=0;l<this->neighbors.size();l++)
						if(this->neighbors[l] == &aim.nodeList[k])
							seen = true;
					if(seen)
						break;
					this->neighbors.push_back(&aim.nodeList[k]);
					sf::Vector2f middle2(aim.nodeList[k].nodeBox.left+(aim.nodeList[k].nodeBox.width/2),
						aim.nodeList[k].nodeBox.top+(aim.nodeList[k].nodeBox.height/2));	
					cout << "pushing back node " << middle2.x << "," << middle2.y << "-" << middle.x << "," << middle.y << endl;
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
		cout << "finding neighbors " << i << "," << nodeList.size() << endl;
		this->nodeList[i].findNeighbors(ship->collisionBoxes);
	}
	int debug = 0;
}