#include <iostream>
#include <string>
#include <vector>

#include "../include/Audio.h"

using namespace std;

void clear(void) { system("clear") ;}

int main(int argc, char* argv[]){
	clear();
    // can handle multiple commands for ease of use
    for (int i = 1; i < argc; i++){

        // command
        string arg = argv[i];

        // add images
        if (arg == "-a"){
            
        }
    }
}