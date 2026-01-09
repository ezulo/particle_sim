CXX := clang++
SRC_DIR := src
BUILD_DIR := target
INCLUDE_DIR := include

SRCS := $(wildcard $(SRC_DIR)/*.cpp)

# Serial build
OBJS_SERIAL := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/serial/%.o,$(SRCS))
DEPS_SERIAL := $(OBJS_SERIAL:.o=.d)

# OpenMP build
OBJS_OPENMP := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/openmp/%.o,$(SRCS))
DEPS_OPENMP := $(OBJS_OPENMP:.o=.d)

CXXFLAGS_BASE := -g -Wall -Wextra -I$(INCLUDE_DIR) -MMD -MP
CXXFLAGS_SERIAL := $(CXXFLAGS_BASE)
CXXFLAGS_OPENMP := $(CXXFLAGS_BASE) -fopenmp -DUSE_OPENMP

LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

.PHONY: all openmp clean

all: run

openmp: run-openmp

$(BUILD_DIR)/serial:
	mkdir -p $(BUILD_DIR)/serial

$(BUILD_DIR)/openmp:
	mkdir -p $(BUILD_DIR)/openmp

$(BUILD_DIR)/serial/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/serial
	$(CXX) $(CXXFLAGS_SERIAL) -c $< -o $@

$(BUILD_DIR)/openmp/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)/openmp
	$(CXX) $(CXXFLAGS_OPENMP) -c $< -o $@

run: $(OBJS_SERIAL)
	$(CXX) $^ $(LDLIBS) -o $@

run-openmp: $(OBJS_OPENMP)
	$(CXX) -fopenmp $^ $(LDLIBS) -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f run run-openmp

-include $(DEPS_SERIAL)
-include $(DEPS_OPENMP)
