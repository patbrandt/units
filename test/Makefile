#CXX=/path/to/compiler
CFLAGS=-std=c++11
INCLUDES=-I$(GTEST_DIR)/include -I../src
LDFLAGS=-pthread -lgtest_main

test: test.cpp
	$(CXX) $(CFLAGS) test.cpp -o test $(INCLUDES) $(LDFLAGS)
	./test

all: test

clean:
	rm test -f
