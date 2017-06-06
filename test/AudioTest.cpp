#define CATCH_CONFIG_MAIN

#include <iostream>
#include <fstream>
#include "../include/catch.hpp"
#include "../include/Audio.h"
#include "../include/audioUtil.h"
#include <typeinfo>

using namespace RSSELI007;
using namespace std;


TEST_CASE("Mono Audio template", "[mono]"){
    SECTION("CONSTRUCT and WRITE: Mono"){
        AudioBase * a = createAudio(44100, 8, 1, "input/beez18sec_44100_signed_8bit_mono.raw");

        REQUIRE(a->getSampleRate() == 44100);
        REQUIRE(a->getFilename() == "input/beez18sec_44100_signed_8bit_mono.raw");
        REQUIRE(typeid(*a) == typeid(Audio<int8_t, 1>));

        a->write("test");
        AudioBase * test = createAudio(44100, 8, 1, "output/test.raw");
        REQUIRE(a->dataPieces() == test->dataPieces());
        
    }

    SECTION("COPY"){
        AudioBase * a = createAudio(44100, 8, 1, "input/beez18sec_44100_signed_8bit_mono.raw");
        AudioBase * b = a;
        
    }

    SECTION("COPY Assignment"){
        
    }

    SECTION("MOVE"){
    
    }

    SECTION("MOVE Assignment"){
        
    }
}