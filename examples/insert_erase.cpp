#include <trie.h>
#include <string>
#include <iostream>

int main(int argc, char ** argv)
{

    rtv::Trie<char, std::string> dictionary('$');

// adding key value pair to the Trie
    if (dictionary.insert("karma$", "Destiny or fate, following as effect from cause").second) {
        std::cout << "added karma" << std::endl;
    }

// removing key from the Trie
    if (dictionary.erase("karma$")) {
        std::cout << "removed karma" << std::endl;
    }

    return 0;
}
