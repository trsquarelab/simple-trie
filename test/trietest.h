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

#ifndef TRIETEST_H_
#define TRIETEST_H_

#include <iostream>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <ctime>

class TrieCompare {
public:
    bool operator()(char v1, char v2) {
        return v1 < v2;
    }  
};

class TrieCaseInsensitiveCompare {
public:
    bool operator()(char v1, char v2) {
        int i1 = std::tolower(v1);
        int i2 = std::tolower(v2);
        return i1 < i2;
    }
};

class TrieTraverseCallBack {
public:
    mutable unsigned int mCount;
    bool mPrint;
    char mEndSymbol;

    TrieTraverseCallBack(char endSymbol='$')
        : mCount(0),
          mPrint(true),
          mEndSymbol(endSymbol)
    {}

    void operator()(const char * key, std::string const & value) const {
        if (mPrint) {
            std::string k;
            for (int i=0; key[i] != mEndSymbol; ++i) {
                char astr[2] = {'\0', '\0'};
                astr[0] = key[i];
                k.append(astr);
            }

            std::cout.width(10);
            k.insert(0, "[");
            k.insert(k.length(), "]");
            std::cout << std::left << k.c_str() << " : ";
            std::cout.width(0);
            std::cout.width(70);
            std::cout << std::left << value.c_str() << std::endl;
        }
        ++mCount;
    }
};

class TrieTestCases
{
private:
    typedef void (*TestCaseFunction)();

public:
    static TrieTestCases * instance() {
        static TrieTestCases _instance;
        return &_instance;
    }

    bool addTestCase(TestCaseFunction testCase) {
        mTestCases.push_back(testCase);
        return true;
    }

    void execute() {
        for (std::vector<TestCaseFunction>::iterator iter = mTestCases.begin() ;
             iter != mTestCases.end(); ++iter) {
            (*iter)();
        }
    }

    template <typename D>
    void populateTrieWithSampleValues(D  & aTrie) {
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            TrieTestCases::testAndAddToTrie(aTrie, iter->first, iter->second);
        }
    }

    template <typename D>
    void testKeyInTrie(D  & aTrie, std::string const & word, bool expected) {
        if (aTrie.hasKey(word.c_str())) {
            assert(expected, "Found", word.c_str(), "when it should not be found !!!");            
        } else {
            assert(!expected, "Did not find", word.c_str(), "when it should be !!!");
        }
    }

    template <typename D>
    void testAndAddToTrie(D & aTrie, const std::string & word, const std::string & meaning) {
        unsigned int trieSize = aTrie.size();
        aTrie.insert(word.c_str(), meaning.c_str());
        assert(aTrie.get(word.c_str())->compare(meaning.c_str()) == 0, "Error in Trie::insert or Trie::get !!!");
        assert(aTrie.size() == trieSize+1, "Trie size did not updated properly after Trie::insert!!!");
    }
   
    template<typename D>
    void teststartsWith(D & aTrie, const std::string & key) {
        unsigned int countForKey = 0;
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            int cv = key.compare(0, key.length()-1, iter->first, 0, key.length()-1);
            if (cv == 0) {
                ++countForKey;
            }
        }
        std::vector< std::pair < std::vector<char> , std::string > > result;
        aTrie.startsWith(key.c_str(), result, mSampleValues.size());
        TrieTestCases::assert(result.size() == countForKey, "Error in Trie::startsWith functionality!!!");
    }

    template <typename D>
    void testSuite(D & aTrie) {

        std::srand((unsigned int)std::time(0));

        //Test Trie::empty functionality
        aTrie.clear();
        assert(aTrie.empty(), "Trie::empty failed!!!");
        
        //Test Trie::insert functionality
        TrieTestCases::populateTrieWithSampleValues(aTrie);
        
        //Test Trie::size functionality
        assert(aTrie.size() == mSampleValues.size(), "Trie::size failed!!!",
                                                  "Trie::size returned", aTrie.size(),
                                                  "Expected", mSampleValues.size());
        //Test Trie::traverse functionality
        do {
            TrieTraverseCallBack ttcb;
            ttcb.mPrint = false;
            aTrie.traverse(ttcb);
            TrieTestCases::assert(ttcb.mCount == mSampleValues.size(),
                                  "Error in Trie::traverse functionality!!! Expected count",
                                  mSampleValues.size(),
                                  "Actual count", ttcb.mCount);
        } while (0);

        //Test Trie::hasKey functionality
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            TrieTestCases::testKeyInTrie(aTrie, iter->first, true);
        }
        TrieTestCases::testKeyInTrie(aTrie, "something which is not in the Trie$", false);

        //Test Trie::startsWith functionality
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string keyStr = iter->first;
            if (keyStr.length() > 0) {
                std::string keyPart = keyStr.substr(0, std::rand() % keyStr.length());
                keyPart.insert(keyPart.length(), "$");
                TrieTestCases::teststartsWith(aTrie, keyPart);
            }
        }
        std::string negTest("something which is not in the Trie$");
        TrieTestCases::teststartsWith(aTrie, negTest);

        //Test Trie::erase functionality
        unsigned int trieSize = aTrie.size();
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            assert(aTrie.erase(iter->first.c_str()), "Removing ", iter->first.c_str(), "failed!!!");
            assert(aTrie.get(iter->first.c_str()) == 0, "Removing ", iter->first.c_str(), "failed!!!");
            assert(--trieSize == aTrie.size(), "Trie size is not updated after remove operation!!!");

            TrieTraverseCallBack ttcb;
            ttcb.mPrint = false;
            aTrie.traverse(ttcb);
            assert(aTrie.size() == ttcb.mCount, "Trie size is not updated after remove operation!!!");
        }

        //Test Trie::clear functionality
        TrieTestCases::populateTrieWithSampleValues(aTrie);
        aTrie.clear();
        assert(aTrie.empty(), "Trie::clear failed!!!");
        do {
            TrieTraverseCallBack ttcb;
            ttcb.mPrint = false;
            aTrie.traverse(ttcb);
            TrieTestCases::assert(ttcb.mCount == 0,
                                  "Error in Trie::clear functionality!!! Expected count",
                                  0,
                                  "Actual count", ttcb.mCount);
        } while (0);
    }

    template <typename T>
    void assert(bool result, const T & message) {
        if (!result) {
            std::cerr << message << std::endl;
            std::abort();
        }
    }

    template <typename T1, typename T2>
    void assert(bool result,
                       const T1 & message1,
                       const T2 & message2) {
        if (!result) {
            std::cerr << message1 << " " <<
                         message2 << std::endl;
            std::abort();
        }
    }

    template <typename T1, typename T2, typename T3>
    void assert(bool result,
                       const T1 & message1,
                       const T2 & message2,
                       const T3 & message3) {
        if (!result) {
            std::cerr << message1 << " " <<
                         message2 << " " <<
                         message3 << std::endl;
            std::abort();
        }
    }

    template <typename T1, typename T2, typename T3, typename T4>
    void assert(bool result,
                       const T1 & message1,
                       const T2 & message2,
                       const T3 & message3,
                       const T4 & message4) {
        if (!result) {
            std::cerr << message1 << " " <<
                         message2 << " " <<
                         message3 << " " <<
                         message4 << std::endl;
            std::abort();
        }
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    void assert(bool result,
                       const T1 & message1,
                       const T2 & message2,
                       const T3 & message3,
                       const T4 & message4,
                       const T5 & message5) {
        if (!result) {
            std::cerr << message1 << " " << message2 << " " <<
                         message3 << " " << message4 << " " <<
                         message5 << std::endl;
            std::abort();
        }
    }

private:
    TrieTestCases()
    {
        const std::pair<std::string, std::string> sampleValues[] = {
            std::pair<std::string, std::string>(std::string("Multiset$"),
                std::string("Multisets are associative containers with the same properties as set containers, but allowing for multiple keys with equal values")),
            std::pair<std::string, std::string>(std::string("Deque$"),
                std::string("Double-ended queue")),
            std::pair<std::string, std::string>(std::string("Multimap$"),
                std::string("Multimaps are a kind of associative container that stores elements formed by the combination of a key value and a mapped value, much like map containers, but allowing different elements to have the same key value")),
            std::pair<std::string, std::string>(std::string("Stack$"),
                std::string("Stacks are a type of container adaptor, specifically designed to operate in a LIFO context")),
            std::pair<std::string, std::string>(std::string("Set$"),
                std::string("Sets are a kind of associative container that stores unique elements, and in which the elements themselves are the keys")),
            std::pair<std::string, std::string>(std::string("Vector$"),
                std::string("Vectors are a kind of sequence container. As such, their elements are ordered following a strict linear sequence")),
            std::pair<std::string, std::string>(std::string("Array$"),
                std::string("Arrays are fixed-size sequence containers: they hold a specific number of elements ordered in a strict linear sequence")),
            std::pair<std::string, std::string>(std::string("Map$"),
                std::string("Maps are a kind of associative container that stores elements formed by the combination of a key value and a mapped value")),
            std::pair<std::string, std::string>(std::string("List$"),
                std::string("Lists are a kind of sequence container. As such, their elements are ordered following a linear sequence")),
            std::pair<std::string, std::string>(std::string("Bitset$"),
                std::string("A bitset is a special container class that is designed to store bits")),
        };

        for (unsigned int i=0; i<sizeof(sampleValues)/sizeof(sampleValues[0]); ++i) {
            mSampleValues.insert(mSampleValues.begin(), sampleValues[i]);
        }
    }

    TrieTestCases(TrieTestCases const &);
    TrieTestCases & operator=(TrieTestCases const &);

private:
    typedef std::vector< std::pair<std::string, std::string> > SampleValues;
    typedef SampleValues::iterator SampleValuesIter;
    typedef SampleValues::const_iterator SampleValuesConstIter;

    std::vector<TestCaseFunction> mTestCases;
    SampleValues mSampleValues;
};

#endif
