/*
 * Copyright (c) 2012, Ranjith TV
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause ("BSD New" or "BSD Simplified") license.
 * You may obtain a copy of the License at
 *
 * http://www.opensource.org/licenses/BSD-3-Clause
 *
 */

#define HIGH_PERFORMANCE

#include "trie.h"

#include <iostream>
#include <vector>

void test(rtv::Trie<char, std::string, '$', std::less<char> >  & dictionary, std::string const & word, bool expected) {
    if (dictionary.hasKey(word.c_str())) {
        if (!expected) {
            std::cerr << "Found " << word.c_str()  << " when it should not be found !!!"<< std::endl;
        }
    } else {
        if (expected) {
            std::cerr << "Did not find " << word.c_str()  << " when it should be !!!"<< std::endl;
        }
    }
}

void add(rtv::Trie<char, std::string, '$', std::less<char> >  & dictionary, const char * word, const char * meaning) {
    dictionary.add(word, meaning);
    if (dictionary.get(word)->compare(meaning) != 0) {
        std::cerr << "Error in Trie::add!!!" << std::endl;
    }
}

struct ListCallBack {
    void operator()(std::vector<char> key, std::string value) const {
        key.push_back(0);
        std::cout.width(10);
        std::string k(&key[0]);
        k.insert(0, "[");
        k.insert(k.length(), "]");
        std::cout << std::left << k.c_str() << " : ";
        std::cout.width(0);
        std::cout.width(70);
        value.insert(0, "[");
        value.insert(value.length(), "]");
        std::cout << std::left << value.c_str() << std::endl;
    }
};

int main(int argc, char ** argv) {

    rtv::Trie<char, std::string, '$', std::less<char> > dictionary(256);
    
    add(dictionary, "Multiset$", "Multisets are associative containers with the same properties as set containers, but allowing for multiple keys with equal values");
    add(dictionary, "Deque$", "Double-ended queue");
    add(dictionary, "Multimap$", "Multimaps are a kind of associative container that stores elements formed by the combination of a key value and a mapped value, much like map containers, but allowing different elements to have the same key value");
    add(dictionary, "Stack$", "Stacks are a type of container adaptor, specifically designed to operate in a LIFO context");
    add(dictionary, "Set$", "Sets are a kind of associative container that stores unique elements, and in which the elements themselves are the keys");
    add(dictionary, "Vector$", "Vectors are a kind of sequence container. As such, their elements are ordered following a strict linear sequence");
    add(dictionary, "Array$", "Arrays are fixed-size sequence containers: they hold a specific number of elements ordered in a strict linear sequence");
    add(dictionary, "Map$", "Maps are a kind of associative container that stores elements formed by the combination of a key value and a mapped value");
    add(dictionary, "List$", "Lists are a kind of sequence container. As such, their elements are ordered following a linear sequence");
    add(dictionary, "Bitset$", "A bitset is a special container class that is designed to store bits");
    
    test(dictionary, "Map$", true);
    test(dictionary, "List$", true);
    test(dictionary, "Bitset$", true);
    test(dictionary, "Squence$", false);

    std::vector< std::pair < std::vector<char> , std::string > > result;
    dictionary.startsWith("Multi$", result);
    if (result.size() != 2) {
        std::cerr << "Error in Trie::startsWith functionality!!!" << std::endl;
    }

    bool finished = false;
    while (!finished) {
        std::cout << std::endl;
        std::cout << "******************************************" << std::endl;
        std::cout << "0 : To Exit" << std::endl;
        std::cout << "1 : Add a word " << std::endl;
        std::cout << "2 : Search a word (Enter starting letters)" << std::endl;
        std::cout << "3 : List all words " << std::endl;
        std::cout << "******************************************" << std::endl;

        std::cout << std::endl << "Enter Choice : ";

        char c;
        std::cin >> c;
        switch (c) {
            case 'q':
            case '0': {
                finished = true;
                break;
                    }
            case '1': {
                char word[80];
                char meaning[1024];
                std::cout << "Enter the word : ";
                std::cin.ignore();
                std::cin.get(word, sizeof(word), '\n');
                std::string delimWord(word);
                delimWord.insert(delimWord.length(), "$");
                std::cout << "Enter the meaning : ";
                std::cin.ignore();
                std::cin.get(meaning, sizeof(meaning), '\n');
                dictionary.add(delimWord.c_str(), meaning);
                break;
                    }
            case '2': {
                char word[80];
                std::cout << "Enter the word : ";
                std::cin.ignore();
                std::cin.get(word, sizeof(word), '\n');
                std::string delimWord(word);
                delimWord.insert(delimWord.length(), "$");
                std::cout << "Searching ... ";

                std::vector< std::pair < std::vector<char> , std::string > > result;
                dictionary.startsWith(delimWord.c_str(), result);
                std::vector< std::pair < std::vector<char> , std::string > >::iterator iter = result.begin();

                if (result.size() == 0) {
                    std::cout << "Could not find the given word";
                } else {
                    std::cout << std::endl << "*****************************************";
                    for (; iter != result.end(); ++iter) {
                        iter->first.push_back(0);
                        std::cout << std::endl << &((iter->first)[0]) << " : " << iter->second.c_str();
                    }
                    std::cout << std::endl << "*****************************************";
                }
                break;
                    }
            case '3': {
                std::cout << "*****************************************" << std::endl;
                dictionary.traverse(ListCallBack());
                std::cout << "*****************************************" << std::endl;
                break;
                    }
            default: {
                std::cout << "Invalid choice please try again" << std::endl;
                break;
                     }
        }
    }

    return 0;
}
