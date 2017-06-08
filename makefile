CC = g++
CC_FLAGS = -c -Wall -o 
LD_FLAGS = -o
SRC_FILES = $(wildcard src/*.cpp)
TEST_FILES = $(wildcard test/*.cpp)
OBJ_FILES = $(addprefix obj/,$(notdir $(SRC_FILES:.cpp=.o)))
OUT = samp

default: $(OBJ_FILES)
# create main
	$(CC) $(SRC_FILES) $(LD_FLAGS) bin/$(OUT) --std=c++11

obj/%.o :src/%.cpp
	$(CC) $(CC_FLAGS) $@ $^ --std=c++11

clean:
	-rm obj/*.o
	-rm bin/$(OUT)
	-rm bin/test
	-rm output/*

run:
	@make
	bin/$(OUT) $(args)

quickMono8:
	bin/$(OUT) -r 44100 -b 8 -c 1 -o monoAdd8 -add input/beez18sec_44100_signed_8bit_mono.raw input/frogs18sec_44100_signed_8bit_mono.raw
	bin/$(OUT) -r 44100 -b 8 -c 1 -o monoCut8 -cut 198450 595350 input/beez18sec_44100_signed_8bit_mono.raw
	bin/$(OUT) -r 44100 -b 8 -c 1 -o monoRAdd8 -radd 0 8 9 17 input/beez18sec_44100_signed_8bit_mono.raw input/frogs18sec_44100_signed_8bit_mono.raw
	bin/$(OUT) -r 44100 -b 8 -c 1 -o monoCat8 -cat input/beez18sec_44100_signed_8bit_mono.raw input/frogs18sec_44100_signed_8bit_mono.raw
	bin/$(OUT) -r 44100 -b 8 -c 1 -o monoV8 -v 0.5 0.5 input/beez18sec_44100_signed_8bit_mono.raw
	bin/$(OUT) -r 44100 -b 8 -c 1 -o monoRev8 -rev input/beez18sec_44100_signed_8bit_mono.raw
	bin/$(OUT) -r 44100 -b 8 -c 1 -o monoNorm8 -norm 10 10 input/beez18sec_44100_signed_8bit_mono.raw
	bin/$(OUT) -r 44100 -b 8 -c 1 -rms input/beez18sec_44100_signed_8bit_mono.raw

quickMono16:
	bin/$(OUT) -r 44100 -b 16 -c 1 -o monoAdd16 -add input/beez18sec_44100_signed_16bit_mono.raw input/frogs18sec_44100_signed_16bit_mono.raw
	bin/$(OUT) -r 44100 -b 16 -c 1 -o monoCut16 -cut 198450 595350 input/beez18sec_44100_signed_16bit_mono.raw
	bin/$(OUT) -r 44100 -b 16 -c 1 -o monoRAdd16 -radd 0 8 8 16 input/beez18sec_44100_signed_16bit_mono.raw input/frogs18sec_44100_signed_16bit_mono.raw
	bin/$(OUT) -r 44100 -b 16 -c 1 -o monoCat16 -cat input/beez18sec_44100_signed_16bit_mono.raw input/frogs18sec_44100_signed_16bit_mono.raw
	bin/$(OUT) -r 44100 -b 16 -c 1 -o monoV16 -v 0.5 0.5 input/beez18sec_44100_signed_16bit_mono.raw
	bin/$(OUT) -r 44100 -b 16 -c 1 -o monoRev16 -rev input/beez18sec_44100_signed_16bit_mono.raw
	bin/$(OUT) -r 44100 -b 16 -c 1 -o monoNorm16 -norm 10 10 input/beez18sec_44100_signed_16bit_mono.raw
	bin/$(OUT) -r 44100 -b 16 -c 1 -rms input/beez18sec_44100_signed_16bit_mono.raw

quickStereo8:
	bin/$(OUT) -r 44100 -b 8 -c 2 -o stereoAdd8 -add input/beez18sec_44100_signed_8bit_stereo.raw input/frogs18sec_44100_signed_8bit_stereo.raw
	bin/$(OUT) -r 44100 -b 8 -c 2 -o stereoCut8 -cut 198450 595350 input/beez18sec_44100_signed_8bit_stereo.raw
	bin/$(OUT) -r 44100 -b 8 -c 2 -o stereoRAdd8 -radd 0 8 8 16 input/beez18sec_44100_signed_8bit_stereo.raw input/frogs18sec_44100_signed_8bit_stereo.raw
	bin/$(OUT) -r 44100 -b 8 -c 2 -o stereoCat8 -cat input/beez18sec_44100_signed_8bit_stereo.raw input/frogs18sec_44100_signed_8bit_stereo.raw
	bin/$(OUT) -r 44100 -b 8 -c 2 -o stereoV8 -v 0.5 0.5 input/beez18sec_44100_signed_8bit_stereo.raw
	bin/$(OUT) -r 44100 -b 8 -c 2 -o stereoRev8 -rev input/beez18sec_44100_signed_8bit_stereo.raw
	bin/$(OUT) -r 44100 -b 8 -c 2 -o stereoNorm8 -norm 10 10 input/beez18sec_44100_signed_8bit_stereo.raw
	bin/$(OUT) -r 44100 -b 8 -c 2 -rms input/beez18sec_44100_signed_8bit_stereo.raw

quickStereo16:
	bin/$(OUT) -r 44100 -b 16 -c 2 -o stereoAdd16 -add input/beez18sec_44100_signed_16bit_stereo.raw input/frogs18sec_44100_signed_16bit_stereo.raw
	bin/$(OUT) -r 44100 -b 16 -c 2 -o stereoCut16 -cut 198450 595350 input/beez18sec_44100_signed_16bit_stereo.raw
	bin/$(OUT) -r 44100 -b 16 -c 2 -o stereoRAdd16 -radd 0 8 8 16 input/beez18sec_44100_signed_16bit_stereo.raw input/frogs18sec_44100_signed_16bit_stereo.raw
	bin/$(OUT) -r 44100 -b 16 -c 2 -o stereoCat16 -cat input/beez18sec_44100_signed_16bit_stereo.raw input/frogs18sec_44100_signed_16bit_stereo.raw
	bin/$(OUT) -r 44100 -b 16 -c 2 -o stereoV16 -v 0.5 0.5 input/beez18sec_44100_signed_16bit_stereo.raw
	bin/$(OUT) -r 44100 -b 16 -c 2 -o stereoRev16 -rev input/beez18sec_44100_signed_16bit_stereo.raw
	bin/$(OUT) -r 44100 -b 16 -c 2 -o stereoNorm16 -norm 10 10 input/beez18sec_44100_signed_16bit_stereo.raw
	bin/$(OUT) -r 44100 -b 16 -c 2 -rms input/beez18sec_44100_signed_16bit_stereo.raw
	
test: $(OBJ_FILES)
	$(CC) $(TEST_FILES) -o bin/test --std=c++11
	bin/test $(args)
