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
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <ctime>

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

class TrieTestCases
{
private:
    typedef void (*TestCaseFunction)();

public:
    static TrieTestCases *instance() {
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
    void populateTrieWithSampleValues(D   &aTrie) {
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            char endSymbol = aTrie.endSymbol();
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            TrieTestCases::testAndAddToTrie(aTrie, key, iter->second);
        }
    }

    template <typename D>
    void testKeyInTrie(D   &aTrie, std::string const &word, bool expected) {
        if (aTrie.hasKey(word.c_str())) {
            testResult(expected, "Found", word.c_str(), "when it should not be found !!!");
        } else {
            testResult(!expected, "Did not find", word.c_str(), "when it should be !!!");
        }
    }

    template <typename D>
    void testAndAddToTrie(D &aTrie, const std::string &word, const std::string &meaning) {
        unsigned int trieSize = aTrie.size();
        std::pair<typename D::Iterator, bool> res = aTrie.insert(word.c_str(), meaning.c_str());
        if (res.second) {
            std::string keyStr = keyToString(aTrie.endSymbol(), res.first->first);
            testResult(word.compare(keyStr) == 0, "Error in the Iterator returned after insertion!!!Expected", word.c_str(),
                                                  "Actual", keyStr.c_str());
            testResult(res.first->second->compare(meaning) == 0, "Error in the Iterator returned after insertion!!! Expected",
                                                                 meaning.c_str(), 
                                                                 "Actual", res.first->second->c_str());
        }
        testResult(aTrie.get(word.c_str())->compare(meaning.c_str()) == 0, "Error in Trie::insert or Trie::get !!!");
        testResult(aTrie.size() == trieSize + 1, "Trie size did not updated properly after Trie::insert!!!");
    }

    template<typename D>
    void teststartsWith(D & aTrie, const std::string &key) {
        if (key.length() > 0) {
            unsigned int countForKey = 0;
            for (SampleValuesIter iter = mSampleValues.begin();
                 iter != mSampleValues.end(); ++iter) {
                int cv = key.compare(0, key.length() - 1, iter->first, 0, key.length() - 1);
                if (cv == 0) {
                    ++countForKey;
                }
            }
            typename D::Iterator iter = aTrie.startsWith(key.c_str());
            unsigned int countStartsWith = 0;
            for (; iter != aTrie.end(); ++iter) {
                std::string keyStr = keyToString(aTrie.endSymbol(), iter->first);
                keyStr.erase(keyStr.length()-1);
                bool res = isPresent(keyStr);
                TrieTestCases::testResult(res, "Invalid key in Iterator returned from Trie::startsWith!!! For prefix",
                    key.c_str(), "Returned Key",
                    keyToString(aTrie.endSymbol(), iter->first).c_str());
                ++countStartsWith;
            }

            TrieTestCases::testResult(countStartsWith == countForKey, "Invalid count returned by Trie::startsWith!!!");
        }
    }

    template<typename D>
    void teststartsWith(const D & aTrie, const std::string &key) const {
        if (key.length() > 0) {
            unsigned int countForKey = 0;
            for (SampleValuesConstIter iter = mSampleValues.begin();
                 iter != mSampleValues.end(); ++iter) {
                int cv = key.compare(0, key.length() - 1, iter->first, 0, key.length() - 1);
                if (cv == 0) {
                    ++countForKey;
                }
            }
            typename D::ConstIterator iter = aTrie.startsWith(key.c_str());
            unsigned int countStartsWith = 0;
            for (; iter != aTrie.end(); ++iter) {
                std::string keyStr = keyToString(aTrie.endSymbol(), iter->first);
                keyStr.erase(keyStr.length()-1);
                bool res = isPresent(keyStr);
                TrieTestCases::testResult(res, "Invalid key in Iterator returned from Trie::startsWith!!! For prefix",
                    key.c_str(), "Returned Key",
                    keyToString(aTrie.endSymbol(), iter->first).c_str());
                ++countStartsWith;
            }

            TrieTestCases::testResult(countStartsWith == countForKey, "Invalid count returned by Trie::startsWith!!!");
        }
    }

    std::string keyToString(char endSymbol, const char *key) const{
        int i = 0;
        for (; key[i] != endSymbol; ++i) {
        }
        return std::string(key, i + 1);
    }

    template <typename D>
    void testSuite(D &aTrie) {

        char endSymbol = aTrie.endSymbol();

        std::srand((unsigned int)std::time(0));

        //Test Trie::empty functionality
        aTrie.clear();
        testResult(aTrie.empty(), "Trie::empty failed!!!");

        //Test Trie::insert functionality
        TrieTestCases::populateTrieWithSampleValues(aTrie);

        //Test Trie::find functionality
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            typename D::Iterator titer = aTrie.find(key.c_str());
            testResult(key.compare(keyToString(aTrie.endSymbol(), titer->first)) == 0, "Trie::find() failed for key!!!");
            testResult(iter->second.compare(*(titer->second)) == 0, "Trie::find() failed for value!!!");

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first)) {
                testResult(aTrie.find(key.c_str()) == aTrie.end(), "For negative test Trie::find() failed!!! Found key", key.c_str());
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            testResult(aTrie.find(negKey.c_str()) == aTrie.end(), "For negative test Trie::get() failed!!! Found key", negKey.c_str());
        }


        //Test Trie::find() const functionality
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            typename D::ConstIterator titer = ((const D &) (aTrie)).find(key.c_str());
            testResult(key.compare(keyToString(aTrie.endSymbol(), titer->first)) == 0, "Trie::find() failed for key!!!");
            testResult(iter->second.compare(*(titer->second)) == 0, "Trie::find() failed for value!!!");

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first)) {
                testResult(((const D &) (aTrie)).find(key.c_str()) == ((const D &) (aTrie)).end(), "For negative test Trie::find() failed!!! Found key", key.c_str());
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            testResult(((const D &) (aTrie)).find(negKey.c_str()) == ((const D &) (aTrie)).end(), "For negative test Trie::get() failed!!! Found key", negKey.c_str());
        }

        //Test Trie::get functionality
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            testResult((aTrie.get(key.c_str()))->compare(iter->second) == 0, "Trie::get() failed!!!");

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first)) {
                testResult(aTrie.get(key.c_str()) == 0, "For negative test Trie::get() failed!!! Found key", key.c_str());
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            testResult(aTrie.get(negKey.c_str()) == 0, "For negative test Trie::get() failed!!! Found key", negKey.c_str());
        }


        //Test Trie::get() const functionality
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            testResult((((D const &)aTrie).get(key.c_str()))->compare(iter->second) == 0, "Trie::get() failed!!!");

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first)) {
                testResult(((D const &)aTrie).get(key.c_str()) == 0, "For negative test Trie::get() failed!!!");
            }
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
                iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            testResult(((D const &)aTrie).get(negKey.c_str()) == 0, "For negative test Trie::get() failed!!! Found key", negKey.c_str());
        }

        //Test Trie::size functionality
        testResult(aTrie.size() == mSampleValues.size(), "Trie::size failed!!!",
                   "Trie::size returned", aTrie.size(),
                   "Expected", mSampleValues.size());

        //Test Trie::ConstIterator functionality
        SampleValues copySamples = mSampleValues;
        std::sort(copySamples.begin(), copySamples.end(), PairSort());

        SampleValuesIter siter = copySamples.begin();

        // operator++()
        for (typename D::ConstIterator iter = ((const D &)aTrie).begin();
             iter != ((const D &)aTrie).end(); ++iter, ++siter) {
            std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
            iterStr.erase(iterStr.length() - 1);
            testResult(siter->first.compare(iterStr) == 0,
                       "Error in Trie::Iterator traversal!!! \nExpected Key",
                       siter->first.c_str(), "Actual Key", iterStr.c_str());

            testResult(siter->second.compare(iter->second->c_str()) == 0,
                       "Error in Trie::Iterator traversal!!! Expected Key",
                       siter->second.c_str(), "Actual Key", iter->second->c_str());
        }

        // operator++(int)
        siter = copySamples.begin();
        for (typename D::ConstIterator iter = ((const D &)aTrie).begin();
             iter != ((const D &)aTrie).end(); iter++, siter++) {
            std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
            iterStr.erase(iterStr.length() - 1);
            testResult(siter->first.compare(iterStr) == 0,
                       "Error in Trie::Iterator traversal!!! Expected Key",
                       siter->first.c_str(), "Actual Key", iterStr.c_str());

            testResult(siter->second.compare(iter->second->c_str()) == 0,
                       "Error in Trie::Iterator traversal!!! Expected Key",
                       siter->second.c_str(), "Actual Key", iter->second->c_str());
        }

        //Test Trie::Iterator functionality
        std::sort(copySamples.begin(), copySamples.end(), PairSort());

        siter = copySamples.begin();

        // operator++()
        for (typename D::Iterator iter = aTrie.begin();
             iter != aTrie.end(); ++iter, ++siter) {
            std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
            iterStr.erase(iterStr.length() - 1);
            testResult(siter->first.compare(iterStr) == 0,
                       "Error in Trie::Iterator traversal!!! Expected Key",
                       siter->first.c_str(), "Actual Key", iterStr.c_str());

            testResult(siter->second.compare(iter->second->c_str()) == 0,
                       "Error in Trie::Iterator traversal!!! Expected Key",
                       siter->second.c_str(), "Actual Key", iter->second->c_str());
        }

        // operator++(int)
        siter = copySamples.begin();
        for (typename D::Iterator iter = aTrie.begin();
             iter != aTrie.end(); iter++, siter++) {
            std::string iterStr = keyToString(aTrie.endSymbol(), iter->first);
            iterStr.erase(iterStr.length() - 1);
            testResult(siter->first.compare(iterStr) == 0,
                       "Error in Trie::Iterator traversal!!! Expected Key",
                       siter->first.c_str(), "Actual Key", iterStr.c_str());

            testResult(siter->second.compare(iter->second->c_str()) == 0,
                       "Error in Trie::Iterator traversal!!! Expected Key",
                       siter->second.c_str(), "Actual Key", iter->second->c_str());
        }

        //Iterator other functionality
        for (typename D::ConstIterator iter = ((const D &)aTrie).begin();
             iter != ((const D &)aTrie).end(); ++iter) {
            bool res = (iter == ((const D &)aTrie).find(iter->first));
            testResult(res, "Iterator different from Iterator from find method when it should be same!!!");
            res = (iter != ++(((const D &)aTrie).find(iter->first)));
            testResult(res, "Iterator same when it should be differnt!!!");
            res = (iter == ((const D &)aTrie).startsWith(iter->first));
            testResult(res, "Iterator different from Iterator from startsWith method when it should be same!!!");
        }
        for (typename D::Iterator iter = aTrie.begin();
             iter != aTrie.end(); ++iter) {
            bool res = (iter == aTrie.find(iter->first));
            testResult(res, "Iterator different when it should be same!!!");
            res = (iter != ++(aTrie.find(iter->first)));
            testResult(res, "Iterator same when it should be differnt!!!");
            typename D::ConstIterator c_iter = iter;
            res = (c_iter == ((const D &)aTrie).find(c_iter->first));
            testResult(res, "Iterator different when it should be same!!!");
        }

        //Test Trie::hasKey functionality
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string key = iter->first;
            key.append(&endSymbol, 1);
            testKeyInTrie(aTrie, key, true);

            int rn = std::rand() % iter->first.length();
            if (!rn) {
                rn = 1;
            }
            key = iter->first.substr(0, rn);
            key.append(&endSymbol, 1);

            if (!isPresent(iter->first)) {
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
        teststartsWith(aTrie, endKeyStr);

        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string keyStr = iter->first;
            if (keyStr.length() > 0) {
                std::string keyPart = keyStr.substr(0, std::rand() % keyStr.length());
                keyPart.append(std::string(&endSymbol, 1));
                teststartsWith(aTrie, keyPart);
            }
        }

        //Test Trie::startsWith() const functionality
        ((const TrieTestCases *)this)->teststartsWith((const D &)aTrie, endKeyStr);

        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string keyStr = iter->first;
            if (keyStr.length() > 0) {
                std::string keyPart = keyStr.substr(0, std::rand() % keyStr.length());
                keyPart.append(std::string(&endSymbol, 1));
                ((const TrieTestCases *)this)->teststartsWith((const D &)aTrie, keyPart);
            }
        }

        //Test Trie::erase functionality
        unsigned int trieSize = aTrie.size();
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string key = iter->first;
            key.append(std::string(&endSymbol, 1));
            testResult(aTrie.erase(key.c_str()), "Removing ", iter->first.c_str(), "failed!!!");
            testResult(aTrie.hasKey(key.c_str()) == false, "Removing ", iter->first.c_str(), "failed!!!");
            testResult(--trieSize == aTrie.size(), "Trie size is not updated after remove operation!!!");
        }

        for (NegativeSampleValuesIter iter = mNegativeSampleValues.begin();
             iter != mNegativeSampleValues.end(); ++iter) {
            std::string negKey = *iter;
            negKey.append(std::string(&endSymbol, 1));
            testResult(aTrie.erase(negKey.c_str()) == false, "Negative test for Trie::erase(", negKey.c_str(), ")failed!!!");
        }

        TrieTestCases::populateTrieWithSampleValues(aTrie);
        trieSize = aTrie.size();
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string key = iter->first;
            key.append(std::string(&endSymbol, 1));
            testResult(aTrie.erase(aTrie.find(key.c_str())), "Removing ", iter->first.c_str(), "failed!!!");
            testResult(aTrie.hasKey(key.c_str()) == false, "Removing ", iter->first.c_str(), "failed!!!");
            testResult(--trieSize == aTrie.size(), "Trie size is not updated after remove operation!!!");
        }

        //Test Trie::clear functionality
        TrieTestCases::populateTrieWithSampleValues(aTrie);
        aTrie.clear();
        int elementCount = 0;
        for (typename D::Iterator iter = aTrie.begin();
             iter != aTrie.end(); ++iter) {
                 ++elementCount;
        }
        testResult(elementCount == 0,
                    "Error in Trie::clear functionality!!! Expected count", 0,
                    "Actual count", elementCount);
        testResult(aTrie.empty(), "Trie::clear failed!!!");

        //Test Trie::operator[] functionality
        TrieTestCases::populateTrieWithSampleValues(aTrie);
        for (SampleValuesIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
            std::string key = iter->first;
            key.append(std::string(&endSymbol, 1));
            bool res = aTrie[key.c_str()].compare(*aTrie.get(key.c_str())) == 0;
            testResult(res, "operator[] != Trie::get()!!!");
        }
    }

    template <typename T>
    void testResult(bool result, const T &message) const {
        if (!result) {
            std::cerr << message << std::endl;
            std::abort();
        }
    }

    template <typename T1, typename T2>
    void testResult(bool result,
                    const T1 &message1,
                    const T2 &message2) const {
        if (!result) {
            std::cerr << message1 << " " <<
                      message2 << std::endl;
            std::abort();
        }
    }

    template <typename T1, typename T2, typename T3>
    void testResult(bool result,
                    const T1 &message1,
                    const T2 &message2,
                    const T3 &message3) const {
        if (!result) {
            std::cerr << message1 << " " <<
                      message2 << " " <<
                      message3 << std::endl;
            std::abort();
        }
    }

    template <typename T1, typename T2, typename T3, typename T4>
    void testResult(bool result,
                    const T1 &message1,
                    const T2 &message2,
                    const T3 &message3,
                    const T4 &message4) const {
        if (!result) {
            std::cerr << message1 << " " <<
                      message2 << " " <<
                      message3 << " " <<
                      message4 << std::endl;
            std::abort();
        }
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    void testResult(bool result,
                    const T1 &message1,
                    const T2 &message2,
                    const T3 &message3,
                    const T4 &message4,
                    const T5 &message5) const {
        if (!result) {
            std::cerr << message1 << " " << message2 << " " <<
                      message3 << " " << message4 << " " <<
                      message5 << std::endl;
            std::abort();
        }
    }

    bool isPresent(std::string const & key) const {
        for (SampleValuesConstIter iter = mSampleValues.begin();
             iter != mSampleValues.end(); ++iter) {
                 if (iter->first.compare(key) == 0) {
                     return true;
                 }
        }
        return false;
    }

private:
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

        for (unsigned int i = 0; i < sizeof(sampleValues) / sizeof(sampleValues[0]); ++i) {
            mSampleValues.insert(mSampleValues.begin(), sampleValues[i]);
        }

        for (unsigned int i = 0; i < sizeof(negativeSampleValues) / sizeof(negativeSampleValues[0]); ++i) {
            mNegativeSampleValues.push_back(negativeSampleValues[i]);
        }
    }

    TrieTestCases(TrieTestCases const &);
    TrieTestCases &operator=(TrieTestCases const &);

private:
    typedef std::vector< std::pair<std::string, std::string> > SampleValues;
    typedef SampleValues::iterator SampleValuesIter;
    typedef SampleValues::const_iterator SampleValuesConstIter;

    typedef std::vector<std::string> NegativeSampleValues;
    typedef NegativeSampleValues::iterator NegativeSampleValuesIter;
    typedef NegativeSampleValues::const_iterator NegativeSampleValuesConstIter;

    std::vector<TestCaseFunction> mTestCases;
    SampleValues mSampleValues;
    NegativeSampleValues mNegativeSampleValues;
};

#endif
