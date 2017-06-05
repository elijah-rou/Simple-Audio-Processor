#include <iostream>
#include <string>
#include <vector>

#include "../include/audioUtil.h"

using namespace std;

int main(int argc, char* argv[]){
    RSSELI007::clear();
    // can handle multiple commands for ease of use
    int r = 0;
    int b = 0;
    int c = 0;
    std::string o = "";
    int opIndex = 0;
    RSSELI007::AudioBase * song_1;
    RSSELI007::AudioBase * song_2;
    float param_1 = 0;
    float param_2 = 0;
    for (int i = 1; i < argc; i++){
        // parameters
        string arg = argv[i];
        try{
            if (arg == "-r"){
                r = atoi(argv[i+1]); 
                ++i;
            }
            if (arg == "-b"){
                b = atoi(argv[i+1]); 
                ++i;
            }
            if (arg == "-c"){
                c = atoi(argv[i+1]); 
                ++i;
            }
            if (arg == "-o"){
                o = argv[i+1];
                ++i; 
            }
            if(r != 0 && (b == 8 || b == 16) && (c == 1 || c == 2) && o != ""){  
                if (arg == "-add"){
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+1]);
                    song_2 = &RSSELI007::createAudio(r, b, c, argv[i+2]);
                    RSSELI007::AudioBase & result = song_1->add(song_2);
                    result.write(o);
                }
                else if (arg == "-cut"){
                    param_1 = atoi(argv[i+1]);
                    param_2 = atoi(argv[i+2]);
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    RSSELI007::AudioBase & result = song_1->cut((int)param_1, (int)param_2);
                    result.write(o);
                }
                else if (arg == "-radd"){
                    param_1 = atoi(argv[i+1]);
                    param_2 = atoi(argv[i+2]);
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    RSSELI007::AudioBase & result = song_1->radd(song_2, (int)param_1, (int)param_2);
                    result.write(o);
                }
                else if (arg == "-cat"){
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+1]);
                    song_2 = &RSSELI007::createAudio(r, b, c, argv[i+2]);
                    RSSELI007::AudioBase & result = song_1->cat(song_2);
                    result.write(o);
                }
                else if (arg == "-v"){
                    param_1 = atoi(argv[i+1]);
                    param_2 = atoi(argv[i+2]);
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    RSSELI007::AudioBase & result = song_1->v(param_1, param_2);
                    result.write(o);
                }
                else if (arg == "-rev"){
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    RSSELI007::AudioBase & result = song_1->rev();
                    result.write(o);
                }
                else if (arg == "-rms"){
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    RSSELI007::AudioBase & result = song_1->rms();
                    result.write(o);
                }
                else if (arg == "-norm"){
                    param_1 = atoi(argv[i+1]);
                    param_2 = atoi(argv[i+2]);
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    RSSELI007::AudioBase & result = song_1->norm(param_1, param_2);
                    result.write(o);
                }
                else if (arg == "-fadein"){
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    RSSELI007::AudioBase & result = song_1->fadeIn();
                    result.write(o);
                }
                else if (arg == "-fadeout"){
                    song_1 = &RSSELI007::createAudio(r, b, c, argv[i+3]);
                    RSSELI007::AudioBase & result = song_1->fadeOut();
                    result.write(o);
                }
            }
            else{
                std::cout << "Incorrect initialisation arguments." << std::endl;
            }
        }
        catch(int e){
            std::cout << "Operation input error." << std::endl;
        }
        delete song_1;
        delete song_2;
    }
}