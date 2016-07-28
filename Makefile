SRC=src
BIN=bin

CXXFLAGS+=-I${SEQAN_INCLUDE}
CXXFLAGS+=-Ilibs
CXXFLAGS+=-Isrc
CXXFLAGS+=-std=c++11
CXXFLAGS+=-Wno-unknown-pragmas -Wno-pragmas

LDFLAGS+=-lm

default: all

all: main test

main: $(BIN)/main

test: $(BIN)/test

$(BIN)/main: main.o imgt_xml_wrapper.o
	mkdir -p bin
	$(CXX) $(LDFLAGS) -o $(BIN)/main main.o imgt_xml_wrapper.o

$(BIN)/test: test.o imgt_xml_wrapper.o
	mkdir -p bin
	$(CXX) $(LDFLAGS) -o $(BIN)/test test.o imgt_xml_wrapper.o

main.o: $(SRC)/main.cpp
	$(CXX) $(CXXFLAGS) -c -o main.o $(SRC)/main.cpp

test.o: $(SRC)/test.cpp
	$(CXX) $(CXXFLAGS) -c -o test.o $(SRC)/test.cpp

imgt_xml_wrapper.o: $(SRC)/imgt_xml_wrapper.cpp $(SRC)/imgt_xml_wrapper.hpp
	$(CXX) $(CXXFLAGS) -c -o imgt_xml_wrapper.o $(SRC)/imgt_xml_wrapper.cpp

clean:
	rm -f test.o main.o $(BIN)/test imgt_xml_wrapper.o

.PHONY: default all main test clean
