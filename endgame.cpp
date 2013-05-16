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

bool die = false;

void initEndScreen(){
	const int p1Column = 25;
	const int p2Column = 450;
	const int spacing = 48;
	const sf::Vector2f sf = sf::Vector2f(0.75,0.75); //scale factor

	ebgSprite.setTexture(bgTexture);
	ebButSprite.setTexture(bButTex);

	player1.setFont(font);
	player1.setOrigin(player1.getLocalBounds().width/2,player1.getLocalBounds().height/2);
	player1.setPosition(p1Column+50,200);
	player1.setScale(sf);
	
	player2.setFont(font);
	player2.setOrigin(player2.getLocalBounds().width/2,player2.getLocalBounds().height/2);
	player2.setPosition(p2Column+50,200);
	player2.setScale(sf);

	p1Score.setFont(font);
	p1Score.setPosition(p1Column,200+spacing*1);
	p1Score.setScale(sf);
	p1Kills.setFont(font);
	p1Kills.setPosition(p1Column,200+spacing*2);
	p1Kills.setScale(sf);
	p1HealthUsed.setFont(font);
	p1HealthUsed.setPosition(p1Column,200+spacing*3);
	p1HealthUsed.setScale(sf);
	p1ShotsFired.setFont(font);
	p1ShotsFired.setPosition(p1Column,200+spacing*4+12);
	p1ShotsFired.setScale(sf);

	p2Score.setFont(font);
	p2Score.setPosition(p2Column,200+spacing*1);
	p2Score.setScale(sf);
	p2Kills.setFont(font);
	p2Kills.setPosition(p2Column,200+spacing*2);
	p2Kills.setScale(sf);
	p2HealthUsed.setFont(font);
	p2HealthUsed.setPosition(p2Column,200+spacing*3);
	p2HealthUsed.setScale(sf);
	p2ShotsFired.setFont(font);
	p2ShotsFired.setPosition(p2Column,200+spacing*4+12);
	p2ShotsFired.setScale(sf);

	
	totalScore.setFont(font);
	totalScore.setScale(sf);
	totalKills.setFont(font);
	totalKills.setScale(sf);
}

void updateEndScreen(Stats *endStats){

	sf::Vector2f mPos;
	mPos.x = mousePos.x;
	mPos.y = mousePos.y;
	if(mouseRight && ebButSprite.getGlobalBounds().contains(mPos)){
		state = 0;
		if(gameStateDone)
			killAll();
		loading = false;
		inputIP = false;
		doShutdown = true;
		ready = false;
		connectedOnce = false;
		level = 1;
		serverThread->wait();
		clientThread->wait();
		serverEntities.entityList.clear();
		doShutdown = false;
		die = true;
	}

	player1.setString("Player 1");
	player2.setString("Player 2");
	p1Score.setString("Score: " + intToStr(endStats->p1Score));
	p1Kills.setString("Kills: " + intToStr(endStats->p1Kills));
	p1HealthUsed.setString("Health packs\nused: " + intToStr(endStats->p1HealthUsed));
	p1ShotsFired.setString("Shots fired: " + intToStr(endStats->p1ShotsFired));

	player1.setString("Player 1");
	player2.setString("Player 2");
	p2Score.setString("Score: " + intToStr(endStats->p2Score));
	p2Kills.setString("Kills: " + intToStr(endStats->p2Kills));
	p2HealthUsed.setString("Health packs\nused: " + intToStr(endStats->p2HealthUsed));
	p2ShotsFired.setString("Shots fired: " + intToStr(endStats->p2ShotsFired));

	totalScore.setString("Total score: " + intToStr(endStats->p1Score+endStats->p2Score));
	totalScore.setOrigin(totalScore.getLocalBounds().width/2,totalScore.getLocalBounds().height/2);
	totalScore.setPosition(400,500);

	totalKills.setString("Total Kills: " + intToStr(endStats->p1Kills+endStats->p2Kills));
	totalKills.setOrigin(totalKills.getLocalBounds().width/2,totalKills.getLocalBounds().height/2);
	totalKills.setPosition(400,548);

}

void drawEndScreen(sf::RenderWindow *screen){
	//if(die)
	//	screen->close();
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

	screen->draw(totalScore);
	screen->draw(totalKills);
}