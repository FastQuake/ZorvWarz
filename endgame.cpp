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
	ebgSprite.setTexture(bgTexture);
	ebButSprite.setTexture(bButTex);

	player1.setFont(font);
	player1.setString("Player 1");
	player2.setFont(font);
	player2.setString("Player 2");

	p1Score.setFont(font);
	p1Kills.setFont(font);
	p1HealthUsed.setFont(font);
	p1ShotsFired.setFont(font);

	p2Score.setFont(font);
	p2Kills.setFont(font);
	p2HealthUsed.setFont(font);
	p2ShotsFired.setFont(font);

	totalKills.setFont(font);
	totalKills.setFont(font);
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