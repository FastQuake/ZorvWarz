#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
//#include "dungeon.h"
#include "ship.h"
using namespace std;

int main(int argc, char *argv[]){
	int seed = time(NULL);
	cout << "seed: " << seed << endl;
	srand(seed);
	//Dungeon bleh;
	Ship bleh;
	return 0;
}
