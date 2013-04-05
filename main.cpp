#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include "dungeon.h"
using namespace std;

int main(int argc, char *argv[]){
	int seed = time(NULL);
	cout << "seed: " << seed << endl;
	srand(seed);
	Dungeon bleh;
	return 0;
}
