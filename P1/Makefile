# # # # # # # # # # # # # # # # # # # # # # # # # #
# @file Makefile                                  #
# @author Carlos Rodrigo Sanabria Flores          #
# @date 27 Apr 2018                               #
# @copyright 2018 Carlos Rodrigo Sanabria Flores  #
# # # # # # # # # # # # # # # # # # # # # # # # # #
CXX = clang++
STD = c++17
CXXFLAGS = -std=${STD} -g -pedantic -Wall -Wextra -Wfloat-equal -Wundef\
	-Wcast-align -Wwrite-strings -Wmissing-declarations -Wredundant-decls\
	-Wshadow -Woverloaded-virtual
CPPFLAGS = -DP1 -I../Tests-auto -I.
VPATH = ../Tests-auto:.
RM = rm -f
SRCPP = fecha.cpp cadena.cpp test-caso0-fecha-auto.cpp\
	test-caso0-cadena-auto.cpp test-auto.cpp test-P1-consola.cpp
OBJS := $(SRCPP:.cpp=.o)
MAIN_EXE = test-P1-consola
TEST_EXE = test-P1-auto

CHECK_SRC = fecha_check.cpp cadena_check.cpp
CHECK_EXE = ${CHECK_SRC:.cpp=}

all: ${TEST_EXE} ${MAIN_EXE}

test-consola: ${MAIN_EXE}
	@./$<

test-auto: ${TEST_EXE}
	@./$<

valgrind: ${MAIN_EXE} ${TEST_EXE}
	valgrind --quiet ./${TEST_EXE} --logger=minimal
	valgrind --quiet ./${MAIN_EXE}

${MAIN_EXE}: test-P1-consola.o fecha.o cadena.o
	@echo "Enlazando Objetos test-P1-consola"
	@$(CXX) $(CXXFLAGS) $^ -o $@

${TEST_EXE}: test-caso0-fecha-auto.o test-caso0-cadena-auto.o\
	test-auto.o cadena.o fecha.o
	@echo "Enazando Objetos test-P1-auto"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS)  $^ -o $@

$(OBJS): $(SRCPP)

check: $(CHECK_EXE)
	@./cadena_check cadena.cpp -- -std=${STD}
	@./fecha_check fecha.cpp -- -std=${STD}

$(CHECK_EXE): $(CHECK_SRC)
	@$(MAKE) -f Make_check.mk

.PHONY: all test-consola test-auto check valgrind clean
clean:
	@echo "Limpiando ..."
	@$(MAKE) -f Make_check.mk clean
	@$(RM) $(OBJS) $(MAIN_EXE) $(TEST_EXE)
