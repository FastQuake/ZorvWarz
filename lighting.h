#ifndef LIGHTING_H
#define LIGHTING_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

class Light{
	public:
	int x;
	int y;
	int radius;
	sf::FloatRect hitBox;
	sf::Color lightColour;

	Light(int xx,int yy, int rad);
	void update();
};

class LightManager{
	public:
	vector<Light*> lightList;
	sf::RenderTexture lightLayer;
	sf::RenderTexture lightMask;
	sf::Sprite sLightsLayer;
	sf::Sprite sLightMask;
	sf::Sprite lights;
	sf::Texture lightTexture;

	LightManager();
	void drawLights(sf::RenderWindow *screen,int screenx, int screeny);
	private:
	sf::Vector2f getCirclePoint(float radius,float angle,sf::Vector2f relPoint);
	bool checkBoxes(vector<sf::FloatRect>, sf::Vector2f point);
};


#endif
