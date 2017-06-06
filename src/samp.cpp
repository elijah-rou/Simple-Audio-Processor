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
    RSSELI007::AudioBase * song_1 = nullptr;
    RSSELI007::AudioBase * song_2 = nullptr;
    RSSELI007::AudioBase * result = nullptr;
    float param_1 = 0;
    float param_2 = 0;
    for (int i = 1; i < argc; i++){
        // parameters
        string arg = argv[i];
        try{
            if (arg == "-r"){
                r = atoi(argv[i+1]); 
                ++i;
                std::cout << "r: " << r << std::endl;
            }
            if (arg == "-b"){
                b = atoi(argv[i+1]); 
                ++i;
                std::cout << "b: " << b << std::endl;
            }
            if (arg == "-c"){
                c = atoi(argv[i+1]); 
                ++i;
                std::cout << "c: " << c << std::endl;
            }
            if (arg == "-o"){
                o = argv[i+1];
                ++i; 
                std::cout << "o: " << o << std::endl;
                continue;
            }
            if(r != 0 && (b == 8 || b == 16) && (c == 1 || c == 2) && o != ""){  
                std::cout << "Initialising operation: " << arg << std::endl;
                if (arg == "-add"){
                    std::cout << "Adding audio files together" << std::endl;
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+1]);
                    song_2 = RSSELI007::createAudio(r, b, c, argv[i+2]);
                    result = *song_1+*song_2;
                    result->write(o);
                    break;
                }
                else if (arg == "-cut"){
                    std::cout << "Removing range from audio" << std::endl;
                    param_1 = atoi(argv[i+1]);
                    param_2 = atoi(argv[i+2]);
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+3]);
                    std::pair<int, int> range(param_1, param_2);
                    result = *song_1^range;
                    result->write(o);
                    break;
                }
                else if (arg == "-radd"){
                    std::cout << "Adding audio files over range" << std::endl;
                    param_1 = atoi(argv[i+1]);
                    param_2 = atoi(argv[i+2]);
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+3]);
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+4]);
                    result = song_1->radd(song_2, (int)param_1, (int)param_2);
                    result->write(o);
                    break;
                }
                else if (arg == "-cat"){
                    std::cout << "Concatenating audio files" << std::endl;
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+1]);
                    song_2 = RSSELI007::createAudio(r, b, c, argv[i+2]);
                    result = *song_1|*song_2;
                    result->write(o);
                    break;
                }
                else if (arg == "-v"){
                    std::cout << "Volume over range" << std::endl;
                    param_1 = atoi(argv[i+1]);
                    param_2 = atoi(argv[i+2]);
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+3]);
                    std::pair<float, float> scaleFactor(param_1, param_2);
                    result = (*song_1)*(scaleFactor);
                    result->write(o);
                }
                else if (arg == "-rev"){
                    std::cout << "Reversing audio" << std::endl;
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+1]);
                    result = !*song_1;
                    result->write(o);
                    break;
                }
                else if (arg == "-rms"){
                    std::cout << "Root-Mean-Square of audio file" << std::endl;
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+1]);
                    result = song_1->rms();
                    result->write(o);
                    break;
                }
                else if (arg == "-norm"){
                    std::cout << "Normalising over range" << std::endl;
                    param_1 = atoi(argv[i+1]);
                    param_2 = atoi(argv[i+2]);
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+3]);
                    result = song_1->norm(param_1, param_2);
                    result->write(o);
                    break;
                }
                else if (arg == "-fadein"){
                    std::cout << "Implementing fade in on audio" << std::endl;
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+1]);
                    result = song_1->fadeIn();
                    result->write(o);
                    break;
                }
                else if (arg == "-fadeout"){
                    std::cout << "Implementing fade out on audio" << std::endl;
                    song_1 = RSSELI007::createAudio(r, b, c, argv[i+1]);
                    result = song_1->fadeOut();
                    result->write(o);
                    break;
                }
            }
            else{
                //std::cout << "Incorrect initialisation arguments." << std::endl;
                //break;
            }
        }
        catch(int e){
            std::cout << "Operation input error." << std::endl;
        }
        delete song_1;
        delete song_2;
        delete result;
    }
}