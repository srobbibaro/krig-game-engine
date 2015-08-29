CC            := g++
CFLAGS        := -O3 -O2 -O1 -O -fpermissive -O3 -O2 -O1 -O
INCLUDES      := -I./src/ -I/usr/include/ -I/usr/include/malloc
TEST_INCLUDES := -I./src/ -I/usr/include/ -I/usr/include/malloc
LIBS          := -llua5.1 -lvorbisfile -lvorbisenc -lvorbis -lalut -logg -lopenal -lglut -lGL -lGLU

SRC_DIR        := ./src/
BUILD_DIR      := ./obj/src/
TEST_SRC_DIR   := ./test/
TEST_BUILD_DIR := ./obj/test/

SRC = $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)*.cpp))
OBJ = $(patsubst $(SRC_DIR)%.cpp, $(BUILD_DIR)%.o, $(SRC))
OBJ_WITHOUT_MAIN := $(filter-out $(BUILD_DIR)main.o, $(OBJ))

TEST_SRC = $(foreach sdir, $(TEST_SRC_DIR), $(wildcard $(sdir)*.cpp))
TEST_OBJ = $(patsubst $(TEST_SRC_DIR)%.cpp, $(TEST_BUILD_DIR)%.o, $(TEST_SRC))

default: build

all: build-test

rebuild: clean build

rebuild-all: clean build-test

krig: $(OBJ)
	$(CC) $(LIBS) $^ -o $@

unit-test: $(TEST_OBJ) $(OBJ_WITHOUT_MAIN)
	$(CC) $(LIBS) $^ -o $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(TEST_BUILD_DIR)%.o: $(TEST_SRC_DIR)%.cpp
	$(CC) $(CFLAGS) $(TEST_INCLUDES) -c $< -o $@

checkdirs: $(BUILD_DIR) $(TEST_BUILD_DIR)

build: checkdirs krig

build-test: build unit-test

run-tests: build-test
	./unit-test

run-demo: build
	./krig krig3

doc: build
	cd html; doxygen docs/doxyfile

$(BUILD_DIR):
	@mkdir -p $@

$(TEST_BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)
