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
	-rm bin/output/*

run:
	@make
	bin/$(OUT) $(args)

quick:
	@make
	bin/$(OUT) -r 44100 -b 8 -c 1 -o outQuick -rev input/beez18sec_44100_signed_8bit_mono.raw
	
test: $(OBJ_FILES)
	$(CC) $(TEST_FILES) -o bin/test --std=c++11
	bin/test $(args)
	rm output/test.raw
