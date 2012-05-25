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
#include "trietest.h"

namespace {

void test2() {
    std::cout << "Executing Test Case 2 ... ";

    rtv::Trie<char, std::string, std::less<char> > dictionary('$', 256);
    
    TrieTestCases::populateTrieWithSampleValues(dictionary);
    
    do {
        TrieTraverseCallBack ttcb;
        ttcb.mPrint = false;
        dictionary.traverse(ttcb);
        TrieTestCases::assert(ttcb.mCount == sampleValuesCount,
                              "Error in Trie::traverse functionality!!! Expected count",
                              sampleValuesCount,
                              "Actual count", ttcb.mCount);
    } while (0);

    TrieTestCases::testKeyInTrie(dictionary, "Map$", true);
    TrieTestCases::testKeyInTrie(dictionary, "List$", true);
    TrieTestCases::testKeyInTrie(dictionary, "Bitset$", true);
    TrieTestCases::testKeyInTrie(dictionary, "Squence$", false);

    std::vector< std::pair < std::vector<char> , std::string > > result;
    dictionary.startsWith("Multi$", result);
    TrieTestCases::assert(result.size() == 2, "Error in Trie::startsWith functionality!!!");
    std::cout << "Succeeded" << std::endl;
}

bool testCaseAdded = TrieTestCases::instance()->addTestCase(test2);
}
