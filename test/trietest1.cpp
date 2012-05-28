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

namespace {

void test1() {
    std::cout << "Executing Test Case 1 ... ";

    rtv::Trie<char, std::string, TrieCompare, rtv::SetItems<char, std::string, TrieCompare > > dictionary('$');
        
    TrieTestCases::instance()->testSuite(dictionary);

    std::cout << "Succeeded" << std::endl;
}

bool testCaseAdded = TrieTestCases::instance()->addTestCase(test1);

}
