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

int main(int argc, char ** argv) {

    TrieTestCases::instance()->execute();

    rtv::Trie<char, std::string, TrieCaseInsensitiveCompare> dictionary('$', 256);
    
    TrieTestCases::instance()->populateTrieWithSampleValues(dictionary);
    
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
                dictionary.traverse(TrieTraverseCallBack());
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
