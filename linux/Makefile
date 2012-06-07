bin_PROGRAMME = trie_test_$(shell uname -s)_$(shell uname -m)
trie_test_SOURCES = ../test/trietest1.cpp  ../test/trietest2.cpp  ../test/trietestmain.cpp
trie_test_OBJ = $(patsubst ../test/%.cpp, %.o, $(trie_test_SOURCES))
CXX = g++
CXFLAGS = -g3 -Wall
DEPS = ../inc/trie.h ../test/trietest.h

all: $(bin_PROGRAMME)
	cd ../examples/linux && make	
	

$(bin_PROGRAMME): $(trie_test_SOURCES) $(DEPS)
	$(CXX) -o $(bin_PROGRAMME) $(trie_test_SOURCES) -I../inc $(CXFLAGS)

#$(trie_test_OBJ): $(trie_test_SOURCES) $(DEPS)
#	$(CXX) -c -o $@ $< $(CXFLAGS) -I../inc

clean:
	rm -rf $(bin_PROGRAMME) $(trie_test_OBJ)
	cd ../examples/linux && make clean

