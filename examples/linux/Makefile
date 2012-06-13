array_node = array_node_$(shell uname -s)_$(shell uname -m)
array_node_SOURCE = ../array_node.cpp

common_prefix = common_prefix_$(shell uname -s)_$(shell uname -m)
common_prefix_SOURCE = ../common_prefix.cpp

insert_erase =  insert_erase_$(shell uname -s)_$(shell uname -m) 
insert_erase_SOURCE = ../insert_erase.cpp

iterators = iterators_$(shell uname -s)_$(shell uname -m) 
iterators_SOURCE = ../iterators.cpp

retrieval = retrieval_$(shell uname -s)_$(shell uname -m)
retrieval_SOURCE = ../retrieval.cpp

trie_interactive = trie_interactive_$(shell uname -s)_$(shell uname -m)
trie_interactive_SOURCE = ../trie_interactive.cpp

bin_PROGRAMMES = $(array_node) \
		 $(common_prefix) \
		 $(insert_erase) \
		 $(iterators) \
		 $(retrieval) \
		 $(trie_interactive)

CXX = g++
CXFLAGS = -g3 -Wall -I../../inc
DEPS = ../../inc/trie.h

all: $(bin_PROGRAMMES)
	
$(array_node): $(array_node_SOURCE) $(DEPS)
	$(CXX) -o $@ $< $(CXFLAGS)

$(common_prefix): $(common_prefix_SOURCE) $(DEPS)
	$(CXX) -o $@ $< $(CXFLAGS)

$(insert_erase): $(insert_erase_SOURCE) $(DEPS) 
	$(CXX) -o $@ $< $(CXFLAGS)

$(iterators): $(iterators_SOURCE) $(DEPS) 
	$(CXX) -o $@ $< $(CXFLAGS)

$(retrieval): $(retrieval_SOURCE) $(DEPS) 
	$(CXX) -o $@ $< $(CXFLAGS)

$(trie_interactive): $(trie_interactive_SOURCE) $(DEPS) 
	$(CXX) -o $@ $< $(CXFLAGS)

clean:
	rm -rf $(bin_PROGRAMMES)

