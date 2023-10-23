CPP = g++
CPPFLAGS = -Wall -std=c++17

PROGRAMS = sequential_server

all: $(PROGRAMS)

sequential_server: utils.cpp sequential_server.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@
