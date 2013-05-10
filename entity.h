#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <enet/enet.h>
#include "ship.h"

extern int idCounter;

class Node;

class Entity{
	public:
		std::string type;
		bool drawable,collides,readyToUpdate;
		bool alive;
		std::vector<sf::FloatRect> collisionBoxes;
		float x,y;
		float rot;
		long ID;

		Entity();
		//virtual ~Entity(){};

		void collideWall(sf::FloatRect otherBox);

		virtual void update(int framecount){};
		virtual void onCollision(Entity *object, sf::FloatRect otherBox){};
		virtual void draw(sf::RenderWindow *screen,int screenx,int screeny){};
};

class EntityManager{
	public:
	std::vector<Entity*> entityList;

	//~EntityManager();
	void updateEntities(int framecount);
	void collideEntities();
	void drawEntities(sf::RenderWindow *screen,int screenx,int screeny);
	void removeByID(int ID);
};

class Bullet : public Entity {
	public:
	sf::Vector2f vel;
	sf::Sprite sBullet;

	Bullet(float x, float y, float rot);
	void update(int framecount);
	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen,int screenx,int screeny);
};

class Mob : public Entity {
	public:
	sf::Sprite mobSprite;
	sf::Texture texture;
	ENetPeer *peer;
	bool connected;
	
	Mob(int id);
	Mob(std::string textureFile, int id);
	void update(int framecount);
	void onCollision(Entity *object, sf::FloatRect otherBox){};
	void draw(sf::RenderWindow *window, int screenx,int screeny);
};

class Player : public Entity {
	public:
	float xVol;
	float yVol;
	float speed;
	sf::Sprite playerSprite;
	sf::Sprite gun;
	sf::Texture pTexture;
	sf::Texture gTexture;
	
	Player(std::string playerTexture);
	void update(int framecount);
	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen,int screenx,int screeny);
};

class Monster : public Entity {
	public:
	int xVol;
	int yVol;
	int speed;
	std::vector<Node*> currentPath;
	
	Monster();
	void buildPath(int player);
	void drawPath(sf::RenderWindow *screen, int screenx, int screeny);

	void update(int framecount);
	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen,int screenx,int screeny){};
};

class ShipEntity : public Entity{
	public:
	Ship *map;
	
	ShipEntity(std::string tilesFile);
	~ShipEntity();

	void onCollision(Entity *object, sf::FloatRect otherBox){};
	void draw(sf::RenderWindow *screen, int screenx, int screeny);
	void getColBoxes();
	sf::Vector2i getRandomFloorTile();

};

extern Monster *testMonster;

#endif
