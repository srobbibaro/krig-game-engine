OS_NAME       := $(shell uname -s)
HOMEBREW_DIR  := $(shell brew --prefix)
DEBUG_LEVEL   := 2


CC            := g++
CFLAGS        := -O3 -O2 -O1 -O -fpermissive -O3 -O2 -O1 -O -std=c++11
INCLUDES      := -I./src/ -I/usr/include/
TEST_INCLUDES := -I./src/ -I/usr/include/
LIBS          := -lluajit -lvorbisfile -lvorbisenc -lvorbis -logg -lopenal -lpng -lglut -lGL -lGLU
LDFLAGS       :=
DEFINES       := -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

ifeq ($(OS_NAME), Darwin)
	INCLUDES      += -I$(HOMEBREW_DIR)/include/ -I$(HOMEBREW_DIR)/opt/openal-soft/include/
	TEST_INCLUDES += -I$(HOMEBREW_DIR)/include/ -I$(HOMEBREW_DIR)/opt/openal-soft/include/
	LDFLAGS       := -L$(HOMEBREW_DIR)/lib -L$(HOMEBREW_DIR)/opt/libglu/lib -L$(HOMEBREW_DIR)/opt/openal-soft/lib/ -L$(HOMEBREW_DIR)/opt/freeglut/lib/
endif

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
	$(CC) $^ $(LIBS) $(LDFLAGS) -o $@

unit-test: $(TEST_OBJ) $(OBJ_WITHOUT_MAIN)
	$(CC) $^ $(LIBS) $(LDFLAGS) -o $@

$(BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(TEST_BUILD_DIR)%.o: $(TEST_SRC_DIR)%.cpp
	$(CC) $(CFLAGS) $(TEST_INCLUDES) $(DEFINES) -c $< -o $@

checkdirs: $(BUILD_DIR) $(TEST_BUILD_DIR)

build-debug: DEFINES += -DDEBUG=1 -DMSG_LVL=$(DEBUG_LEVEL)
build-edit: DEFINES += -DEDIT=1 -DDEBUG=1 -DMSG_LVL=$(DEBUG_LEVEL)

build: checkdirs krig
build-edit: build
build-debug: build
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
	@rm -rf $(TEST_BUILD_DIR)
