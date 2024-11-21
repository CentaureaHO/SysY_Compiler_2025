INCLUDE_DIR := include
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

CXX ?= g++

INCLUDES = -I./$(INCLUDE_DIR)

CXX_STANDARD = -std=c++17

DBGFLAGS = -g

WERROR_FLAGS := -Wall -Wextra -Wpedantic -Werror

WARNINGS_IGNORE := -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-value

CXXFLAGS = $(CXX_STANDARD) $(INCLUDES) $(WERROR_FLAGS) $(DBGFLAGS) $(WARNINGS_IGNORE) -O3

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp' -o -name '*.c')

OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SOURCES:.cpp=.o))
OBJECTS := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(OBJECTS:.c=.o))

TEST_SRC_DIR := testcase/source
TEST_BUILD_DIR := testcase/build

LEXER_SRC := parser/lexer.l
LEXER_C_T := parser/lexer.cpp
LEXER_C := $(SRC_DIR)/parser/lexer.cpp

BISON_SRC := parser/yacc.y
BISON_C_T := parser/yacc.cpp
BISON_C := $(SRC_DIR)/parser/yacc.cpp
BISON_H_T := parser/yacc.hpp
BISON_H := $(INCLUDE_DIR)/parser/yacc.hpp

LOC_H_T := parser/location.hh
LOC_H := $(INCLUDE_DIR)/parser/location.hh

VAL_OPTS := --leak-check=full --track-origins=yes -s

.PHONY: all
all: obj bin

.PHONY: obj
obj: $(OBJECTS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: bin
bin: $(BIN_DIR)/SysYc

$(BIN_DIR)/SysYc: $(OBJECTS) main.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) main.cpp $(OBJECTS) -o $(BIN_DIR)/SysYc

.PHONY: test
test: $(BIN_DIR)/test

$(BIN_DIR)/test: $(OBJECTS) test.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) test.cpp $(OBJECTS) -o $(BIN_DIR)/test

.PHONY: lexer_test
lexer_test: $(BIN_DIR)/lexer_test

$(BIN_DIR)/lexer_test: $(OBJECTS) lexer_test.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) lexer_test.cpp $(OBJECTS) -o $(BIN_DIR)/lexer_test

.PHONY: valgrind
valgrind: $(BIN_DIR)/test
	valgrind $(VAL_OPTS) $(BIN_DIR)/SysYc -llvm -o test.ll test.in 2> valgrind_report

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TEST_BUILD_DIR) $(BIN_DIR)

.PHONY: lexer
lexer: $(LEXER_C) $(BISON_C) $(BISON_H) $(LOC_H)

$(LEXER_C): $(LEXER_SRC)
	flex --c++ --outfile=$(LEXER_C_T) $(LEXER_SRC)
	sed -i 's|#include "yacc.hpp"|#include <parser/yacc.hpp>|' $(LEXER_C_T)
	mv $(LEXER_C_T) $(LEXER_C)

$(BISON_C) $(BISON_H) $(LOC_H): $(BISON_SRC)
	bison -d --language=c++ --defines=$(BISON_H_T) -o $(BISON_C_T) $(BISON_SRC)
	sed -i 's|#include "yacc.hpp"|#include <parser/yacc.hpp>|' $(BISON_C_T)
	mv $(BISON_C_T) $(BISON_C)
	mv $(BISON_H_T) $(BISON_H)
	mv $(LOC_H_T) $(LOC_H)

.PHONY: format
format:
	find . -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.hh" \) -exec clang-format -i {} +