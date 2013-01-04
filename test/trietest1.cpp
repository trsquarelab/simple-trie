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

using namespace rtv;

namespace
{

typedef rtv::SetItems<char, std::string, TrieCompare > SetItemClass;

typedef TrieTestCases< rtv::Trie<char, std::string, TrieCompare, SetItemClass> > TrieSetTestCases;

TEST_F(TrieSetTestCases, TrieTestCaseSetImplCase_Dollar_EndSymbol) {
    TheTrie dictionary('$');
    testSuite(dictionary);
}

TEST_F(TrieSetTestCases, TrieTestCaseSetImplCase_Hash_EndSymbol) {
    TheTrie dictionary('#');
    testSuite(dictionary);
}

TEST_F(TrieSetTestCases, TrieTestCaseSetImplCase_Null_EndSymbol) {
    TheTrie dictionary('\0');
    testSuite(dictionary);
}

}

