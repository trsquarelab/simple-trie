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

#include "trie.h"
#include "trietest.h"

#include <exception>

namespace
{

void test2();
bool testCaseAdded = TrieTestCases::instance()->addTestCase(test2);

// key to vector index converter
// case insensitive and includes alphabets, space and end symbol
class AlphaToIndex
{
public:
    unsigned int operator()(const char & c) const {
        unsigned int index = 0;
        if (c == '\0') {
            index = 26;
        } else if (c == ' ') {
            index = 27;
        } else if (c >= 'A' && c <= 'Z') {
            index = c - 'A';
        } else if (c >= 'a' && c <= 'z') {
            index = c - 'a';
        } else {
            throw std::exception();
        }
        return index;
    }
};

void test2()
{
    (void)testCaseAdded;

    typedef rtv::VectorItems<char, std::string, std::less<char>, 256> VectorItemClass;

    std::cout << "Executing Test Case 2 ... ";

    rtv::Trie<char, std::string, std::less<char>, VectorItemClass> dictionary1('$');
    TrieTestCases::instance()->testSuite(dictionary1);

    rtv::Trie<char, std::string, std::less<char>, VectorItemClass> dictionary2('#');
    TrieTestCases::instance()->testSuite(dictionary2);

    rtv::Trie<char, std::string, std::less<char>, VectorItemClass> dictionary3('\0');
    TrieTestCases::instance()->testSuite(dictionary3);

    rtv::Trie<char, std::string, TrieCaseInsensitiveCompare, rtv::VectorItems<char, std::string, TrieCaseInsensitiveCompare, 28, AlphaToIndex> > dictionary4('\0');
    TrieTestCases::instance()->testSuite(dictionary4);

    std::cout << "Succeeded" << std::endl;
}

}
