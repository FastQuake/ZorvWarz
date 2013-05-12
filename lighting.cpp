#include "main.h"
#include "lighting.h"
#include <math.h>

sf::ConvexShape shadowList[512];

Light::Light(float xx, float yy,int rad, sf::Color col){
	x = xx;
	y = yy;
	radius = rad;

	hitBox.left = x-(radius);
	hitBox.top = y-(radius);
	hitBox.width = radius*2;
	hitBox.height = radius*2;

	lightColour = col;
}

void Light::update(){
	hitBox.left = x-(radius);
	hitBox.top = y-(radius);
}

LightManager::LightManager(){
	lightLayer.create(800,600);
	lightMask.create(800,600);
	allLights.create(800,600);
	sLightsLayer.setTexture(lightLayer.getTexture());
	sLightMask.setTexture(lightMask.getTexture());
	sAllLights.setTexture(allLights.getTexture());

	lightTexture.loadFromFile(lightFile);
	lights.setTexture(lightTexture);
}

void LightManager::drawLights(sf::RenderWindow *screen,int screenx,int screeny){
	lightLayer.clear();
	lightMask.clear(sf::Color(255,255,255));
	allLights.clear();
	int shadowSize = 0;
	for(int i=0;i<lightList.size();i++){
		//cout << "Drawing light " << i << endl;
		vector<sf::FloatRect> targetBoxes;
		//Get list of objects that collide with light
		targetBoxes = whatIntersectsBox(lightList[i]->hitBox);

		shadowSize = 0;
		for(int j=0;j<targetBoxes.size();j++){
			//find middle of box
			sf::Vector2f middle(targetBoxes[j].left+(targetBoxes[j].width/2),
					targetBoxes[j].top+(targetBoxes[j].height/2));	
			//Get my middle
			sf::Vector2f me(lightList[i]->x,lightList[i]->y);

			middle = middle-me;
			float len = sqrt(pow(middle.x,2)+pow(middle.y,2));
			float angle = atan2(middle.y,middle.x)*180/PI;
			
			//Find right point
			while(targetBoxes[j].contains(getCirclePoint(len,angle,me))){
				angle-=1;	
			}
			angle += 1;
			sf::Vector2f p1 = getCirclePoint(len,angle,me);
			sf::Vector2f p2 = getCirclePoint(lightList[i]->radius,angle-1,me);

			//Find left point
			while(targetBoxes[j].contains(getCirclePoint(len,angle,me))){
				angle+=1;
			}
			angle -= 1;
			sf::Vector2f p3 = getCirclePoint(lightList[i]->radius,angle+1,me);
			sf::Vector2f p4 = getCirclePoint(len,angle,me);

			sf::ConvexShape shadow;
			shadow.setPointCount(4);
			shadow.setPoint(0,p1);
			shadow.setPoint(1,p2);
			shadow.setPoint(2,p3);
			shadow.setPoint(3,p4);

			shadowList[j] = shadow;
			shadowSize++;
		}

		lightMask.clear(sf::Color::White);
		lightLayer.clear();

		for(int j=0;j<shadowSize;j++){
			shadowList[j].move(-screenx,-screeny);
			shadowList[j].setFillColor(sf::Color(10,10,10));
			shadowList[j].setOutlineThickness(0.5);
			shadowList[j].setOutlineColor(sf::Color(10,10,10));
			//lightLayer.draw(shadowList[j]);
			lightMask.draw(shadowList[j]);
		}

		for(int j=0;j<targetBoxes.size();j++){
			sf::RectangleShape rect(sf::Vector2f(targetBoxes[j].width,targetBoxes[j].height));
			rect.setPosition(targetBoxes[j].left-screenx,targetBoxes[j].top-screeny);
			rect.setFillColor(sf::Color(100,100,100));
			lightMask.draw(rect);
		}
		
		lights.setScale(lightList[i]->radius/32,lightList[i]->radius/32);
		sf::Vector2f fakePos(lightList[i]->x-screenx-lightList[i]->radius/2,
				lightList[i]->y-screeny-lightList[i]->radius/2);
		lights.setPosition(fakePos);
		lights.setColor(lightList[i]->lightColour);
		lightLayer.draw(lights);

		lightMask.display();
		lightLayer.draw(sLightMask,sf::RenderStates(sf::BlendMultiply));

		lightLayer.display();
		allLights.draw(sLightsLayer,sf::RenderStates(sf::BlendAdd));
	}

	//lightMask.display();
	//lightLayer.draw(sLightMask,sf::RenderStates(sf::BlendMultiply));
	//lightLayer.draw(sLightMask);
	//lightLayer.display();
	allLights.display();

	screen->draw(sAllLights,sf::RenderStates(sf::BlendMultiply));

	//screen->draw(sLightsLayer,sf::RenderStates(sf::BlendMultiply));
	//screen->draw(sLightMask);
	//screen->draw(sLightsLayer);
}

sf::Vector2f LightManager::getCirclePoint(float radius, float angle,sf::Vector2f relPoint){
	sf::Vector2f out;
	float radians = angle*(PI/180); 
	out.x = cos(radians)*radius;
	out.y = sin(radians)*radius;
	out += relPoint;
	return out;
}

bool LightManager::checkBoxes(vector<sf::FloatRect> boxes, sf::Vector2f point){
	for(int i=0;i<boxes.size();i++){
		if(boxes[i].contains(point)){
			return true;
		}
	}
	return false;
}
