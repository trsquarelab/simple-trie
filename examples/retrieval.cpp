#include <trie.h>
#include <string>
#include <iostream>

int main(int argc, char ** argv) {

    rtv::Trie<char, std::string> dictionary('$');

    dictionary.insert("karma$", "Destiny or fate, following as effect from cause");

    if (dictionary.hasKey("karma$")) {
     std::cout << "key karma found" << std::endl;
    }
    std::string * result = dictionary.get("karma$");
    if (result) {
        std::cout << "Found karma using get" << std::endl;
        std::cout << "karma:" << result->c_str() << std::endl;
    }

    rtv::Trie<char, std::string>::Iterator iter = dictionary.find("karma$");
    if (iter != dictionary.end()) {
        std::cout << "Found karma using find" << std::endl;
        int i = 0;
        for (; iter->first[i] != dictionary.endSymbol(); ++i) {
        }
        std::string keyWithNullTermination(iter->first, i);
        std::cout << keyWithNullTermination.c_str() << ":"<< iter->second->c_str() << std::endl;
    }


    return 0;
}
