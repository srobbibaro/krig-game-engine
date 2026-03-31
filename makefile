OS_NAME       := $(shell uname -s)
HOMEBREW_DIR  := $(shell brew --prefix)
DEBUG_LEVEL   := 2
GCOV          := gcov


CC            := g++
CFLAGS        := -O3 -std=c++11
INCLUDES      := -I./src/ -I/usr/include/
TEST_INCLUDES := -I./src/ -I/usr/include/
LIBS          := -lluajit -lvorbisfile -lvorbisenc -lvorbis -logg -lopenal -lpng -lglut -lGL -lGLU
LDFLAGS       :=
DEFINES       :=

ifeq ($(OS_NAME), Darwin)
	INCLUDES      += -I$(HOMEBREW_DIR)/include/ -I$(HOMEBREW_DIR)/opt/openal-soft/include/
	TEST_INCLUDES += -I$(HOMEBREW_DIR)/include/ -I$(HOMEBREW_DIR)/opt/openal-soft/include/
	LDFLAGS       := -L$(HOMEBREW_DIR)/lib -L$(HOMEBREW_DIR)/opt/libglu/lib -L$(HOMEBREW_DIR)/opt/openal-soft/lib/ -L$(HOMEBREW_DIR)/opt/freeglut/lib/
	GCOV          := ./build-scripts/llvm-cov-gcov.sh
endif

SRC_DIR        := ./src/
BUILD_DIR      := ./obj/src/
TEST_SRC_DIR   := ./test/
TEST_BUILD_DIR := ./obj/test/

COV_BUILD_DIR      := ./obj-cov/src/
COV_TEST_BUILD_DIR := ./obj-cov/test/
COVERAGE_DIR       := ./coverage/

COV_FLAGS := -O0 -std=c++11 --coverage

SRC = $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)*.cpp))
OBJ = $(patsubst $(SRC_DIR)%.cpp, $(BUILD_DIR)%.o, $(SRC))
OBJ_WITHOUT_MAIN := $(filter-out $(BUILD_DIR)main.o, $(OBJ))

TEST_SRC = $(foreach sdir, $(TEST_SRC_DIR), $(wildcard $(sdir)*.cpp))
TEST_OBJ = $(patsubst $(TEST_SRC_DIR)%.cpp, $(TEST_BUILD_DIR)%.o, $(TEST_SRC))

COV_OBJ              = $(patsubst $(SRC_DIR)%.cpp, $(COV_BUILD_DIR)%.o, $(SRC))
COV_OBJ_WITHOUT_MAIN := $(filter-out $(COV_BUILD_DIR)main.o, $(COV_OBJ))
COV_TEST_OBJ         = $(patsubst $(TEST_SRC_DIR)%.cpp, $(COV_TEST_BUILD_DIR)%.o, $(TEST_SRC))

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

$(COV_BUILD_DIR)%.o: $(SRC_DIR)%.cpp
	$(CC) $(COV_FLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

$(COV_TEST_BUILD_DIR)%.o: $(TEST_SRC_DIR)%.cpp
	$(CC) $(COV_FLAGS) $(TEST_INCLUDES) $(DEFINES) -c $< -o $@

checkdirs: $(BUILD_DIR) $(TEST_BUILD_DIR)

build-debug: DEFINES += -DDEBUG=1 -DMSG_LVL=$(DEBUG_LEVEL)
build-edit: DEFINES += -DEDIT=1 -DDEBUG=1 -DMSG_LVL=$(DEBUG_LEVEL)

build: checkdirs krig
build-edit: build
build-debug: build
build-test: build unit-test

unit-test-cov: $(COV_TEST_OBJ) $(COV_OBJ_WITHOUT_MAIN)
	$(CC) $^ $(LIBS) $(LDFLAGS) --coverage -o $@

checkdirs-cov: $(COV_BUILD_DIR) $(COV_TEST_BUILD_DIR)

build-coverage: checkdirs-cov unit-test-cov

run-coverage: build-coverage
	./unit-test-cov
	lcov --gcov-tool $(GCOV) --capture \
	     --directory $(COV_BUILD_DIR) \
	     --directory $(COV_TEST_BUILD_DIR) \
	     --output-file coverage.info \
	     --ignore-errors unsupported,range,inconsistent,format,count,unused
	lcov --gcov-tool $(GCOV) --remove coverage.info \
	     '/usr/*' '*/opt/*' '*/test/catch*' \
	     --output-file coverage.info \
	     --ignore-errors unsupported,range,inconsistent,format,count,unused
	genhtml coverage.info --output-directory $(COVERAGE_DIR) \
	         --ignore-errors inconsistent,corrupt,category
	@echo "Report: open $(COVERAGE_DIR)index.html"

clean-coverage:
	@rm -rf $(COV_BUILD_DIR)
	@rm -rf $(COV_TEST_BUILD_DIR)
	@rm -f unit-test-cov coverage.info
	@rm -rf $(COVERAGE_DIR)

run-tests: build-test
	./unit-test

run-demo: build
	./krig krig3

doc:
	doxygen docs/doxyfile

clean-doc:
	@rm -rf ./doc/

$(BUILD_DIR):
	@mkdir -p $@

$(TEST_BUILD_DIR):
	@mkdir -p $@

$(COV_BUILD_DIR):
	@mkdir -p $@

$(COV_TEST_BUILD_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(TEST_BUILD_DIR)
