#ifndef AUDIOUTIL_H
#define AUDIOUTIL_H

#include <iostream>
#include "../include/Audio.h"

namespace RSSELI007{
    void clear(void) { system("clear") ;}

    RSSELI007::AudioBase * createAudio(int rate, int bitNo, int channels, std::string filename){
        // needs to return heap references
        switch(bitNo){
            case 8:{
                switch(channels){
                    case 1:{
                        //std::cout << "int8_t, mono" << std::endl;
                        RSSELI007::Audio<int8_t, 1> * a = new RSSELI007::Audio<int8_t, 1>(filename, rate);
                        return a;
                    }
                    case 2:{
                        RSSELI007::Audio<int8_t, 2> * a = new RSSELI007::Audio<int8_t, 2>(filename, rate);
                        return a;
                    }
                    break;
                }
            }
            case 16:{
                switch(channels){
                    case 1:{
                        RSSELI007::Audio<int16_t, 1> * a = new RSSELI007::Audio<int16_t, 1>(filename, rate);
                        return a;
                    }
                    case 2:{
                        RSSELI007::Audio<int16_t, 2> * a = new RSSELI007::Audio<int16_t, 2>(filename, rate);
                        return a;
                    }
                    break;
                }
            }
        }
    }
}

#endif