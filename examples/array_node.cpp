#include <trie.h>
#include <string>
#include <iostream>
#include <vector>
#include <cctype>

class TrieCaseInsensitiveCompare
{
public:
    bool operator()(char v1, char v2) {
        int i1 = std::tolower(v1);
        int i2 = std::tolower(v2);
        return i1 < i2;
    }
};

// key to vector index converter
// case insensitive and includes alphabets, space and end symbol
class AlphaToIndex
{
public:
    unsigned int operator()(const char & c) const {
        unsigned int index = 0;
        if (c == ' ') {
            index = 27;
        } else if (c >= 'A' && c <= 'Z') {
            index = c - 'A' + 1;
        } else if (c >= 'a' && c <= 'z') {
            index = c - 'a' + 1;
        }
        return index;
    }
};

int main(int argc, char ** argv) {

    rtv::Trie<char, std::string,
              TrieCaseInsensitiveCompare,
              rtv::VectorItems<char, std::string, TrieCaseInsensitiveCompare,
                               28,  // size of each node (alphabets + endsymbol + space)
                               AlphaToIndex // key symbol to index converter
                              >
              > dictionary('\0');

    // adding key value pair to the Trie
    if (dictionary.insert("karma", "Destiny or fate, following as effect from cause").second) {
        std::cout << "added karma" << std::endl;
    }

    // removing key from the Trie
    if (dictionary.erase("karma")) {
        std::cout << "removed karma" << std::endl;
    }

    return 0;
}
