#CXX=/path/to/compiler
#GTEST_DIR=/path/to/googletest
#LIBS=$(GTEST_DIR)/lib/libgtest.a
CFLAGS=-std=c++11
INCLUDES=-I$(GTEST_DIR)/include -I../src
LDFLAGS=-pthread

test: test.cpp
	$(CXX) $(CFLAGS) $(INCLUDES) $(LIBS) $(LDFLAGS) test.cpp -o test
	./test

all: test

clean:
	rm test -f
