CXX ?= g++

INCLUDES = -I./include

CXX_STANDARD = -std=c++17

DBGFLAGS = -g

WERROR_FLAGS := -Wall -Wextra -Wpedantic -Werror

WARNINGS_IGNORE := -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-value

CXXFLAGS = $(CXX_STANDARD) $(INCLUDES) $(WERROR_FLAGS) $(DBGFLAGS) $(WARNINGS_IGNORE)

SRC_DIR := src
BUILD_DIR := build

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp' -o -name '*.c')

OBJECTS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.cpp=.o))
OBJECTS := $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(OBJECTS:.c=.o))

TEST_SRC_DIR := testcase/source
TEST_BUILD_DIR := testcase/build

LEXER_SRC := parser/lexer.l
LEXER_C := src/parser/lexer.c
LEXER_H := include/parser/lexer.h

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
	rm -rf $(BUILD_DIR) $(TEST_BUILD_DIR) $(LEXER_C) $(LEXER_H)

.PHONY: testLexicalAnalysis
testLexicalAnalysis: $(BUILD_DIR)/parser/lexer.o $(BUILD_DIR)/parser/token.o $(BUILD_DIR)/common/str_convert.o $(BUILD_DIR)/parser/token_parser.o
	@mkdir -p $(TEST_BUILD_DIR)/1_lexicalAnalyzer
	$(CXX) $(CXXFLAGS) $(TEST_SRC_DIR)/1_lexicalAnalyzer/lexAnalyzer.cpp \
		$(BUILD_DIR)/parser/lexer.o $(BUILD_DIR)/parser/token.o $(BUILD_DIR)/common/str_convert.o $(BUILD_DIR)/parser/token_parser.o \
		-o $(TEST_BUILD_DIR)/1_lexicalAnalyzer/lexAnalyzer
	@echo "Compiled test: lexAnalyzer"

.PHONY: lexer
lexer: $(LEXER_C) $(LEXER_H)

$(LEXER_C) $(LEXER_H): $(LEXER_SRC)
	flex --noyywrap --nounput --outfile $(LEXER_C) --header-file=$(LEXER_H) $(LEXER_SRC)