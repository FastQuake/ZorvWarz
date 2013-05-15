#include "endgame.h"
#include "menu.h"

sf::Sprite ebgSprite;
sf::Sprite ebButSprite;

sf::Text player1;
sf::Text player2;

sf::Text p1Score;
sf::Text p1Kills;
sf::Text p1HealthUsed;
sf::Text p1ShotsFired;

sf::Text p2Score;
sf::Text p2Kills;
sf::Text p2HealthUsed;
sf::Text p2ShotsFired;

sf::Text totalKills;
sf::Text totalScore;

void initEndScreen(){
	const int p1Column = 100;
	const int p2Column = 400;
	const int spacing = 100;

	ebgSprite.setTexture(bgTexture);
	ebButSprite.setTexture(bButTex);

	player1.setFont(font);
	player1.setOrigin(player1.getLocalBounds().width/2,player1.getLocalBounds().height/2);
	player1.setPosition(p1Column+100,200);
	player1.setString("Player 1");
	player2.setFont(font);
	player2.setOrigin(player2.getLocalBounds().width/2,player2.getLocalBounds().height/2);
	player2.setPosition(p2Column+100,200);
	player2.setString("Player 2");

	p1Score.setFont(font);
	p1Score.setPosition(p1Column,200+spacing*1);
	p1Kills.setFont(font);
	p1Kills.setPosition(p1Column,200+spacing*2);
	p1HealthUsed.setFont(font);
	p1HealthUsed.setPosition(p1Column,200+spacing*3);
	p1ShotsFired.setFont(font);
	p1ShotsFired.setPosition(p1Column,200+spacing*4);

	p2Score.setFont(font);
	p2Score.setPosition(p2Column,200+spacing*1);
	p2Kills.setFont(font);
	p2Kills.setPosition(p2Column,200+spacing*2);
	p2HealthUsed.setFont(font);
	p2HealthUsed.setPosition(p2Column,200+spacing*3);
	p2ShotsFired.setFont(font);
	p2ShotsFired.setPosition(p2Column,200+spacing*4);

	totalKills.setFont(font);
//	totalKills.setOrigin(totalKills.getLocalBounds().width/2,totalKills.getLocalBounds().height/2);
//	totalKills.setPosition(400,450);
	totalScore.setFont(font);
//	totalScore.setOrigin(totalScore.getLocalBounds().width/2,totalKills.getLocalBounds().height/2);
//	totalScore.setPosition(400,500);
}

void drawEndScreen(sf::RenderWindow *screen){
	screen->draw(ebgSprite);
	screen->draw(ebButSprite);
	screen->draw(player1);
	screen->draw(p1Score);
	screen->draw(p1Kills);
	screen->draw(p1HealthUsed);
	screen->draw(p1ShotsFired);

	screen->draw(player2);
	screen->draw(p2Score);
	screen->draw(p2Kills);
	screen->draw(p2HealthUsed);
	screen->draw(p2ShotsFired);
}