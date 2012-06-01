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

namespace
{

void test1();

bool testCaseAdded = TrieTestCases::instance()->addTestCase(test1);

void test1()
{
    (void)testCaseAdded;
    std::cout << "Executing Test Case 1 ... ";

    rtv::Trie<char, std::string, TrieCompare, rtv::SetItems<char, std::string, TrieCompare > > dictionary1('$');
    TrieTestCases::instance()->testSuite(dictionary1);

    rtv::Trie<char, std::string, TrieCompare, rtv::SetItems<char, std::string, TrieCompare > > dictionary2('#');
    TrieTestCases::instance()->testSuite(dictionary2);

    rtv::Trie<char, std::string, TrieCompare, rtv::SetItems<char, std::string, TrieCompare > > dictionary3('\0');
    TrieTestCases::instance()->testSuite(dictionary3);

    std::cout << "Succeeded" << std::endl;
}

}
