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

#include "trietest.h"
#include "trie.h"

int main(int argc, char **argv)
{

    TrieTestCases::instance()->execute();

    rtv::Trie<char, std::string, TrieCaseInsensitiveCompare> dictionary('\0');

    TrieTestCases::instance()->populateTrieWithSampleValues(dictionary);

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
            std::pair< rtv::Trie<char, std::string, TrieCaseInsensitiveCompare>::Iterator, bool> result = dictionary.insert(&word[0], meaning);
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
            std::string * meaning = dictionary.get(word);
            if (meaning) {
                std::cout << word << " : " << meaning->c_str() << std::endl;
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

            rtv::Trie<char, std::string, TrieCaseInsensitiveCompare>::Iterator iter = dictionary.startsWith(&word[0]);

            if (iter == dictionary.end()) {
                std::cout << "Could not find the given word";
            } else {
                std::cout << std::endl << "*****************************************";
                for (; iter != dictionary.end(); ++iter) {
                    std::cout << std::endl
                              << TrieTestCases::instance()->keyToString(dictionary.endSymbol(), iter->first).c_str()
                              << " : " << iter->second->c_str();
                }
                std::cout << std::endl << "*****************************************";
            }
            break;
        }
        case '5': {
            std::cout << "*****************************************" << std::endl;
            rtv::Trie<char, std::string, TrieCaseInsensitiveCompare>::Iterator iter = dictionary.begin();
            rtv::Trie<char, std::string, TrieCaseInsensitiveCompare>::Iterator iend = dictionary.end();

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
