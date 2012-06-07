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

bin_PROGRAMMES = $(array_node) \
		 $(common_prefix) \
		 $(insert_erase) \
		 $(iterators) \
		 $(retrieval)
CXX = g++
CXFLAGS = -g3 -Wall -I../../inc
DEPS = ../../inc/trie.h

all: $(bin_PROGRAMMES)
	

$(array_node):  $(DEPS) $(array_node_SOURCE)
	$(CXX) -o $@ $(array_node_SOURCE) $(CXFLAGS)

$(common_prefix):  $(DEPS)
	$(CXX) -o $@ $(common_prefix_SOURCE) $(CXFLAGS)

$(insert_erase):  $(DEPS) $(insert_erase_SOURCE)
	$(CXX) -o $@ $(insert_erase_SOURCE) $(CXFLAGS)

$(iterators):  $(DEPS) $(iterators_SOURCE)
	$(CXX) -o $@ $(iterators_SOURCE) $(CXFLAGS)

$(retrieval):  $(DEPS) $(retrieval_SOURCE)
	$(CXX) -o $@ $(retrieval_SOURCE) $(CXFLAGS)

clean:
	rm -rf $(bin_PROGRAMMES)

