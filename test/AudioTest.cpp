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

        a->write("testMono");
        AudioBase * test = createAudio(44100, 8, 1, "output/testMono_44100_8bit_mono.raw");
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
        Audio<int8_t, 1> a("input/beez18sec_44100_signed_8bit_mono.raw", 44100);
        Audio<int8_t, 1> b = a;
        REQUIRE(&b != &a);
        REQUIRE(typeid(b) == typeid(a));
        REQUIRE(b==a);
        
        cout << endl;
        
    }
    
    SECTION("MOVE"){
        Audio<int8_t, 1> a("input/beez18sec_44100_signed_8bit_mono.raw", 44100);
        Audio<int8_t, 1> aCopy(a);
        Audio<int8_t, 1> b(std::move(a));
        REQUIRE(&b != &a);
        REQUIRE(typeid(b) == typeid(aCopy));
        REQUIRE(a.getSampleRate() == 0);
        REQUIRE(a.getFilename() == "");
        REQUIRE(a.getCutoff() == 0);
        REQUIRE(a.getData().size() == 0);
        REQUIRE(b==aCopy);
    }

    SECTION("MOVE Assignment"){
        Audio<int8_t, 1> a("input/beez18sec_44100_signed_8bit_mono.raw", 44100);
        Audio<int8_t, 1> aCopy(a);
        Audio<int8_t, 1> b = std::move(a);
        REQUIRE(&b != &a);
        REQUIRE(typeid(b) == typeid(aCopy));
        REQUIRE(a.getSampleRate() == 0);
        REQUIRE(a.getFilename() == "");
        REQUIRE(a.getCutoff() == 0);
        REQUIRE(a.getData().size() == 0);
        REQUIRE(b==aCopy);
    }
    
}

TEST_CASE("MONO: Audio Overloads", "[overloads]"){
    SECTION("Addition"){
        AudioBase * a = createAudio(44100, 8, 1, "test/test_1_8bit_mono.raw");
        AudioBase * b = createAudio(44100, 8, 1, "test/test_2_8bit_mono.raw");
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
        AudioBase * a = createAudio(44100, 8, 1, "test/test_1_8bit_mono.raw");
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
        AudioBase * a = createAudio(44100, 8, 1, "test/test_1_8bit_mono.raw");
        AudioBase * b = createAudio(44100, 8, 1, "test/test_2_8bit_mono.raw");
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
        AudioBase * a = createAudio(44100, 8, 1, "test/test_1_8bit_mono.raw");
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
        AudioBase * a = createAudio(44100, 8, 1, "test/test_1_8bit_mono.raw");
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
        AudioBase * a = createAudio(44100, 8, 1, "test/test_1_8bit_mono.raw");
        AudioBase * result = !*a;
        AudioBase * b = createAudio(44100, 8, 1, "test/test_2_8bit_mono.raw");
        REQUIRE(*result == *b);
        REQUIRE(*result != *a);
        delete a;
        delete result;
        delete b;
    }
}

TEST_CASE("MONO: Audio Methods", "[methods]"){
    SECTION("Range Add"){
        
        AudioBase * a = createAudio(44100, 8, 1, "test/test_1_8bit_mono.raw");
        AudioBase * b = createAudio(44100, 8, 1, "test/test_2_8bit_mono.raw");
        pair<float, float> range1(1.0/44100, 3.0/44100);
        pair<float, float> range2(1.0/44100, 3.0/44100);
        AudioBase * result = a->radd(*b, range1, range2);
        const vector<int8_t> v = dynamic_cast<Audio<int8_t, 1>*>(result)->getData();
        REQUIRE(v.size() == 3);
        REQUIRE(v[0] == 'a');
        REQUIRE(v[1] == 'a');
        REQUIRE(v[2] == 'a');
        delete a;
        delete b;
        delete result;
        
    }

    SECTION("Root-Mean-Square"){
        AudioBase * a = createAudio(44100, 8, 1, "test/test_3_8bit_mono.raw");
        float result = a->rms().first;
        REQUIRE(result == 40);
        delete a;
    }

    SECTION("Normalise over to RMS"){
        AudioBase * a = createAudio(44100, 8, 1, "test/test_3_8bit_mono.raw");
        std::pair<float, float> rms(20, 20);
        AudioBase * result = a->norm(rms);
        REQUIRE(result->rms().first == 20);
        delete a;
    }

    SECTION("Fade In"){

    }

    SECTION("Fade Out"){

    }
}

TEST_CASE("STEREO: Audio template Construction", "[constructors]"){
    SECTION("CONSTRUCT and WRITE"){

        AudioBase * a = createAudio(44100, 8, 2, "input/beez18sec_44100_signed_8bit_stereo.raw");

        REQUIRE(a->getSampleRate() == 44100);
        REQUIRE(a->getFilename() == "input/beez18sec_44100_signed_8bit_stereo.raw");
        REQUIRE(typeid(*a) == typeid(Audio<int8_t, 2>));

        a->write("testStereo");
        AudioBase * test = createAudio(44100, 8, 2, "output/testStereo_44100_8bit_stereo.raw");
        REQUIRE(a->dataPieces() == test->dataPieces());
        
        delete a;
        delete test;

        AudioBase * b = createAudio(44100, 16, 2, "input/beez18sec_44100_signed_16bit_stereo.raw");
        REQUIRE(typeid(*b) == typeid(Audio<int16_t, 2>));
        delete b;

        cout << endl;
        
        
    }

    SECTION("COPY"){
        
        AudioBase * a = createAudio(44100, 16, 2, "input/beez18sec_44100_signed_16bit_stereo.raw");
        AudioBase * b = new Audio<int16_t, 2>(*a);
        REQUIRE(b != a);
        REQUIRE(typeid(*b) == typeid(*a));
        REQUIRE(*b==*a);
        cout << endl;
        
        
    }

    SECTION("COPY Assignment"){
        Audio<int16_t, 2> a("input/beez18sec_44100_signed_16bit_stereo.raw", 44100);
        Audio<int16_t, 2> b = a;
        REQUIRE(&b != &a);
        REQUIRE(typeid(b) == typeid(a));
        REQUIRE(b==a);
        
        cout << endl;
    }
    
    SECTION("MOVE"){
        Audio<int16_t, 2> a("input/beez18sec_44100_signed_16bit_stereo.raw", 44100);
        Audio<int16_t, 2> aCopy(a);
        Audio<int16_t, 2> b(std::move(a));
        REQUIRE(&b != &a);
        REQUIRE(typeid(b) == typeid(aCopy));
        REQUIRE(a.getSampleRate() == 0);
        REQUIRE(a.getFilename() == "");
        REQUIRE(a.getCutoff() == 0);
        REQUIRE(a.getData().size() == 0);
        REQUIRE(b==aCopy);
    }

    SECTION("MOVE Assignment"){
        Audio<int16_t, 2> a("input/beez18sec_44100_signed_16bit_stereo.raw", 44100);
        Audio<int16_t, 2> aCopy(a);
        Audio<int16_t, 2> b = std::move(a);
        REQUIRE(&b != &a);
        REQUIRE(typeid(b) == typeid(aCopy));
        REQUIRE(a.getSampleRate() == 0);
        REQUIRE(a.getFilename() == "");
        REQUIRE(a.getCutoff() == 0);
        REQUIRE(a.getData().size() == 0);
        REQUIRE(b==aCopy);
    }
    
}

TEST_CASE("STEREO: Audio Overloads", "[overloads]"){
    SECTION("Addition"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_1_8bit_stereo.raw");
        AudioBase * b = createAudio(44100, 8, 2, "test/test_2_8bit_stereo.raw");
        AudioBase * result = *a+*b;
        const vector<std::pair<int8_t, int8_t> > v = dynamic_cast<Audio<int8_t, 2>*>(result)->getData();
        for(std::pair<int8_t, int8_t> i : v){
            REQUIRE(i.first == 'a');
            REQUIRE(i.second == 'a');
        }
        delete a;
        delete b;
        delete result;
    }

    SECTION("Volume Scale"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_1_8bit_stereo.raw");
        std::pair<float, float> range(0.5, 0.5);
        AudioBase * result = *a*range;
        const vector<std::pair<int8_t, int8_t> > v = dynamic_cast<Audio<int8_t, 2>*>(result)->getData();
        for(int i = 0; i < v.size(); ++i){
            if(i % 2 == 0){
                REQUIRE(v[i].first == '1'/2);
                REQUIRE(v[i].second == '1'/2);
            }
            else{
                REQUIRE(v[i].first == '0'/2);
                REQUIRE(v[i].second == '0'/2);
            }
        }
    }

    SECTION("Concatenation"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_1_8bit_stereo.raw");
        AudioBase * b = createAudio(44100, 8, 2, "test/test_2_8bit_stereo.raw");
        AudioBase * result = *a|*b;
        const vector<std::pair<int8_t, int8_t> > v = dynamic_cast<Audio<int8_t, 2>*>(result)->getData();
        REQUIRE(result->dataPieces() == 16);
        for(int i = 0; i < 8; ++i){
            if(i % 2 == 0){
                REQUIRE(v[i].first == '0');
                REQUIRE(v[i].second == '0');
            }
            else{
                REQUIRE(v[i].first == '1');
                REQUIRE(v[i].second == '1');
            }
        }
        for(int i = 8; i < v.size(); ++i){
            if(i % 2 == 0){
                REQUIRE(v[i].first == '1');
                REQUIRE(v[i].second == '1');
            }
            else{
                REQUIRE(v[i].first == '0');
                REQUIRE(v[i].second == '0');
            }
        }
        delete a;
        delete b;
        delete result;
    }

    SECTION("Splice"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_1_8bit_stereo.raw");
        std::pair<int, int> range(1, 3);
        AudioBase * result = *a^range;
        const vector<std::pair<int8_t, int8_t> > v = dynamic_cast<Audio<int8_t, 2>*>(result)->getData();
        REQUIRE(result->dataPieces() == 5);
        for(int i = 0; i < v.size(); ++i){
            if(i == 0){
                REQUIRE(v[i].first == '0');
                REQUIRE(v[i].second == '0');
            }
            else if(i % 2 == 0){
                REQUIRE(v[i].first == '1');
                REQUIRE(v[i].second == '1');
            }
            else{
                REQUIRE(v[i].first == '0');
                REQUIRE(v[i].second == '0');
            }
        }
        delete a;
        delete result;
    }

    SECTION("Reverse"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_1_8bit_stereo.raw");
        AudioBase * result = !*a;
        const vector<std::pair<int8_t, int8_t> > v = dynamic_cast<Audio<int8_t, 2>*>(result)->getData();
        for(int i = 0; i < v.size(); ++i){
            if(i % 2 == 0){
                REQUIRE(v[i].first == '1');
                REQUIRE(v[i].second == '1');
            }
            else{
                REQUIRE(v[i].first == '0');
                REQUIRE(v[i].second == '0');
            }
        }
        delete a;
        delete result;
    }

    SECTION("Equality"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_1_8bit_stereo.raw");
        AudioBase * result = !*a;
        AudioBase * b = createAudio(44100, 8, 2, "test/test_2_8bit_stereo.raw");
        REQUIRE(*result == *b);
        REQUIRE(*result != *a);
        delete a;
        delete result;
        delete b;
    }
}

TEST_CASE("STEREO: Audio Methods", "[methods]"){
    SECTION("Range Add"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_1_8bit_stereo.raw");
        AudioBase * b = createAudio(44100, 8, 2, "test/test_2_8bit_stereo.raw");
        pair<float, float> range1(1.0/44100, 3.0/44100);
        pair<float, float> range2(1.0/44100, 3.0/44100);
        AudioBase * result = a->radd(*b, range1, range2);
        const vector<std::pair<int8_t, int8_t> > v = dynamic_cast<Audio<int8_t, 2>*>(result)->getData();
        REQUIRE(v.size() == 3);
        REQUIRE(v[0].first == 'a');
        REQUIRE(v[1].first == 'a');
        REQUIRE(v[2].first == 'a');

        REQUIRE(v[0].second == 'a');
        REQUIRE(v[1].second == 'a');
        REQUIRE(v[2].second == 'a');
        delete a;
        delete b;
        delete result;
    }

    SECTION("Root-Mean-Square"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_3_8bit_stereo.raw");
        std::pair<float, float> result = a->rms();
        REQUIRE(result.first == 40);
        REQUIRE(result.second == 80);
        delete a;
    }

    SECTION("Normalise over range"){
        AudioBase * a = createAudio(44100, 8, 2, "test/test_3_8bit_stereo.raw");
        std::pair<float, float> rms(20, 20);
        AudioBase * result = a->norm(rms);
        REQUIRE(result->rms().first == 20);
        REQUIRE(result->rms().second == 20);
        delete a;
    }

    SECTION("Fade In"){

    }

    SECTION("Fade Out"){

    }
}