#include <iostream>
#include <string>
#include <vector>

#include "../include/Audio.h"

using namespace std;

void clear(void) { system("clear") ;}

int main(int argc, char* argv[]){
	clear();
    // can handle multiple commands for ease of use
    int i = 1;
    int j = 2;
    RSSELI007::AudioBase * song_1;
    RSSELI007::AudioBase * song_2;
    for (int i = 1; i < argc; i++){

        // command
        string arg = argv[i];

        // add images
        if (arg == "-a"){
            
        }
    }
}