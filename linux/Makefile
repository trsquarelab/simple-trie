bin_PROGRAMME = trie_test
trie_test_SOURCES = ../src/test.cpp
trie_test_OBJ = test.o
CXX = g++
CXFLAGS = -g3
DEPS = ../inc/trie.h

all: $(bin_PROGRAMME)
	

$(bin_PROGRAMME): $(trie_test_OBJ)
	$(CXX) -o $(bin_PROGRAMME) $(trie_test_OBJ) $(CXFLAGS)

$(trie_test_OBJ): $(trie_test_SOURCES) $(DEPS)
	$(CXX) -c -o $(trie_test_OBJ) $< $(CXFLAGS) -I../inc

clean:
	rm -rf $(bin_PROGRAMME) $(trie_test_OBJ)
