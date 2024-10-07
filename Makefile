CXX ?= g++

INCLUDES = -I./include

CXX_STANDARD = -std=c++17

DBGFLAGS = -g

WERROR_FLAGS := -Wall -Wextra -Wpedantic

ifneq ($(MAKECMDGOALS),testSymbolTable)
    WERROR_FLAGS += -Werror
endif

CXXFLAGS = $(CXX_STANDARD) $(INCLUDES) $(WERROR_FLAGS) $(DBGFLAGS)

SRC_DIR := src
BUILD_DIR := build

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp' -o -name '*.c')

OBJECTS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.cpp=.o))
OBJECTS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(OBJECTS:.c=.o))

TEST_SRC_DIR := testcase/source
TEST_BUILD_DIR := testcase/build

.PHONY: all
all: obj

.PHONY: obj
obj: $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR)

.PHONY: testSymbolTable
testSymbolTable: $(BUILD_DIR)/symbols/symbolTable.o $(BUILD_DIR)/defs/type.o $(BUILD_DIR)/defs/type_base.o
	@mkdir -p $(TEST_BUILD_DIR)/1_symbolTable
	@for test_src in $(shell find $(TEST_SRC_DIR)/1_symbolTable -name '*.cpp'); do \
		test_name=$$(basename $$test_src .cpp); \
		$(CXX) $(CXXFLAGS) $$test_src $(BUILD_DIR)/symbols/symbolTable.o $(BUILD_DIR)/defs/type.o $(BUILD_DIR)/defs/type_base.o -o $(TEST_BUILD_DIR)/1_symbolTable/$$test_name; \
		echo "Compiled test: $$test_name"; \
	done