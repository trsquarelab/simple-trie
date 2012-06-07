#include <trie.h>
#include <string>
#include <iostream>
#include <vector>

int main(int argc, char ** argv)
{

    rtv::Trie<char, std::string> dictionary('\0');

    dictionary.insert("karma$", "Destiny or fate, following as effect from cause");

    rtv::Trie<char, std::string>::Iterator iter = dictionary.begin();
    rtv::Trie<char, std::string>::Iterator iend = dictionary.end();

    std::cout << "Iterator with ++" << std::endl;
    for (; iter != iend; ++iter) {
        std::cout << iter->first  << " " << iter->second->c_str() << std::endl;
    }

    const rtv::Trie<char, std::string> & const_ref_dictionary = dictionary;
    rtv::Trie<char, std::string>::ConstIterator const_iter = const_ref_dictionary.begin();
    rtv::Trie<char, std::string>::ConstIterator const_iend = const_ref_dictionary.end();

    std::cout << "ConstIterator with ++" << std::endl;
    for (; const_iter != const_iend; ++const_iter) {
        std::cout << const_iter->first  << " " << const_iter->second->c_str() << std::endl;
    }

    std::cout << "Iterator with --" << std::endl;
    iter = dictionary.end();
    for (--iter; iter != iend; --iter) {
        std::cout << iter->first  << " " << iter->second->c_str() << std::endl;
    }


    return 0;
}
