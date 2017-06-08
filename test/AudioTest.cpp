#define CATCH_CONFIG_MAIN
#include "../include/catch.hpp"

#include <iostream>
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
        REQUIRE(*test == *a);
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
        AudioBase * a = createAudio(44100, 8, 1, "input/test_1.raw");
        AudioBase * b = createAudio(44100, 8, 1, "input/test_2.raw");
        AudioBase * result = *a+*b;
        const vector<int8_t> v = dynamic_cast<Audio<int8_t, 1>*>(result)->getData();
        for(int8_t i : v){
            REQUIRE(i == 'a');
        }
        delete a;
        delete b;
        delete result;
    }

    SECTION("Volume Scale"){
        AudioBase * a = createAudio(44100, 8, 1, "input/test_1.raw");
        std::pair<float, float> range(0.5, 1);
        AudioBase * result = *a*range;
        const vector<int8_t> v = dynamic_cast<Audio<int8_t, 1>*>(result)->getData();
        for(int i = 0; i < v.size(); ++i){
            if(i % 2 == 0){
                REQUIRE(v[i] == '1'/2);
            }
            else{
                REQUIRE(v[i] == '0'/2);
            }
        }
    }

    SECTION("Concatenation"){
        AudioBase * a = createAudio(44100, 8, 1, "input/test_1.raw");
        AudioBase * b = createAudio(44100, 8, 1, "input/test_2.raw");
        AudioBase * result = *a|*b;
        const vector<int8_t> v = dynamic_cast<Audio<int8_t, 1>*>(result)->getData();
        REQUIRE(result->dataPieces() == 16);
        for(int i = 0; i < 8; ++i){
            if(i % 2 == 0){
                REQUIRE(v[i] == '0');
            }
            else{
                REQUIRE(v[i] == '1');
            }
        }
        for(int i = 8; i < v.size(); ++i){
            if(i % 2 == 0){
                REQUIRE(v[i] == '1');
            }
            else{
                REQUIRE(v[i] == '0');
            }
        }
        delete a;
        delete b;
        delete result;
    }

    SECTION("Splice"){
        AudioBase * a = createAudio(44100, 8, 1, "input/test_1.raw");
        std::pair<int, int> range(1, 3);
        AudioBase * result = *a^range;
        const vector<int8_t> v = dynamic_cast<Audio<int8_t, 1>*>(result)->getData();
        REQUIRE(result->dataPieces() == 5);
        for(int i = 0; i < v.size(); ++i){
            if(i == 0){
                REQUIRE(v[i] == '0');
            }
            else if(i % 2 == 0){
                REQUIRE(v[i] == '1');
            }
            else{
                REQUIRE(v[i] == '0');
            }
        }
        delete a;
        delete result;
    }

    SECTION("Reverse"){
        AudioBase * a = createAudio(44100, 8, 1, "input/test_1.raw");
        AudioBase * result = !*a;
        const vector<int8_t> v = dynamic_cast<Audio<int8_t, 1>*>(result)->getData();
        for(int i = 0; i < v.size(); ++i){
            if(i % 2 == 0){
                REQUIRE(v[i] == '1');
            }
            else{
                REQUIRE(v[i] == '0');
            }
        }
        delete a;
        delete result;
    }

    SECTION("Equality"){
        AudioBase * a = createAudio(44100, 8, 1, "input/test_1.raw");
        AudioBase * result = !*a;
        AudioBase * b = createAudio(44100, 8, 1, "input/test_2.raw");
        REQUIRE(*result == *b);
        delete a;
        delete result;
        delete b;
    }
}

TEST_CASE("MONO: Audio Methods", "[methods]"){
    SECTION("Range Add"){
        AudioBase * a = createAudio(44100, 8, 1, "input/test_1.raw");
        AudioBase * b = createAudio(44100, 8, 1, "input/test_2.raw");
        std::pair<int, int> range(1, 3);
        AudioBase * result = a->radd(*b, range);
        const vector<int8_t> v = dynamic_cast<Audio<int8_t, 1>*>(result)->getData();
        for(int i = 0; i < v.size(); ++i){
            if(i == 1 || i == 2 || i == 3){
                REQUIRE(v[i] == 'a');
            }
            else if(i % 2 == 0){
                REQUIRE(v[i] == '1');
            }
            else{
                REQUIRE(v[i] == '0');
            }
        }
        delete a;
        delete b;
        delete result;
    }

    SECTION("Root-Mean-Square"){
        AudioBase * a = createAudio(44100, 8, 1, "input/test_3.raw");
        float result = a->rms();
        REQUIRE(result == 40);
        delete a;
    }

    SECTION("Normalise over range"){

    }

    SECTION("Fade In"){

    }

    SECTION("Fade Out"){

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