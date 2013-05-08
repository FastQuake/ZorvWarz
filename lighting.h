#ifndef LIGHTING_H
#define LIGHTING_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
using namespace std;

class Light{
	public:
	float x;
	float y;
	int radius;
	sf::FloatRect hitBox;
	sf::Color lightColour;

	Light(float xx,float yy, int rad);
	void update();
};

class LightManager{
	public:
	vector<Light*> lightList;
	sf::RenderTexture lightLayer;
	sf::RenderTexture lightMask;
	sf::RenderTexture allLights;
	sf::Sprite sLightsLayer;
	sf::Sprite sLightMask;
	sf::Sprite sAllLights;
	sf::Sprite lights;
	sf::Texture lightTexture;

	LightManager();
	void drawLights(sf::RenderWindow *screen,int screenx, int screeny);
	static sf::Vector2f getCirclePoint(float radius,float angle,sf::Vector2f relPoint);

	private:
	bool checkBoxes(vector<sf::FloatRect>, sf::Vector2f point);
};


#endif
