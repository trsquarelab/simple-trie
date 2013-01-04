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

#ifndef TRIETEST_H
#define TRIETEST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>

#include "testsuite.h"

class TrieCompare
{
public:
    bool operator()(char v1, char v2) {
        return v1 < v2;
    }
};

class TrieCaseInsensitiveCompare
{
public:
    bool operator()(char v1, char v2) {
        int i1 = std::tolower(v1);
        int i2 = std::tolower(v2);
        return i1 < i2;
    }
};

template <typename D>
class TrieTestCases: public rtv::RTest
{
protected:
    typedef void (*TestCaseFunction)();

    typedef std::vector< std::pair<std::string, std::string> > SampleValues;
    typedef SampleValues::iterator SampleValuesIter;
    typedef SampleValues::const_iterator SampleValuesConstIter;

    typedef std::vector<std::string> NegativeSampleValues;
    typedef NegativeSampleValues::iterator NegativeSampleValuesIter;
    typedef NegativeSampleValues::const_iterator NegativeSampleValuesConstIter;

    typedef D           TheTrie;
    typedef typename D::Iterator TrieIterator;
    typedef typename D::ConstIterator TrieConstIterator;

public:
    void populateTrieWithSampleValues(D &aTrie, SampleValues & sv) {
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            char endSymbol = aTrie.endSymbol();
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            TrieTestCases::testAndAddToTrie(aTrie, key, iter->second);
        }
    }

    void testKeyInTrie(D   &aTrie, std::string const &word, bool expected) {
        if (aTrie.hasKey(word.c_str())) {
            EXPECT_TRUE(expected);
        } else {
            EXPECT_FALSE(expected);
        }
    }

    void testAndAddToTrie(D &aTrie, const std::string &word, const std::string &meaning) {
        unsigned int trieSize = aTrie.size();
        std::pair<typename D::Iterator, bool> res = aTrie.insert(word.c_str(), meaning.c_str());
        if (res.second) {
            std::string keyStr = keyToString(aTrie.endSymbol(), res.first->first);
            EXPECT_TRUE(word.compare(keyStr) == 0);
            EXPECT_TRUE(res.first->second->compare(meaning) == 0);
        }
        EXPECT_TRUE(aTrie.get(word.c_str())->compare(meaning.c_str()) == 0);
        EXPECT_TRUE(aTrie.size() == trieSize + 1);
    }

    void teststartsWith(D & aTrie, const std::string &key, SampleValues & sv) {
        if (key.length() > 0) {
            unsigned int countForKey = 0;
            for (SampleValuesIter iter = sv.begin();
                    iter != sv.end(); ++iter) {
                int cv = key.compare(0, key.length() - 1, iter->first, 0, key.length() - 1);
                if (cv == 0) {
                    ++countForKey;
                }
            }
            typename D::Iterator iter = aTrie.startsWith(key.c_str());
            unsigned int countStartsWith = 0;
            for (; iter != aTrie.end(); ++iter) {
                std::string keyStr = keyToString(aTrie.endSymbol(), iter->first);
                keyStr.erase(keyStr.length() - 1);
                bool res = isPresent(keyStr, sv);
                EXPECT_TRUE(res);
                ++countStartsWith;
            }

            EXPECT_TRUE(countStartsWith == countForKey);
        }
    }

    void teststartsWith(const D & aTrie, const std::string &key, SampleValues & sv) const {
        if (key.length() > 0) {
            unsigned int countForKey = 0;
            for (SampleValuesConstIter iter = sv.begin();
                    iter != sv.end(); ++iter) {
                int cv = key.compare(0, key.length() - 1, iter->first, 0, key.length() - 1);
                if (cv == 0) {
                    ++countForKey;
                }
            }
            TrieConstIterator iter = aTrie.startsWith(key.c_str());
            unsigned int countStartsWith = 0;
            for (; iter != aTrie.end(); ++iter) {
                std::string keyStr = keyToString(aTrie.endSymbol(), iter->first);
                keyStr.erase(keyStr.length() - 1);
                bool res = isPresent(keyStr, sv);
                EXPECT_TRUE(res);
                ++countStartsWith;
            }

            EXPECT_TRUE(countStartsWith == countForKey);
        }
    }

    std::string keyToString(char endSymbol, const char *key) const {
        int i = 0;
        for (; key[i] != endSymbol; ++i) {
        }
        return std::string(key, i + 1);
    }

    void testSuite(D &aTrie) {
        for (unsigned int i=0; i<mSampleValuesContainer.size(); ++i) {
            doTestSuite(aTrie, mSampleValuesContainer[i]);
        }
    }

    bool isPresent(std::string const & key, SampleValues & sv) const {
        for (SampleValuesConstIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            if (iter->first.compare(key) == 0) {
                return true;
            }
        }
        return false;
    }

protected:
    ~TrieTestCases() {
    }

    class PairSort
    {
    public:
        bool operator()(std::pair<std::string, std::string> const &v1,
                        std::pair<std::string, std::string> const &v2) {
            return v1.first.compare(v2.first) < 0;
        }
    };

    TrieTestCases() {
        const std::pair<std::string, std::string> sampleValues[] = {
            std::make_pair(std::string("Array"), std::string("Arrays are fixed-size sequence containers: they hold a specific number of elements ordered in a strict linear sequence")),
            std::make_pair(std::string("Bitset"), std::string("A bitset is a special container class that is designed to store bits")),
            std::make_pair(std::string("Deque"), std::string("Double-ended queue")),
            std::make_pair(std::string("List"), std::string("Lists are a kind of sequence container. As such, their elements are ordered following a linear sequence")),
            std::make_pair(std::string("Map"), std::string("Maps are a kind of associative container that stores elements formed by the combination of a key value and a mapped value")),
            std::make_pair(std::string("Maps"), std::string("A group of Map")),
            std::make_pair(std::string("Multimap"), std::string("Multimaps are a kind of associative container that stores elements formed by the combination of a key value and a mapped value, much like map containers, but allowing different elements to have the same key value")),
            std::make_pair(std::string("Multiset"), std::string("Multisets are associative containers with the same properties as set containers, but allowing for multiple keys with equal values")),
            std::make_pair(std::string("Set"), std::string("Sets are a kind of associative container that stores unique elements, and in which the elements themselves are the keys")),
            std::make_pair(std::string("Stack"), std::string("Stacks are a type of container adaptor, specifically designed to operate in a LIFO context")),
            std::make_pair(std::string("Vector"), std::string("Vectors are a kind of sequence container. As such, their elements are ordered following a strict linear sequence"))
        };

        const std::string negativeSampleValues[] = {
            std::string("Multiset negative"),
            std::string("Deque negative"),
            std::string("Multimap negative"),
            std::string("Stack negative"),
            std::string("Set negative"),
            std::string("Vector negative"),
            std::string("Array negative"),
            std::string("Map negative"),
            std::string("List negative"),
            std::string("Bitset negative"),
            std::string("Bit"),
            std::string("Multi"),
            std::string("Se"),
            std::string("vect"),
            std::string("zsfdgferg"),
            std::string("!@%^&*()"),
            std::string("~23435678656565764"),
            std::string("11111111111"),
            std::string("aaaaaaaaaaa"),
            std::string("AAAAAAAAAAA")
        };

        SampleValues sv1;
        for (unsigned int i = 0; i < sizeof(sampleValues) / sizeof(sampleValues[0]); ++i) {
            sv1.insert(sv1.begin(), sampleValues[i]);
        }
        mSampleValuesContainer.push_back(sv1);

        SampleValues sv2;
        sv2.insert(sv2.begin(), sampleValues[0]);
        mSampleValuesContainer.push_back(sv2);

        SampleValues sv3;
        sv3.insert(sv3.begin(), sampleValues[4]);
        sv3.insert(sv3.begin(), sampleValues[5]);
        mSampleValuesContainer.push_back(sv3);

        for (unsigned int i = 0; i < sizeof(negativeSampleValues) / sizeof(negativeSampleValues[0]); ++i) {
            mNegativeSampleValues.push_back(negativeSampleValues[i]);
        }
    }

    TrieTestCases(TrieTestCases const &);
    TrieTestCases &operator=(TrieTestCases const &);

protected:
    std::vector<TestCaseFunction> mTestCases;
    std::vector<SampleValues> mSampleValuesContainer;
    NegativeSampleValues mNegativeSampleValues;

public:
    void doTestSuite(D &aTrie, SampleValues & sv) {

        char endSymbol = aTrie.endSymbol();

        std::srand((unsigned int)std::time(0));

        //Test Trie::empty functionality
        aTrie.clear();
        EXPECT_TRUE(aTrie.empty());

        //Test Trie::insert functionality
        TrieTestCases::populateTrieWithSampleValues(aTrie, sv);

        //Test Trie::find functionality
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            typename TheTrie::Iterator titer = aTrie.find(key.c_str());
            EXPECT_TRUE(titer != aTrie.end());

            EXPECT_TRUE(key.compare(keyToString(aTrie.endSymbol(), titer->first)) == 0);
            EXPECT_TRUE(iter->second.compare(*(titer->second)) == 0);

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first, sv)) {
                EXPECT_TRUE(aTrie.find(key.c_str()) == aTrie.end());
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            EXPECT_TRUE(aTrie.find(negKey.c_str()) == aTrie.end());
        }

        //Test Trie::find() const functionality
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            TrieConstIterator titer = ((const TheTrie &)(aTrie)).find(key.c_str());
            EXPECT_TRUE(key.compare(keyToString(aTrie.endSymbol(), titer->first)) == 0);
            EXPECT_TRUE(iter->second.compare(*(titer->second)) == 0);

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first, sv)) {
                EXPECT_TRUE(((const TheTrie &)(aTrie)).find(key.c_str()) == ((const TheTrie &)(aTrie)).end());
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            EXPECT_TRUE(((const TheTrie &)(aTrie)).find(negKey.c_str()) == ((const TheTrie &)(aTrie)).end());
        }

        //Test Trie::get functionality
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            EXPECT_TRUE((aTrie.get(key.c_str()))->compare(iter->second) == 0);

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first, sv)) {
                EXPECT_TRUE(aTrie.get(key.c_str()) == 0);
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            EXPECT_TRUE(aTrie.get(negKey.c_str()) == 0);
        }

        //Test Trie::get() const functionality
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            EXPECT_TRUE((((TheTrie const &)aTrie).get(key.c_str()))->compare(iter->second) == 0);

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first, sv)) {
                EXPECT_TRUE(((TheTrie const &)aTrie).get(key.c_str()) == 0);
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            EXPECT_TRUE(((TheTrie const &)aTrie).get(negKey.c_str()) == 0);
        }

        //Test Trie::size functionality
        EXPECT_TRUE(aTrie.size() == sv.size());

        //Test Trie::ConstIterator functionality
        SampleValues copySamples = sv;
        std::sort(copySamples.begin(), copySamples.end(), PairSort());

        SampleValuesIter siter = copySamples.begin();

        // operator++()
        for (typename TheTrie::ConstIterator iter = ((const TheTrie &)aTrie).begin();
                iter != ((const TheTrie &)aTrie).end(); ++iter, ++siter) {
            std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
            iterStr.erase(iterStr.length() - 1);
            EXPECT_TRUE(siter->first.compare(iterStr) == 0);

            EXPECT_TRUE(siter->second.compare(iter->second->c_str()) == 0);
        }

        // operator--()
        do {
            SampleValuesIter siter = copySamples.end();
            typename TheTrie::ConstIterator iter = ((const TheTrie &)aTrie).end();

            for (--siter, --iter; iter != ((const TheTrie &)aTrie).end(); --iter) {
                std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
                iterStr.erase(iterStr.length() - 1);
                EXPECT_TRUE(siter->first.compare(iterStr) == 0);

                EXPECT_TRUE(siter->second.compare(iter->second->c_str()) == 0);
                if (siter != copySamples.begin()) {
                    --siter;
                }
            }
        } while (0);

        // operator++(int)
        siter = copySamples.begin();
        for (typename TheTrie::ConstIterator iter = ((const TheTrie &)aTrie).begin();
                iter != ((const TheTrie &)aTrie).end(); iter++, siter++) {
            std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
            iterStr.erase(iterStr.length() - 1);
            EXPECT_TRUE(siter->first.compare(iterStr) == 0);

            EXPECT_TRUE(siter->second.compare(iter->second->c_str()) == 0);
        }

        // operator--(int)
        do {
            SampleValuesIter siter = copySamples.end();
            typename TheTrie::ConstIterator iter = ((const TheTrie &)aTrie).end();

            for (siter--, iter--; iter != ((const TheTrie &)aTrie).end(); iter--) {
                std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
                iterStr.erase(iterStr.length() - 1);
                EXPECT_TRUE(siter->first.compare(iterStr) == 0);

                EXPECT_TRUE(siter->second.compare(iter->second->c_str()) == 0);
                if (siter != copySamples.begin()) {
                    siter--;
                }
            }
        } while (0);

        //Test Trie::Iterator functionality
        std::sort(copySamples.begin(), copySamples.end(), PairSort());

        siter = copySamples.begin();

        // operator++()
        for (typename TheTrie::Iterator iter = aTrie.begin();
                iter != aTrie.end(); ++iter, ++siter) {
            std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
            iterStr.erase(iterStr.length() - 1);
            EXPECT_TRUE(siter->first.compare(iterStr) == 0);

            EXPECT_TRUE(siter->second.compare(iter->second->c_str()) == 0);
        }

        // operator--()
        do {
            SampleValuesIter siter = copySamples.end();
            typename TheTrie::Iterator iter = aTrie.end();

            for (--siter, --iter; iter != aTrie.end(); --iter) {
                std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
                iterStr.erase(iterStr.length() - 1);
                EXPECT_TRUE(siter->first.compare(iterStr) == 0);

                EXPECT_TRUE(siter->second.compare(iter->second->c_str()) == 0);
                if (siter != copySamples.begin()) {
                    --siter;
                }
            }
        } while (0);

        // operator++(int)
        siter = copySamples.begin();
        for (typename TheTrie::Iterator iter = aTrie.begin();
                iter != aTrie.end(); iter++, siter++) {
            std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
            iterStr.erase(iterStr.length() - 1);
            EXPECT_TRUE(siter->first.compare(iterStr) == 0);

            EXPECT_TRUE(siter->second.compare(iter->second->c_str()) == 0);
        }

        // operator--(int)
        do {
            SampleValuesIter siter = copySamples.end();
            typename TheTrie::Iterator iter = aTrie.end();

            for (siter--, iter--; iter != aTrie.end(); iter--) {
                std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
                iterStr.erase(iterStr.length() - 1);
                EXPECT_TRUE(siter->first.compare(iterStr) == 0);

                EXPECT_TRUE(siter->second.compare(iter->second->c_str()) == 0);
                if (siter != copySamples.begin()) {
                    siter--;
                }
            }
        } while (0);

        //Iterator other functionalities
        do {
            int elementsCount = aTrie.size();
            int rightCount = 0;
            int leftCount = 0;
            typename TheTrie::Iterator iter = aTrie.begin();
            for (; iter != aTrie.end(); ++iter) {
                    ++rightCount;
            }

            EXPECT_TRUE(rightCount == elementsCount);

            for (int i=0; i<5; ++i) {
                leftCount = 0;
                for (--iter; iter != aTrie.end(); --iter) {
                        ++leftCount;
                }
                EXPECT_TRUE(leftCount == rightCount);
            }

        } while (0);

        for (typename TheTrie::ConstIterator iter = ((const TheTrie &)aTrie).begin();
                iter != ((const TheTrie &)aTrie).end(); ++iter) {
            bool res = (iter == ((const TheTrie &)aTrie).find(iter->first));
            EXPECT_TRUE(res);
            res = (iter != ++(((const TheTrie &)aTrie).find(iter->first)));
            EXPECT_TRUE(res);
            res = (iter == ((const TheTrie &)aTrie).startsWith(iter->first));
            EXPECT_TRUE(res);
        }

        for (typename TheTrie::Iterator iter = aTrie.begin();
                iter != aTrie.end(); ++iter) {
            bool res = (iter == aTrie.find(iter->first));
            EXPECT_TRUE(res);
            res = (iter != ++(aTrie.find(iter->first)));
            EXPECT_TRUE(res);
            res = (iter == aTrie.startsWith(iter->first));
            EXPECT_TRUE(res);
        }

        for (typename TheTrie::Iterator iter = aTrie.begin();
                iter != aTrie.end(); ++iter) {

            if (iter != aTrie.begin()) {
                typename TheTrie::Iterator previter = iter;
                previter = --previter;
                EXPECT_TRUE(iter != previter);
                EXPECT_TRUE((iter == ++previter));
            }
            if (iter != aTrie.end()) {
                typename TheTrie::Iterator nextiter = iter;
                nextiter = ++nextiter;
                EXPECT_TRUE(iter != nextiter);
                EXPECT_TRUE((iter == --nextiter));
            }
        }

        do {
            typename TheTrie::Iterator iter = aTrie.end();
            for (--iter; iter != aTrie.end(); --iter) {
                if (iter != aTrie.begin()) {
                    typename TheTrie::Iterator previter = iter;
                    previter = --previter;
                    EXPECT_TRUE(iter != previter);
                    EXPECT_TRUE((iter == ++previter));
                }
                if (iter != aTrie.end()) {
                    typename TheTrie::Iterator nextiter = iter;
                    nextiter = ++nextiter;
                    EXPECT_TRUE(iter != nextiter);
                    EXPECT_TRUE((iter == --nextiter));
                }
            }
        } while (0);

        for (typename TheTrie::ConstIterator iter = ((const TheTrie &)aTrie).begin();
                iter != ((const TheTrie &)aTrie).end(); ++iter) {
            if (iter != ((const TheTrie &)aTrie).begin()) {
                typename TheTrie::ConstIterator previter = iter;
                previter = --previter;
                EXPECT_TRUE(iter != previter);
                EXPECT_TRUE((iter == ++previter));
            }
            if (iter != ((const TheTrie &)aTrie).end()) {
                typename TheTrie::ConstIterator nextiter = iter;
                nextiter = ++nextiter;
                EXPECT_TRUE(iter != nextiter);
                EXPECT_TRUE((iter == --nextiter));
            }
        }

        do {
            typename TheTrie::ConstIterator iter = ((const TheTrie &)aTrie).end();
            for (--iter; iter != ((const TheTrie &)aTrie).end(); --iter) {
                if (iter != ((const TheTrie &)aTrie).begin()) {
                    typename TheTrie::ConstIterator previter = iter;
                    previter = --previter;
                    EXPECT_TRUE(iter != previter);
                    EXPECT_TRUE((iter == ++previter));
                }
                if (iter != ((const TheTrie &)aTrie).end()) {
                    typename TheTrie::ConstIterator nextiter = iter;
                    nextiter = ++nextiter;
                    EXPECT_TRUE(iter != nextiter);
                    EXPECT_TRUE((iter == --nextiter));
                }
            }
        } while (0);

        //Test Trie::find functionality
        for (typename TheTrie::Iterator iter = aTrie.begin();
                iter != aTrie.end(); ++iter) {
            bool res = (iter == aTrie.find(iter->first));
            EXPECT_TRUE(res);
            res = (iter != ++(aTrie.find(iter->first)));
            EXPECT_TRUE(res);
            typename TheTrie::ConstIterator c_iter = iter;
            res = (c_iter == ((const TheTrie &)aTrie).find(c_iter->first));
            EXPECT_TRUE(res);
        }

        //Test Trie::hasKey functionality
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            testKeyInTrie(aTrie, key, true);

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first, sv)) {
                testKeyInTrie(aTrie, key, false);
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            TrieTestCases::testKeyInTrie(aTrie, negKey, false);
        }

        //Test Trie::startsWith functionality
        char endKeyStr[] = {endSymbol, '\0'};
        teststartsWith(aTrie, endKeyStr, sv);

        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string keyStr = iter->first;
            if (keyStr.length() > 0) {
                std::string keyPart = keyStr.substr(0, std::rand() % keyStr.length());
                keyPart.append(std::string(&endSymbol, 1));
                teststartsWith(aTrie, keyPart, sv);
            }
        }

        //Test Trie::startsWith() const functionality
        ((const TrieTestCases *)this)->teststartsWith((const TheTrie &)aTrie, endKeyStr, sv);

        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string keyStr = iter->first;
            if (keyStr.length() > 0) {
                std::string keyPart = keyStr.substr(0, std::rand() % keyStr.length());
                keyPart.append(std::string(&endSymbol, 1));
                ((const TrieTestCases *)this)->teststartsWith((const TheTrie &)aTrie, keyPart, sv);
            }
        }

        //Test Trie::erase functionality
        unsigned int trieSize = aTrie.size();
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string key = iter->first;
            key.append(std::string(&endSymbol, 1));
            EXPECT_TRUE(aTrie.erase(key.c_str()));
            EXPECT_TRUE(aTrie.hasKey(key.c_str()) == false);
            EXPECT_TRUE(--trieSize == aTrie.size());
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            EXPECT_TRUE(aTrie.erase(negKey.c_str()) == false);
        }

        TrieTestCases::populateTrieWithSampleValues(aTrie, sv);
        trieSize = aTrie.size();
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string key = iter->first;
            key.append(std::string(&endSymbol, 1));
            EXPECT_TRUE(aTrie.erase(aTrie.find(key.c_str())));
            EXPECT_TRUE(aTrie.hasKey(key.c_str()) == false);
            EXPECT_TRUE(--trieSize == aTrie.size());
        }

        //Test Trie::clear functionality
        TrieTestCases::populateTrieWithSampleValues(aTrie, sv);
        aTrie.clear();
        int elementCount = 0;
        for (typename TheTrie::Iterator iter = aTrie.begin();
                iter != aTrie.end(); ++iter) {
            ++elementCount;
        }
        EXPECT_TRUE(elementCount == 0);
        EXPECT_TRUE(aTrie.empty());

        //Test Trie::operator[] functionality
        TrieTestCases::populateTrieWithSampleValues(aTrie, sv);
        for (SampleValuesIter iter = sv.begin();
                iter != sv.end(); ++iter) {
            std::string key = iter->first;
            key.append(std::string(&endSymbol, 1));
            bool res = aTrie[key.c_str()].compare(*aTrie.get(key.c_str())) == 0;
            EXPECT_TRUE(res);
        }
    }
};

#endif

