#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"

#include <iostream>
#include <fstream>
#include <typeinfo>
#include "../include/Audio.h"
#include "../include/audioUtil.h"

using namespace RSSELI007;
using namespace std;


TEST_CASE("MONO: Audio template Construction", "[constructors]"){
    SECTION("CONSTRUCT and WRITE"){
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
        delete a;
        delete b;
        
    }

    SECTION("COPY Assignment"){
        // only for specific situations
        /*
        Audio<int8_t, 1> a("input/beez18sec_44100_signed_8bit_mono.raw", 44100);
        Audio<int8_t, 1> b = dynamic_cast<Aa;
        REQUIRE(b != a);
        REQUIRE(typeid(b) == typeid(a));
        REQUIRE(b==a);
        
        cout << endl;
        */
    }
    
    SECTION("MOVE"){
       
    }

    SECTION("MOVE Assignment"){
        
    }
    
}

TEST_CASE("MONO: Audio Overloads", "[overloads]"){
    SECTION("Addition"){

    }

    SECTION("Volume Scale"){

    }

    SECTION("Concatenation"){

    }

    SECTION("Splice"){

    }

    SECTION("Reverse"){

    }

    SECTION("Equality"){

    }
}

TEST_CASE("MONO: Audio Methods", "[methods]"){
    SECTION("Range Add"){

    }

    SECTION("Root-Mean-Square"){

    }

    SECTION("Normalise over range"){

    }

    SECTION("Fade In"){

    }

    SECTION("Fade Out"){

    }

    SECTION("Equality"){

    }
}

TEST_CASE("STEREO: Audio template Construction", "[constructors]"){
    SECTION("CONSTRUCT and WRITE"){
        /*
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
        */
        
    }

    SECTION("COPY"){
        /*
        AudioBase * a = createAudio(44100, 8, 1, "input/beez18sec_44100_signed_8bit_mono.raw");
        AudioBase * b = new Audio<int8_t, 1>(*a);
        REQUIRE(b != a);
        REQUIRE(typeid(*b) == typeid(*a));
        REQUIRE(*b==*a);
        
        cout << endl;
        */
        
    }

    SECTION("COPY Assignment"){

    }
    
    SECTION("MOVE"){
       
    }

    SECTION("MOVE Assignment"){
        
    }
    
}

TEST_CASE("STEREO: Audio Overloads", "[overloads]"){
    SECTION("Addition"){

    }

    SECTION("Volume Scale"){

    }

    SECTION("Concatenation"){

    }

    SECTION("Splice"){

    }

    SECTION("Reverse"){

    }

    SECTION("Equality"){

    }
}

TEST_CASE("STEREO: Audio Methods", "[methods]"){
    SECTION("Range Add"){

    }

    SECTION("Root-Mean-Square"){

    }

    SECTION("Normalise over range"){

    }

    SECTION("Fade In"){

    }

    SECTION("Fade Out"){

    }

    SECTION("Equality"){

    }
}