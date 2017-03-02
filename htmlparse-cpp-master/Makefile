CXX=clang++
LXX=clang++
RM=rm -fr
PLATFLAGS=-m32
CXXFLAGS=-Wall -std=c++11 -stdlib=libc++ -c
LXXFLAGS=-std=c++11 -stdlib=libc++
OPTFLAGS=-g
EXE=test_parse
OBJS=test_parse.o htmldoc.o blob.o tag.o parseutils.o
all: build_test_parse

clean:
	$(RM) $(OBJS) $(EXE) *.dSYM 2> /dev/null

build_test_parse: $(OBJS)
	$(LXX) $(OBJS) $(PLATFLAGS) $(LXXFLAGS) $(OPTFLAGS) -o $(EXE)

%.o: %.cpp
	$(CXX) $< $(PLATFLAGS) $(CXXFLAGS) $(OPTFLAGS) -o $@

blob.o: blob.hpp

tag.o: tag.hpp

htmldoc.o: htmldoc.hpp
