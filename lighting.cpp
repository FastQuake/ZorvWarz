#include "main.h"
#include "lighting.h"
#include "math.h"

#define PI 3.14159265

Light::Light(int xx, int yy,int rad){
	x = xx;
	y = yy;
	radius = rad;

	hitBox.left = x-(radius);
	hitBox.top = y-(radius);
	hitBox.width = radius*2;
	hitBox.height = radius*2;
}

void Light::update(){
	hitBox.left = x-(radius);
	hitBox.top = y-(radius);
}

LightManager::LightManager(){
	lightLayer.create(800,600);
	lightMask.create(800,600);
	sLightsLayer.setTexture(lightLayer.getTexture());
	sLightMask.setTexture(lightMask.getTexture());

	lightTexture.loadFromFile(lightFile);
	lights.setTexture(lightTexture);
}

void LightManager::drawLights(sf::RenderWindow *screen,int screenx,int screeny){
	lightLayer.clear();
	lightMask.clear(sf::Color(255,255,255));
	for(int i=0;i<lightList.size();i++){
		vector<sf::FloatRect> targetBoxes;
		//Get list of objects that collide with light
		for(int j=0;j<ship->collisionBox.size();j++){
			if(lightList[i]->hitBox.intersects(ship->collisionBox[j])){
				targetBoxes.push_back(ship->collisionBox[j]);
			}
		}	

		vector<sf::ConvexShape> shadowList;
		for(int j=0;j<targetBoxes.size();j++){
			//find middle of box
			sf::Vector2f middle(targetBoxes[j].left+(targetBoxes[j].width/2),
					targetBoxes[j].top+(targetBoxes[j].height/2));	
			//Get my middle
			sf::Vector2f me(lightList[i]->x,lightList[i]->y);

			middle = middle-me;
			float len = sqrt(pow(middle.x,2)+pow(middle.y,2));
			float angle = atan2(middle.y,middle.x)*180/PI;
			
			if(angle < 0){
				angle = 360+angle;
			}

			//Find left point
			while(targetBoxes[j].contains(getCirclePoint(len,angle,me))){
				angle-=1;	
				//cout << "DOIN LINE" << endl;
			}
			angle += 1;
			sf::Vector2f p1 = getCirclePoint(len,angle,me);
			sf::Vector2f p2 = getCirclePoint(lightList[i]->radius,angle,me);

			//Find right point
			while(targetBoxes[j].contains(getCirclePoint(len,angle,me))){
				angle+=1;
			}
			angle -= 1;
			sf::Vector2f p3 = getCirclePoint(lightList[i]->radius,angle,me);
			sf::Vector2f p4 = getCirclePoint(len,angle,me);

			sf::ConvexShape shadow;
			shadow.setPointCount(4);
			shadow.setPoint(0,p1);
			shadow.setPoint(1,p2);
			shadow.setPoint(2,p3);
			shadow.setPoint(3,p4);

			shadowList.push_back(shadow);

			

			sf::Vector2f bleh2 = getCirclePoint(len,angle,me);
			bleh2.x -= screenx;//-me.x;
			bleh2.y -= screeny;//-me.y;
			
			middle += me;
			middle.x -= screenx;
			middle.y -= screeny;

			me.x -= screenx;
			me.y -= screeny;

			//p1 += me;
			p1.x -= screenx;
			p1.y -= screeny;

			//p4 += me;
			p4.x -= screenx;
			p4.y -= screeny;



			sf::Vertex line[] = {me,p1};
			//screen->draw(line,2,sf::Lines);

			sf::Vertex line2[] = {me,bleh2};
			line2[0].color = sf::Color::Green;
			line2[1].color = sf::Color::Green;
			//screen->draw(line2,2,sf::Lines);
		}

		for(int j=0;j<shadowList.size();j++){
			shadowList[j].move(-screenx,-screeny);
			shadowList[j].setFillColor(sf::Color(10,10,10));
			//lightLayer.draw(shadowList[j]);
			lightMask.draw(shadowList[j]);
		}
		
		lights.setScale(lightList[i]->radius/32,lightList[i]->radius/32);
		lights.setPosition(lightList[i]->x-screenx-lightList[i]->radius/2,
				lightList[i]->y-screeny-lightList[i]->radius/2);
		lightLayer.draw(lights);

	}

	lightLayer.draw(sLightMask,sf::RenderStates(sf::BlendMultiply));

	screen->draw(sLightsLayer,sf::RenderStates(sf::BlendMultiply));
	//screen->draw(lights);
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
