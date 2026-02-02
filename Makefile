CXX 		:= g++
FLAGS 		:= -std=c++20 -O2

TARGET 		:= proxy.out
SRC_DIR 	:= src
BUILD_DIR 	:= build

SRCS 		:= $(wildcard $(SRC_DIR)/*.cpp)
BIN 		:= $(BUILD_DIR)/$(TARGET)

all: $(BIN)

$(BIN): $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(FLAGS) $(SRCS) -o $(BIN)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
