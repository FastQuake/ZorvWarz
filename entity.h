#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

extern int idCounter;

class Entity{
	public:
		std::string type;
		bool drawable,collides,readyToUpdate;
		bool alive;
		sf::FloatRect *collisionBox; 
		int x,y;
		int ID;

		Entity();
		//virtual ~Entity(){};

		virtual void update(int framecount){};
		virtual void onCollision(Entity object){};
		virtual void draw(sf::RenderWindow *screen){};
};

class EntityManager{
	public:
	std::vector<Entity*> entityList;

	~EntityManager();
	void updateEntities(int framecount);
	void collideEntities();
	void drawEntities(sf::RenderWindow *screen);
	void removeByID(int ID);
};


#endif
