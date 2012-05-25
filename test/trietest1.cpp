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

    rtv::Trie<char, std::string, TrieCompare> dictionary('$', 256);
    
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

    std::string multi("Multi$");
    TrieTestCases::teststartsWith(dictionary, multi);

    std::string negTest("something which is not in the dictionary$");
    TrieTestCases::teststartsWith(dictionary, negTest);

    std::cout << "Succeeded" << std::endl;
}

bool testCaseAdded = TrieTestCases::instance()->addTestCase(test1);

}
