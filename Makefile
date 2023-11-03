CPP = g++
CPPFLAGS = -Wall -std=c++20
SRCDIR = src
LDFLAGS = -I./include

PROGRAMS = sequential_server \
					multithreaded_server

all: $(PROGRAMS)

sequential_server: $(SRCDIR)/utils.cpp $(SRCDIR)/sequential_server.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

multithreaded_server: $(SRCDIR)/utils.cpp $(SRCDIR)/multithreaded_server.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(PROGRAMS) *.o *.a a.out *.err *~

format:
	clang-format -style=LLVM -i *.cpp *.h