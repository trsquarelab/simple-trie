#include <trie.h>
#include <string>
#include <iostream>
#include <vector>

int main(int argc, char ** argv) {
    rtv::Trie<char, std::string> dictionary('\0');

    dictionary.insert("karma", "Destiny or fate, following as effect from cause");
    rtv::Trie<char, std::string>::Iterator iter = dictionary.startsWith("kar");

    std::cout << "Keys with prefix kar are " << std::endl;

    for (; iter != dictionary.end(); ++iter) {
     std::cout << iter->first << " : " << iter->second->c_str() << std::endl;
    }

    return 0;
}
