#ifndef ENTITY_H
#define ENTITY_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <enet/enet.h>
#include "ship.h"

extern int idCounter;
extern bool ready;

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
		int health;
		sf::Clock animTimer;

		Entity();
		//virtual ~Entity(){};

		void collideWall(sf::FloatRect otherBox);

		virtual void update(int framecount, float dTime){};
		virtual void onCollision(Entity *object, sf::FloatRect otherBox){};
		virtual void draw(sf::RenderWindow *screen,int screenx,int screeny){};
};

class EntityManager{
	public:
	std::vector<Entity*> entityList;

	//~EntityManager();
	void updateEntities(int framecount, float dTime);
	void collideEntities();
	void drawEntities(sf::RenderWindow *screen,int screenx,int screeny);
	void removeByID(int ID);
	void removeByRef(Entity *ent);
	Entity* getByID(int ID);
};

class Bullet : public Entity {
	public:
	sf::Vector2f vel;
	sf::Sprite sBullet;
	int playerFired;

	Bullet(float x, float y, float rot, int playerFired);
	void update(int framecount,float dTime);
	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen,int screenx,int screeny);
};

class Mob : public Entity {
	public:
	int frame;
	int state;
	sf::Sprite mobSprite;
	sf::Texture texture;
	ENetPeer *peer;
	bool connected;
	
	Mob(int id);
	Mob(std::string textureFile, int id);
	void update(int framecount, float dTime);
	void onCollision(Entity *object, sf::FloatRect otherBox){};
	virtual void draw(sf::RenderWindow *window, int screenx,int screeny);
};

class PMob : public Mob {
	public:
	sf::Texture gTex;
	sf::Sprite gSprite;
	
	PMob(std::string textureFile, int id);
	//void update(int framecount);
	//void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *window, int screenx, int screeny);
};

class Player : public Entity {
	public:
	float xVel;
	float yVel;
	float speed;
	int bullets;
	int frame;
	int state;
	sf::Clock dTimer;
	sf::Sprite playerSprite;
	sf::Sprite gun;
	sf::Texture pTexture;
	sf::Texture gTexture;
	
	Player(std::string playerTexture);
	void update(int framecount, float dTime);
	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen,int screenx,int screeny);
};

class Monster : public Entity {
public:
	float xVel;
	float yVel;
	float speed;
	int targetNodeNum;
	int targetPlayer;
	bool atEnd;
	std::vector<Node*> currentPath;
	sf::Clock pathTimer;
	
	Monster();
	void buildPath();
	void stepPath(Node* currentNode, float dTime);
	void drawPath(sf::RenderWindow *screen, int screenx, int screeny);

	void update(int framecount,float dTime);
	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen,int screenx,int screeny){};
private:
	void stepTowards(sf::Vector2f targetPos, float dTime);
};

class ShipEntity : public Entity{
	public:
	Ship *map;
	
	ShipEntity(std::string tilesFile, bool serv);
	~ShipEntity();

	void onCollision(Entity *object, sf::FloatRect otherBox){};
	void draw(sf::RenderWindow *screen, int screenx, int screeny);
	void getColBoxes();
	sf::Vector2f getRandomFloorTile();

};

class AmmoBox : public Entity {
	public:
	sf::Texture ammoTex;
	sf::Sprite box;
	
	AmmoBox(float x, float y);

	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen, int screenx, int screeny);
};

class HealthBox : public Entity{
	public:
	sf::Texture hBoxTex;
	sf::Sprite hBoxSprite;

	HealthBox(float x, float y);
	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen, int screenx, int screeny);
};

class Stairs : public Entity {
	public:
	int sType;
	sf::Texture sTex;
	sf::Sprite sSprite;

	Stairs(float x, float y, int type);

	void updates(int framecount, float dTime){};
	void onCollision(Entity *object, sf::FloatRect otherBox);
	void draw(sf::RenderWindow *screen, int screenx, int screeny);
};

#endif
