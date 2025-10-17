CC := clang++
SRC_DIR := src
BUILD_DIR := target
INCLUDE_DIR := include
LIB_DIR := lib
EXEC := run

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(addprefix $(BUILD_DIR)/,$(patsubst $(SRC_DIR)/%.cpp,%.o,$(SRCS)))
LINK_SFML := -lsfml-graphics -lsfml-window -lsfml-system
CC_FLAGS := -Wall -Wextra -I$(INCLUDE_DIR) $(LINK_SFML)

all: $(EXEC)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CC) $(CC_FLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm $(EXEC)

