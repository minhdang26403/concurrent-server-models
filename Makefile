CPP = g++
CPPFLAGS = -Wall -std=c++20
SRCDIR = src
INCLUDEDIR = include
LDFLAGS = -I./$(INCLUDEDIR)

PROGRAMS = sequential_server \
					multithreaded_server \
					threadpool_server \
					concurrent_queue_test \
					thread_pool_test \

all: $(PROGRAMS)

sequential_server: $(SRCDIR)/utils.cpp $(SRCDIR)/sequential_server.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

multithreaded_server: $(SRCDIR)/utils.cpp $(SRCDIR)/multithreaded_server.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

threadpool_server: $(SRCDIR)/utils.cpp $(SRCDIR)/threadpool_server.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

concurrent_queue_test: $(SRCDIR)/concurrent_queue_test.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

thread_pool_test: $(SRCDIR)/thread_pool_test.cpp
	$(CPP) $(CPPFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -rf $(PROGRAMS) *.o *.a a.out *.err *~

format:
	clang-format -style=Google -i $(SRCDIR)/*.cpp $(INCLUDEDIR)/*.h