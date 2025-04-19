INC_DIR := include optimize utils/include
SRC_DIR := src optimize utils/src
OBJ_DIR := obj
BIN_DIR := bin

CXX ?= clang++

INCLUDES = $(addprefix -I, $(INC_DIR))

CXX_STANDARD = -std=c++17

DBGFLAGS = -g

WERROR_FLAGS := -Wall -Wextra -Wpedantic

WARNINGS_IGNORE := -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-value

CXXFLAGS = $(CXX_STANDARD) $(INCLUDES) $(WERROR_FLAGS) $(DBGFLAGS) $(WARNINGS_IGNORE) -O3

SOURCES :=
$(foreach dir,$(SRC_DIR),$(eval SOURCES += $(shell find $(dir) -name '*.cpp' -o -name '*.c')))

OBJECTS := $(SOURCES:.cpp=.o)
OBJECTS := $(OBJECTS:.c=.o)
OBJECTS := $(patsubst src/%,$(OBJ_DIR)/%,$(OBJECTS))
OBJECTS := $(patsubst optimize/%,$(OBJ_DIR)/optimize/%,$(OBJECTS))
OBJECTS := $(patsubst utils/src/%,$(OBJ_DIR)/utils/%,$(OBJECTS))

TEST_SRC_DIR := testcase/source
TEST_BUILD_DIR := testcase/build

LEXER_SRC := parser/lexer.l
LEXER_C_T := parser/lexer.cpp
LEXER_C := src/parser/lexer.cpp

BISON_SRC := parser/yacc.y
BISON_C_T := parser/yacc.cpp
BISON_C := src/parser/yacc.cpp
BISON_H_T := parser/yacc.hpp
BISON_H := include/parser/yacc.hpp

LOC_H_T := parser/location.hh
LOC_H := include/parser/location.hh

VAL_OPTS := --leak-check=full --track-origins=yes -s

.PHONY: all
all: obj bin

.PHONY: obj
obj: $(OBJECTS)

$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(CXX) $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/optimize/%.o: optimize/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(CXX) $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/utils/%.o: utils/src/%.cpp
	@mkdir -p $(dir $@)
	@echo "$(CXX) $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo "$(CXX) $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/optimize/%.o: optimize/%.c
	@mkdir -p $(dir $@)
	@echo "$(CXX) $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/utils/%.o: utils/src/%.c
	@mkdir -p $(dir $@)
	@echo "$(CXX) $< -> $@"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: bin
bin: $(BIN_DIR)/SysYc

$(BIN_DIR)/SysYc: $(OBJECTS) main.cpp
	@mkdir -p $(BIN_DIR)
	@echo "$(CXX) main.cpp -> $(BIN_DIR)/SysYc"
	@$(CXX) $(CXXFLAGS) main.cpp $(OBJECTS) -o $(BIN_DIR)/SysYc $(INCLUDES)

.PHONY: test
test: $(BIN_DIR)/test

$(BIN_DIR)/test: $(OBJECTS) test.cpp
	@mkdir -p $(BIN_DIR)
	@echo "$(CXX) test.cpp -> $(BIN_DIR)/test"
	@$(CXX) $(CXXFLAGS) test.cpp $(OBJECTS) -o $(BIN_DIR)/test

.PHONY: valgrind
valgrind: $(BIN_DIR)/test
	@valgrind $(VAL_OPTS) $(BIN_DIR)/SysYc -llvm -o test.ll test.in 2> valgrind_report

.PHONY: clean
clean:
	@rm -rf $(OBJ_DIR) $(TEST_BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned"

.PHONY: lexer
lexer: $(LEXER_C) $(BISON_C) $(BISON_H) $(LOC_H)

$(LEXER_C): $(LEXER_SRC)
	@flex --c++ --outfile=$(LEXER_C_T) $(LEXER_SRC)
	sed -i 's|#include "yacc.hpp"|#include <parser/yacc.hpp>|' $(LEXER_C_T)
	@mv $(LEXER_C_T) $(LEXER_C)

$(BISON_C) $(BISON_H) $(LOC_H): $(BISON_SRC)
	@bison -d --language=c++ --defines=$(BISON_H_T) -o $(BISON_C_T) $(BISON_SRC)
	sed -i 's|#include "yacc.hpp"|#include <parser/yacc.hpp>|' $(BISON_C_T)
	@mv $(BISON_C_T) $(BISON_C)
	@mv $(BISON_H_T) $(BISON_H)
	@mv $(LOC_H_T) $(LOC_H)

.PHONY: format
format:
	@find . -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" -o -name "*.hh" \) -exec clang-format -i {} +
