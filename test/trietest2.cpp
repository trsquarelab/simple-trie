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
#include "testsuite.h"

#include <exception>

using namespace rtv;

namespace
{

// key to vector index converter
// includes alphabets, space and end symbol
class CaseSensitiveAlphaToIndex
{
public:
    unsigned int operator()(const char & c) const {
        unsigned int index = 53;
        if (c == ' ') {
            index = 52;
        } else if (c >= 'A' && c <= 'Z') {
            index = c - 'A';
        } else if (c >= 'a' && c <= 'z') {
            index = 26 + c - 'a';
        }
        return index;
    }
};

// key to vector index converter
// case insensitive and includes alphabets, space and end symbol
class AlphaToIndex
{
public:
    unsigned int operator()(const char & c) const {
        unsigned int index = 27;
        if (c == ' ') {
            index = 26;
        } else if (c >= 'A' && c <= 'Z') {
            index = c - 'A';
        } else if (c >= 'a' && c <= 'z') {
            index = c - 'a';
        }
        return index;
    }
};


typedef rtv::VectorItems<char, std::string, std::less<char>, 256> VectorItemClass;

typedef TrieTestCases< rtv::Trie<char, std::string, std::less<char>, VectorItemClass> > TrieVectorTestCases;
TEST_F(TrieVectorTestCases, TrieVectorImplCase_Hash_EndSymbol)
{
    TheTrie dictionary('#');
    testSuite(dictionary);
}

TEST_F(TrieVectorTestCases, TrieVectorImplCase_Dollar_EndSymbol)
{
    TheTrie dictionary('$');
    testSuite(dictionary);
}

TEST_F(TrieVectorTestCases, TrieVectorImplCase_Null_EndSymbol)
{
    TheTrie dictionary('\0');
    testSuite(dictionary);
}

typedef TrieTestCases< rtv::Trie<char, std::string, TrieCaseInsensitiveCompare, rtv::VectorItems<char, std::string,
                             TrieCaseInsensitiveCompare, 28, AlphaToIndex> > > TrieCaseInSensitiveCustomVectorTestCases;
TEST_F(TrieCaseInSensitiveCustomVectorTestCases, TrieVectorImplCase_CustomVectorItemCaseInsensitive)
{
    TheTrie dictionary('\0');
    testSuite(dictionary);
}

typedef TrieTestCases< rtv::Trie<char, std::string, std::less<char>, rtv::VectorItems<char, std::string, std::less<char>,
                                              54, CaseSensitiveAlphaToIndex> > > TrieCaseSensitiveCustomVectorTestCases;
TEST_F(TrieCaseSensitiveCustomVectorTestCases, TrieVectorImplCase_CustomVectorItemCaseSensitive)
{
    TheTrie dictionary('\0');
    testSuite(dictionary);
}

}

