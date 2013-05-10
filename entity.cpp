#include "entity.h" 

int idCounter = 0;

/**
  * Super constructor for Entities that assignes them an ID
  */
Entity::Entity(){
	ID = idCounter;
	idCounter++;
}

/*
EntityManager::~EntityManager(){
	//Delete all alocated entities from the entity list
	for(int i=0;i<entityList.size();i++){
		delete entityList[i];
	}
}*/ // Don't think I acutally need to delete all entities if I pass them by &entity to the list

/**
  * Loop through all entities, update them if they are ready to update
  * Kill the entities if they are not alive anymore
  */
void EntityManager::updateEntities(int framecount){
	//Loop through all entities
	for(int i=0;i<entityList.size();i++){
		//Call entities update funciton only if it is ready to update
		if(entityList[i]->readyToUpdate){
			entityList[i]->update(framecount);
			//if entity is dead kill it! possibly with fire
			if(entityList[i]->alive == false){
				removeByID(entityList[i]->ID);
			}
		}
	}
}

/**
  * Loop through all entities, and if they are collidable check if they had collided
  */
void EntityManager::collideEntities(){
	for(int i=0;i<entityList.size();i++){
		if(entityList[i]->type == "map"){
			continue;
		}
		for(int j=0;j<entityList.size();j++){
			if(j == i){
				continue;
			}
			if(entityList[i]->type == "player" &&
					entityList[j]->type == "player"){
				continue;
			}
			if(entityList[i]->collides && entityList[j]->collides){
				for(int k=0;k<entityList[i]->collisionBoxes.size();k++){
					for(int l=0;l<entityList[j]->collisionBoxes.size();l++){
						if(entityList[i]->collisionBoxes[k].intersects(
									entityList[j]->collisionBoxes[l])){
							//std::cout << "COLLISON on " << entityList[i]->type << std::endl;
							entityList[i]->onCollision(entityList[j],entityList[j]->collisionBoxes[l]);
							entityList[j]->onCollision(entityList[i],entityList[i]->collisionBoxes[k]);
						}
					}
				}
			}
		}
	}
}

/**
  * Loop Through all entities and draw them if they are drawable
  */
void EntityManager::drawEntities(sf::RenderWindow *screen,int screenx,int screeny){
	for(int i=0;i<entityList.size();i++){
		if(entityList[i]->drawable){
			entityList[i]->draw(screen,screenx,screeny);
		}
	}
}

/**
  * Loop through all entities and if the entitiy has the same ID delete it
  */
void EntityManager::removeByID(int ID){
	for(int i=0;i<entityList.size();i++){
		if(entityList[i]->ID == ID){
			delete entityList[i];
			entityList.erase(entityList.begin() + i);
			break;
		}
	}
}

void Entity::collideWall(sf::FloatRect otherBox){
	int xMag = (collisionBoxes[0].left+collisionBoxes[0].width) - (otherBox.left+otherBox.width);
	int yMag = (collisionBoxes[0].top+collisionBoxes[0].height) - (otherBox.top+otherBox.height);
	/*if(xMag == yMag){
		xMag--;
	}*/

	//Because fuck logic
	if(xMag == 26 && yMag == 28){
		xMag = 29;
	}
	if(xMag == 27 && yMag == 30){
		yMag = 26;
	}

	int xx = 0;
	int yy = 0;
	if(xMag != 0){
		xx = xMag/abs(xMag);
	}
	if(yMag != 0){
		yy = yMag/abs(yMag);
	}
	//std::cout << xx << " " << yy << std::endl;
	//std::cout << "VOLS: " << xVol << " " << yVol << std::endl;
	//std::cout << "MAGS: " << xMag << " " << yMag << std::endl;
	while(collisionBoxes[0].intersects(otherBox)){
		if(abs(xMag) > abs(yMag)){
			//std::cout << "DOIN X" << "\n";
			x += xx;
		} else {
			//std::cout << "DOIN Y" << "\n";
			y += yy;
		}
		collisionBoxes[0].left = x;
		collisionBoxes[0].top = y;
	}
	if(abs(xMag) >= abs(yMag)){
		x += xx;
	} else {
		y += yy;
	}
	collisionBoxes[0].left = x;
	collisionBoxes[0].top = y;
}
