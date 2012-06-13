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

#include <iostream>
#include "trie.h"

class TrieCaseInsensitiveCompare
{
public:
    bool operator()(char v1, char v2) {
        int i1 = std::tolower(v1);
        int i2 = std::tolower(v2);
        return i1 < i2;
    }
};
const std::pair<std::string, std::string> sampleValues[] = {
            std::make_pair(std::string("Array"), std::string("Arrays are fixed-size sequence containers: they hold a specific number of elements ordered in a strict linear sequence")),
            std::make_pair(std::string("Bitset"), std::string("A bitset is a special container class that is designed to store bits")),
            std::make_pair(std::string("Deque"), std::string("Double-ended queue")),
            std::make_pair(std::string("List"), std::string("Lists are a kind of sequence container. As such, their elements are ordered following a linear sequence")),
            std::make_pair(std::string("Map"), std::string("Maps are a kind of associative container that stores elements formed by the combination of a key value and a mapped value")),
            std::make_pair(std::string("Maps"), std::string("A group of Map")),
            std::make_pair(std::string("Multimap"), std::string("Multimaps are a kind of associative container that stores elements formed by the combination of a key value and a mapped value, much like map containers, but allowing different elements to have the same key value")),
            std::make_pair(std::string("Multiset"), std::string("Multisets are associative containers with the same properties as set containers, but allowing for multiple keys with equal values")),
            std::make_pair(std::string("Set"), std::string("Sets are a kind of associative container that stores unique elements, and in which the elements themselves are the keys")),
            std::make_pair(std::string("Stack"), std::string("Stacks are a type of container adaptor, specifically designed to operate in a LIFO context")),
            std::make_pair(std::string("Vector"), std::string("Vectors are a kind of sequence container. As such, their elements are ordered following a strict linear sequence"))
        };


int main(int argc, char **argv)
{

    typedef rtv::Trie<char, std::string, TrieCaseInsensitiveCompare> Dictionary;
    typedef Dictionary::Iterator Iterator;
    typedef Dictionary::Iterator ConstIterator;

    Dictionary dictionary('\0');

    for (unsigned int i = 0; i < sizeof(sampleValues) / sizeof(sampleValues[0]); ++i) {
        dictionary.insert(sampleValues[i].first.c_str(), sampleValues[i].second);
    }


    bool finished = false;
    while (!finished) {
        std::cout << std::endl;
        std::cout << "******************************************" << std::endl;
        std::cout << "0 : To Exit" << std::endl;
        std::cout << "1 : Add a word " << std::endl;
        std::cout << "2 : Remove a word " << std::endl;
        std::cout << "3 : Get a word " << std::endl;
        std::cout << "4 : Search a word (Enter starting letters)" << std::endl;
        std::cout << "5 : List all words " << std::endl;
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
            std::cout << "Enter the meaning : ";
            std::cin.ignore();
            std::cin.get(meaning, sizeof(meaning), '\n');
            std::pair<Iterator, bool> result = dictionary.insert(&word[0], meaning);
            if (result.second) {
                std::cout << "Inserted" << std::endl;
            } else {
                std::cout << "Insertion failed! key already present" << std::endl;
                std::cout << result.first->first << " : " << result.first->second->c_str() << std::endl;
            }
            break;
        }
        case '2': {
            char word[80];
            std::cout << "Enter the word : ";
            std::cin.ignore();
            std::cin.get(word, sizeof(word), '\n');
            if (dictionary.erase(&word[0])) {
                std::cout << "Removed " << word << std::endl;
            } else {
                std::cout << "Failed to Remove " << word << std::endl;
            }

            break;
        }
        case '3': {
            char word[80];
            std::cout << "Enter the word : ";
            std::cin.ignore();
            std::cin.get(word, sizeof(word), '\n');

            Iterator iter = dictionary.find(word);
            if (iter != dictionary.end()) {
                std::cout << word << " : " << iter->second->c_str() << std::endl;
            } else {
                std::cout << "Could not find " << word << std::endl;
            }
            break;
        }

        case '4': {
            char word[80];
            std::cout << "Enter the word : ";
            std::cin.ignore();
            std::cin.get(word, sizeof(word), '\n');
            std::cout << "Searching ... ";

            Iterator iter = dictionary.startsWith(&word[0]);

            if (iter == dictionary.end()) {
                std::cout << "Could not find the given word";
            } else {
                std::cout << std::endl << "*****************************************";
                for (; iter != dictionary.end(); ++iter) {
                    std::cout << std::endl
                              << iter->first
                              << " : " << iter->second->c_str();
                }
                std::cout << std::endl << "*****************************************";
            }
            break;
        }
        case '5': {
            std::cout << "*****************************************" << std::endl;
            Iterator iter = dictionary.begin();
            Iterator iend = dictionary.end();

            for (; iter != iend; ++iter) {
                std::string k((const char *)(iter->first));

                std::cout.width(10);
                k.insert(0, "[");
                k.insert(k.length(), "]");
                std::cout << std::left << k.c_str() << " : ";
                std::cout.width(0);
                std::cout.width(70);
                std::cout << std::left << iter->second->c_str() << std::endl;
            }

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
