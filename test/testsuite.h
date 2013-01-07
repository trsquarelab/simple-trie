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

#ifndef TESTSUITE_H
#define TESTSUITE_H

#include <sstream>
#include <string>
#include <set>
#include <map>
#include <iostream>

namespace rtv
{

class RTest
{
public:
    enum Status
    {
        Passed,
        Failed,
        Disabled
    };

    RTest()
     : mStatus(Passed)
    {}

    RTest(std::string const & name)
     : mName(name),
       mStatus(Passed)
    {}

    virtual ~RTest()
    {}

    void setName(std::string const & name)
    {
        mName = name;
    }

    std::string name() const
    {
        return mName;
    }

    void setGroup(std::string const & group)
    {
        mGroup = group;
    }

    std::string group() const
    {
        return mGroup;
    }

    virtual void exec() = 0;

    Status status() const
    {
        return mStatus;
    }

    std::string message() const
    {
        return mMessage;
    }

    bool testCondition(bool actual, bool expected,
                     std::string const & condition,
                     std::string const & file, int line) const
    {
        return const_cast<RTest *>(this)->testCondition(actual, expected,
                                                 condition, file, line);
    }

    bool testCondition(bool actual, bool expected,
                     std::string const & condition,
                     std::string const & file, int line)
    {
        if (mStatus == Passed) {
            if (expected != actual) {
                std::ostringstream outstream;
                outstream << condition
                          << " Failed in " << mGroup << "::" << mName
                          << "(" << file << ":" << line << ")";
            
                mMessage = outstream.str();
                mStatus = Failed;
            }
        }
        return mStatus == Passed;
    }

protected:
    void setStatus(Status s)
    {
        mStatus = s;
    }

private:
    std::string mName;
    std::string mGroup;
    std::string mMessage;
    Status mStatus;
};

class RTestWriter
{
public:
    RTestWriter()
     : mOut(&std::cout)
    {}

    RTestWriter& operator<<(std::string const &v)
    {
        (*mOut) << v.c_str();
        return *this;
    }

    RTestWriter& operator<<(std::ostream& (*pf)(std::ostream&))
    {
        pf(*mOut);
        return *this;
    }

    RTestWriter& operator<<(const char *v)
    {
        (*mOut) << v;
        return *this;
    }

    RTestWriter& operator<<(int v)
    {
        (*mOut) << v;
        return *this;
    }

    RTestWriter& operator<<(char v)
    {
        (*mOut) << v;
        return *this;
    }

private:
    std::ostream * mOut;
};

class RTestReporter
{
public:
    RTestReporter()
    {}

    virtual ~RTestReporter()
    {}

    virtual void startTesting(int totalGroup, int totalTests)
    {
        mTotalTests = totalTests;
        mTotalTestGroups = totalGroup;
        mTotalTestPassed = 0;
        mTotalTestFailed = 0;
        mTotalTestDisabled = 0;
    }

    virtual void endTesting()
    {}

    virtual void startGroup(std::string const & name, int testCount)
    {
        mCurrentGroup = name;
        mGroupTestCount = testCount;
        mTestPassedCount = 0;
        mTestFailedCount = 0;
        mTestDisabledCount = 0;
    }

    virtual void endGroup()
    {
        mTotalTestPassed += mTestPassedCount;
        mTotalTestFailed += mTestFailedCount;
        mTotalTestDisabled += mTestDisabledCount;
    }

    virtual void startTest(RTest * test)
    {
        mCurrentTest = test->name();
    }

    virtual void endTest(RTest * test)
    {
        switch (test->status()) {
            case RTest::Passed: {
                ++mTestPassedCount;
                break;
            }
            case RTest::Failed: {
                ++mTestFailedCount; 
                break;
            }
            case RTest::Disabled: {
                ++mTestDisabledCount;
                break;
            }
            default: {
                break;
            }
        }
    }

protected:
    std::string mCurrentGroup;
    std::string mCurrentTest;
    int mTotalTests;
    int mTotalTestGroups;
    int mTotalTestPassed;
    int mTotalTestFailed;
    int mTotalTestDisabled;
    int mTestPassedCount;
    int mTestFailedCount;
    int mTestDisabledCount;
    int mGroupTestCount;
    RTestWriter mWriter;
};

class RBltnTestReporter: public RTestReporter
{
private:
    const int mMaxWidth;
    const char mHoriChar;
    const char mVertChar;
    const char mFillChar;
    const int mStatusWidth;

public:
    RBltnTestReporter()
     : mMaxWidth(80),
       mHoriChar('-'),
       mVertChar('|'),
       mFillChar(' '),
       mStatusWidth(mMaxWidth / 4)
    {
    }

    void endLine()
    {
        mWriter << std::endl;
    }

    void horizontalLine(int w)
    {
        mWriter << mVertChar;
        for (int i=0; i<w-2; ++i) {
            mWriter << mHoriChar;
        }
        mWriter << mVertChar;
    }

    void fill(int n)
    {
        for (int i=0; i<n; ++i) {
            mWriter << mFillChar;
        }
    }

    bool isEven(int n)
    {
        return (((n/2) * 2) == n);
    }

    void outString(std::string const & str, int w)
    {
        w -= 2;
        int strL = str.length();
        int nb = (strL / w) + 1; 
        if (nb == 1) {
            mWriter << mVertChar;
            int gap = (w - strL) / 2;
            fill(gap);
            mWriter << str.c_str();
            fill(gap);
            if (!isEven(w-strL)) {
                fill(1);
            }
            mWriter << mVertChar;
        } else {
            unsigned int spos = 0;
            unsigned int subL = w-2;
            do {
                outString(str.substr(spos, subL), w+2);
                spos = spos + subL;
                if (spos >= str.length()) {
                    break;
                }
                endLine();
            } while (1);
        }
    }

    void startTesting(int totalGroup, int totalTests)
    {
        RTestReporter::startTesting(totalGroup, totalTests);

        std::ostringstream outstream;
        outstream << "RUNNING " << totalTests << " TESTS FROM " << totalGroup << " TEST GROUPS";

        horizontalLine(mMaxWidth);
        endLine();
        outString(outstream.str(), mMaxWidth);
        endLine();
        horizontalLine(mMaxWidth);
        endLine();
    }

    void startGroup(std::string const & name, int testCount)
    {
        RTestReporter::startGroup(name, testCount);

        outString(name, mMaxWidth);
        endLine();
        horizontalLine(mMaxWidth);
        endLine();
    }
 
    void startTest(RTest * test)
    {
        RTestReporter::startTest(test);

        std::ostringstream outstream;
        outstream << mCurrentGroup.c_str() << "::" << mCurrentTest.c_str();

        outString(outstream.str(), mMaxWidth-mStatusWidth);
    }

    void endTest(RTest * test)
    {
        RTestReporter::endTest(test);

        switch (test->status()) {
            case RTest::Passed: {
                outString("PASSED", mStatusWidth);
                endLine();
                break;
            }
            case RTest::Failed: {
                outString("FAILED", mStatusWidth);
                endLine();
                outString(test->message(), mMaxWidth);
                endLine();
                break;
            }
            case RTest::Disabled: {
                outString("DISABLED", mStatusWidth);
                endLine();
                break;
            }
            default: {
                break;
            }
        }
    }

    void endGroup()
    {
        RTestReporter::endGroup();

        if (mGroupTestCount) {
            if (mTestPassedCount) {
                std::ostringstream outstream;
                outString("PASSED", mMaxWidth-mStatusWidth);
                outstream << mTestPassedCount;
                outString(outstream.str(), mStatusWidth);
                endLine();
            }
            if (mTestFailedCount) {
                std::ostringstream outstream;
                outString("FAILED", mMaxWidth-mStatusWidth);
                outstream.clear();
                outstream << mTestFailedCount;
                outString(outstream.str(), mStatusWidth);
                endLine();
            }
            if (mTestDisabledCount) {
                std::ostringstream outstream;
                outString("DISABLED", mMaxWidth-mStatusWidth);
                outstream << mTestDisabledCount;
                outString(outstream.str(), mStatusWidth);
                endLine();
            }

            std::ostringstream outstream;
            outString("TOTAL", mMaxWidth-mStatusWidth);
            outstream << mGroupTestCount;
            outString(outstream.str(), mStatusWidth);
            endLine();
        }

        horizontalLine(mMaxWidth);
        endLine();
    }

    void endTesting()
    {
        RTestReporter::endTesting();

        std::ostringstream outs;
        outs << mTotalTests << " tests from " << mTotalTestGroups << " test group ran";
        outString(outs.str(), mMaxWidth);
        endLine();
        horizontalLine(mMaxWidth);
        endLine();

        if (mTotalTestPassed) {
            std::ostringstream outstream;
            outString("PASSED", mMaxWidth-mStatusWidth);
            outstream << mTotalTestPassed;
            outString(outstream.str(), mStatusWidth);
            endLine();
        }
        if (mTotalTestFailed) {
            std::ostringstream outstream;
            outString("FAILED", mMaxWidth-mStatusWidth);
            outstream << mTotalTestFailed;
            outString(outstream.str(), mStatusWidth);
            endLine();
        }
        if (mTotalTestDisabled) {
            std::ostringstream outstream;
            outString("DISABLED", mMaxWidth-mStatusWidth);
            outstream << mTotalTestDisabled;
            outString(outstream.str(), mStatusWidth);
            endLine();
        }
        horizontalLine(mMaxWidth);
        endLine();
    }
};

class RTestManager
{
public:
    RTestManager()
     : mContinueOnFailure(true),
       mTotalTests(0),
       mTotalTestGroups(0),
       mReporter(0)
    {
        mReporter = new RBltnTestReporter();
    }

    ~RTestManager()
    {
        clearTests();
        delete mReporter;
    }

    bool addTest(RTest * test)
    {
        std::string group = test->group();
        TestGroupIter iter = mTestGroup.find(group);
        if (iter == mTestGroup.end()) {
            iter = mTestGroup.insert(std::pair<std::string, TestSet> (group, TestSet())).first;
            ++mTotalTestGroups;
        }

        if (iter->second.insert(test).second) {
            ++mTotalTests;
        }
        return true;
    }
    
    static RTestManager * instance()
    {
        static RTestManager _instance;
        return &_instance;
    }

    int exec()
    {
        if (mTotalTests == 0) {
            return 0;
        }

        int result = 0;

        mReporter->startTesting(mTotalTestGroups, mTotalTests);
    
        for (TestGroupIter iterGroup = mTestGroup.begin(); iterGroup != mTestGroup.end(); ++iterGroup) {
            TestSet testSet = iterGroup->second;
            mReporter->startGroup(iterGroup->first, testSet.size());
            for (TestSetIter iterTests = testSet.begin(); iterTests != testSet.end(); ++iterTests) {
                RTest *  currentTest = *iterTests;
                mReporter->startTest(currentTest);
                if (currentTest->status() != RTest::Disabled) {
                    currentTest->exec();
                    if (currentTest->status() != RTest::Passed) {
                        result = 1;
                    }
                }
                mReporter->endTest(currentTest);

            }
            mReporter->endGroup();
        }
        mReporter->endTesting();

        return result;
    }
private:
    void clearTests()
    {
        for (TestGroupIter iterGroup = mTestGroup.begin(); iterGroup != mTestGroup.end(); ++iterGroup) {
            TestSet testSet = iterGroup->second;
            for (TestSetIter iterTests = testSet.begin(); iterTests != testSet.end(); ++iterTests) {
                delete *iterTests;
            }
            testSet.clear();
        }
        mTestGroup.clear();
    }

private:
    typedef std::set<RTest *> TestSet;
    typedef TestSet::iterator TestSetIter;
    typedef TestSet::const_iterator TestSetConstIter;

    typedef std::map<std::string, TestSet> TestGroup;
    typedef TestGroup::iterator TestGroupIter;
    typedef TestGroup::const_iterator TestGroupConstIter;
    
    TestGroup mTestGroup;
    bool mContinueOnFailure;
    int mTotalTests;
    int mTotalTestGroups;
    RTestReporter * mReporter;
};

#define TESTDEF(testcase, test, parent, status) \
    class testcase##_##test##_test: public parent { \
    public: \
        testcase##_##test##_test() \
        { \
            setName(#test); \
            setGroup(#testcase); \
            setStatus(status); \
        } \
        void exec(); \
    }; \
    bool testcase##_##test##_res = RTestManager::instance()->addTest(new testcase##_##test##_test()); \
    void testcase##_##test##_test::exec()

#define TEST(testcase, test) TESTDEF(testcase, test, RTest, RTest::Passed)
#define TEST_D(testcase, test) TESTDEF(testcase, test, RTest, RTest::Disabled)
#define TEST_F(testcase, test) TESTDEF(testcase, test, testcase, RTest::Passed)
#define TEST_F_D(testcase, test) TESTDEF(testcase, test, testcase, RTest::Disabled)

#define ExcecuteTests() RTestManager::instance()->exec()

#define TEST_BOOL_CONDITION(actual, expected, condition) testCondition(actual, expected, condition, __FILE__, __LINE__)

#define EXPECT_TRUE(v) TEST_BOOL_CONDITION(v, true, "(" #v ")" )
#define EXPECT_FALSE(v) TEST_BOOL_CONDITION(v, false, "(" #v ")" )
#define EXPECT_EQ(v1, v2) TEST_BOOL_CONDITION(v1==v2, true, "(" #v1 "==" #v2 ")" )
#define EXPECT_NE(v1, v2) TEST_BOOL_CONDITION(v1!=v2, true, "(" #v1 "!=" #v2 ")" )

#define ASSERT_CONDITION(actual, expected, condition) if (!TEST_BOOL_CONDITION(actual, expected, condition)) { return; }

#define ASSERT_TRUE(v) ASSERT_CONDITION(v, true, "(" #v ")" )
#define ASSERT_FALSE(v) ASSERT_CONDITION(v, false, "(" #v ")" )
#define ASSERT_EQ(v1, v2) ASSERT_CONDITION(v1==v2, true, "(" #v1 "==" #v2 ")" )
#define ASSERT_NE(v1, v2) ASSERT_CONDITION(v1!=v2, true, "(" #v1 "!=" #v2 ")" )

}

#endif

