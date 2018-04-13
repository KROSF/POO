CXX = clang++
STD = c++11
CXXFLAGS = -std=${STD} -g -pedantic -Wall
CPPFLAGS = -DP1 -I../Tests-auto -I.
VPATH = ../Tests-auto:.
RM = rm -f
SRCPP = fecha.cpp cadena.cpp test-caso0-fecha-auto.cpp test-caso0-cadena-auto.cpp test-auto.cpp test-P1-consola.cpp
OBJS := $(SRCPP:.cpp=.o)
EXES = test-P1-auto test-P1-consola

all: $(EXES)

test-consola: test-P1-consola
	@./$<

test-auto: test-P1-auto
	@./$<

test-P1-consola: test-P1-consola.o fecha.o cadena.o
	@echo "Enlazando Objetos"
	@$(CXX) $(CXXFLAGS) $^ -o $@

test-P1-auto: test-caso0-fecha-auto.o test-caso0-cadena-auto.o test-auto.o cadena.o fecha.o
	@echo "Enazando Objetos"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS)  $^ -o $@

$(OBJS): $(SRCPP)

check: cadena_check fecha_check
	./cadena_check cadena.cpp -- -std=${STD}
	./fecha_check fecha.cpp -- -std=${STD}

cadena_check fecha_check: cadena_check.cpp fecha_check.cpp
	@$(MAKE) -f Make_check.mk
.SILENT: $(OBJS)
.PHONY: clean
clean:
	@echo "Limpiando ..."
	@$(MAKE) -f Make_check.mk clean
	@$(RM) $(OBJS) $(EXE)
