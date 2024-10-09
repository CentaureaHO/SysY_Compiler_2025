CXX ?= g++

INCLUDES = -I./include -I./parser

CXX_STANDARD = -std=c++17

DBGFLAGS = -g

WERROR_FLAGS := -Wall -Wextra -Wpedantic -Werror

WARNINGS_IGNORE := -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-value

CXXFLAGS = $(CXX_STANDARD) $(INCLUDES) $(WERROR_FLAGS) $(DBGFLAGS) $(WARNINGS_IGNORE)

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp' -o -name '*.c')

OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SOURCES:.cpp=.o))
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(OBJECTS:.c=.o))

TEST_SRC_DIR := testcase/source
TEST_BUILD_DIR := testcase/build

LEXER_SRC := parser/lexer.l
LEXER_C := parser/lexer.c
LEXER_H := parser/lexer.h

BISON_SRC := parser/yacc.y
BISON_C := parser/yacc.c
BISON_H := parser/yacc.h

TOKEN_H := include/parse/token.h

PARSER_OBJECTS := $(OBJ_DIR)/parser/yacc.o $(OBJ_DIR)/parser/lexer.o
OBJECTS += $(PARSER_OBJECTS)

.PHONY: all
all: obj

.PHONY: obj
obj: $(OBJECTS)

$(OBJ_DIR)/parser/%.o: parser/%.c
	@mkdir -p $(OBJ_DIR)/parser
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TEST_BUILD_DIR) $(LEXER_C) $(LEXER_H) $(BISON_C) $(BISON_H) $(TOKEN_H) $(BIN_DIR)

.PHONY: testLexicalAnalysis
testLexicalAnalysis: $(OBJ_DIR)/parser/yacc.o $(OBJ_DIR)/parser/lexer.o $(OBJ_DIR)/common/str_convert.o $(OBJ_DIR)/parse/token.o
	@mkdir -p $(TEST_BUILD_DIR)/1_lexicalAnalyzer
	$(CXX) $(CXXFLAGS) $(TEST_SRC_DIR)/1_lexicalAnalyzer/lexAnalyzer.cpp \
		$(OBJ_DIR)/parser/yacc.o $(OBJ_DIR)/parser/lexer.o $(OBJ_DIR)/common/str_convert.o $(OBJ_DIR)/parse/token.o \
		-o $(TEST_BUILD_DIR)/1_lexicalAnalyzer/lexAnalyzer
	@echo "Compiled test: lexAnalyzer"

.PHONY: lexer
lexer: $(LEXER_C) $(LEXER_H)

$(LEXER_C) $(LEXER_H): $(LEXER_SRC)
	bison -d --defines=$(BISON_H) -o $(BISON_C) $(BISON_SRC)
	flex --outfile=$(LEXER_C) --header-file=$(LEXER_H) $(LEXER_SRC)
	python3 parser/genfunc.py $(BISON_H) $(TOKEN_H)