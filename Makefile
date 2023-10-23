CPP = g++
CPPFLAGS = -Wall -std=c++17

PROGRAMS = sequential_server \
					multithreaded_server

all: $(PROGRAMS)

sequential_server: utils.cpp sequential_server.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@

multithreaded_server: utils.cpp multithreaded_server.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@

clean:
	rm -rf $(PROGRAMS) *.o *.a a.out *.err *~

format:
	clang-format -style=LLVM -i *.cpp *.h