#define CATCH_CONFIG_MAIN

#include <iostream>
#include <fstream>
#include <typeinfo>
#include "../include/catch.hpp"
#include "../include/Audio.h"
#include "../include/audioUtil.h"

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
        delete a;
        delete test;

        AudioBase * b = createAudio(44100, 16, 1, "input/beez18sec_44100_signed_16bit_mono.raw");
        REQUIRE(typeid(*b) == typeid(Audio<int16_t, 1>));
        delete b;

        cout << endl;
        
    }

    SECTION("COPY"){
        AudioBase * a = createAudio(44100, 8, 1, "input/beez18sec_44100_signed_8bit_mono.raw");
        AudioBase * b = new Audio<int8_t, 1>(*a);
        REQUIRE(b != a);
        REQUIRE(typeid(*b) == typeid(*a));
        REQUIRE(*b==*a);
        
        cout << endl;
        
    }

    SECTION("COPY Assignment"){

    }
    /*
    SECTION("MOVE"){
        AudioBase * a = createAudio(44100, 8, 1, "input/beez18sec_44100_signed_8bit_mono.raw");

        AudioBase * b = new Audio<int8_t, 1>(std::move(*a));
        REQUIRE(a == nullptr);
        
        cout << endl;
    }

    SECTION("MOVE Assignment"){
        
    }
    */
}