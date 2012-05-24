/*
 * Copyright (c) 2011, Ranjith TV
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause ("BSD New" or "BSD Simplified") license.
 * You may obtain a copy of the License at
 *
 * http://www.opensource.org/licenses/BSD-3-Clause
 *
 */

/*!
 * @mainpage trie
 * @section intro_sec Introduction
 * This is a simple implementation of Trie.
 * @section Configurations
 * @subsection  MACRO
 * <table border="0">
 * <tr>
 * <td>HIGH_PERFORMANCE</td>
 * <td>
 * For performace oriented applications.
 * This macro will create an array for the key characters which
 * enables uses of random access and speed up the lookup and insertion of the key
 * characters.This option will consume more memory.<br>
 * If the macro is not used space is allocated for just the key characters.
 * Lookup for key characers in that case takes logarithmic time,
 * so is insertion of a new characters.
 * </td>
 * </tr>
 * </table>
 */

#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <set>
#include <algorithm>

namespace rtv
{

template<typename T,
         typename V,
         T end,
         typename Cmp> class Node;

template<typename T,
         typename V,
         T end,
         typename Cmp> class NodeItem {
public:
    NodeItem() 
     : mKey(end),
       mChilds(0) {}

    NodeItem(T const & key) 
     : mKey(key),
       mChilds(0) {
        createChilds();
    }

    virtual ~NodeItem() {
        delete mChilds;
    }

    bool operator<(NodeItem<T, V, end, Cmp> const & oth) const {
        return Cmp()(this->mKey, oth.mKey);
    }

    bool operator<(T const & key) const {
        return Cmp()(this->mKey, key);
    }

    bool operator==(T const & key) const {
        return !Cmp()(this->mKey, key) && !Cmp()(key, this->mKey);
    }

    bool operator==(NodeItem<T, V, end, Cmp> const & oth) const {
        return !Cmp()(this->mKey, oth.mKey) && !Cmp()(oth.mKey, this->mKey);
    }

    void set(T const & key) {
        mKey = key;
        createChilds();
    }

    const T & get() const {
        return mKey;
    }

    Node<T, V, end, Cmp> * getChilds() {
        createChilds();
        return mChilds;
    }

private:
    void createChilds() {
        if (!mChilds) {
            mChilds = new Node<T, V, end, Cmp>();
        }
    }

    NodeItem(NodeItem const &);
    NodeItem & operator=(NodeItem const &);

private:
    T mKey;
    Node<T, V, end, Cmp> * mChilds;
};

template<typename T,
         typename V,
         T end,
         typename Cmp> class EndNodeItem: public NodeItem<T, V, end, Cmp>
{
    typedef NodeItem<T, V, end, Cmp> ParentClass;
public:
    EndNodeItem(T const & key) 
     : ParentClass(key)
    {}

    EndNodeItem(T const & key, V const & value) 
     : ParentClass(key),
       mValue(value)
    {}

    void set(T const & key, V const & value) {
        ParentClass::set(key);
        mValue = value;
    }

    const V & getValue() const {
        return mValue;
    }

private:
    EndNodeItem(EndNodeItem const &);
    EndNodeItem & operator=(EndNodeItem const &);

private:
    V mValue;
};

template<typename T,
         typename V,
         T end,
         typename Cmp> class NodeItemPtrCompare {
public:
    bool operator()(const NodeItem<T, V, end, Cmp> * v1, const NodeItem<T, V, end, Cmp> * v2) {
        return *v1 < *v2;
    }
};

template<typename T,
         typename V,
         T end,
         typename Cmp> class Node
{
public:
    Node(int max = 256) 
#ifdef HIGH_PERFORMANCE
        : mItems(max, 0)
#endif
    {}

    static void deleteItem(NodeItem<T, V, end, Cmp> * item) {
        delete item;
    }

    ~Node() {
        std::for_each(mItems.begin(), mItems.end(), deleteItem);
        mItems.clear();
    }

    void add(const T * key, V const & value) {
        addData(key, value, 0);
    }

    const V * get(const T * key) {
        return get(key, 0);
    }

    bool hasKey(const T * key) {
        return hasKey(key, 0);
    }

    void startsWith(const T * key, std::vector< std::pair<std::vector<T>, V> > & values, int count) {
        startsWith(key, values, count, 0);
    }

    template <typename CB>
    void traverse(CB const & cb) {
        std::vector<T> v;
        traverse(v, cb);
    }

private:
    Node(Node const &);
    Node & operator=(Node const &);

    template <typename CB>
    void traverse(std::vector<T> key, CB const & cb) {

        ItemsContainerIter iterEnd = mItems.end();
        for (ItemsContainerIter iter = mItems.begin(); iter != iterEnd; ++iter) {
            if (*iter) {
                NodeItem<T, V, end, Cmp> & item = *(NodeItem<T, V, end, Cmp> *) *iter;
                if (item == end) {                    
                    cb(key, ((EndNodeItem<T, V, end, Cmp>&)item).getValue());
                } else {
                    key.push_back(item.get());
                    item.getChilds()->traverse(key, cb);
                    key.pop_back();
                }
            }
        }
    }

    void startsWith(const T * key, std::vector< std::pair<std::vector<T>, V> > & values, int count, int i) {
        
        if (key[i] == end) {
            std::vector<T> valueSoFar;
            for (int i=0;key[i] != end;++i) {
                valueSoFar.push_back(key[i]);
            }
            
            accumulate(valueSoFar, values, count);
            return;
        }

        NodeItem<T, V, end, Cmp> * item = getItem(key[i]);
        if (!item) {
            return;
        } else if (*item == key[i]) {
            item->getChilds()->startsWith(key, values, count, ++i);
        }
    }

    void accumulate(std::vector<T> key, std::vector< std::pair<std::vector<T>, V> > & values, int count) {
        ItemsContainerIter iterEnd = mItems.end();
        for (ItemsContainerIter iter = mItems.begin(); iter != iterEnd && count > 0; ++iter) {
            if (*iter != 0) {
                NodeItem<T, V, end, Cmp> & item = *(NodeItem<T, V, end, Cmp> *)*iter;
                if (item == end) {
                    std::vector<T> result;
                    result.assign(key.begin(), key.end());
                    values.push_back(std::make_pair(result, ((EndNodeItem<T, V, end, Cmp>&)item).getValue()));
                    --count;
                } else {
                    key.push_back(item.get());
                    item.getChilds()->accumulate(key, values, count);
                    key.pop_back();
                }
            }
        }
    }

    const V * get(const T * key, int i) {
        NodeItem<T, V, end, Cmp> * itemp = getItem(key[i]);
        if (!itemp) {
            return 0;
        }
        NodeItem<T, V, end, Cmp> & item = *itemp;
        if (key[i] == end && item == end) {
            return &(((EndNodeItem<T, V, end, Cmp>&)item).getValue());
        } else if (item == key[i]) {
            return item.getChilds()->get(key, ++i);
        } else {
            return 0;
        }
    }

    bool hasKey(const T * key, int i) {
        NodeItem<T, V, end, Cmp> * itemp = getItem(key[i]);
        if (!itemp) {
            return false;
        }

        NodeItem<T, V, end, Cmp> & item = *itemp;
        
        if (key[i] == end && item == end) {
            return true;
        } else if (item == key[i]) {
            return item.getChilds()->hasKey(key, ++i);
        } else {
            return false;
        }
    }

    void addData(const T * key, V const & value, int i) {

        NodeItem<T, V, end, Cmp> * item = addItem(key[i]);

        if (key[i] == end) {
            ((EndNodeItem<T, V, end, Cmp>*)item)->set(key[i], value);
        } else if (*item == key[i]) {
            item->getChilds()->addData(key, value, ++i);
        }
    }


    NodeItem<T, V, end, Cmp> * createNodeItem(T const & k) {
        if (k == end) {
            return new EndNodeItem<T, V, end, Cmp>(k);
        } else {
            return new NodeItem<T, V, end, Cmp>(k);
        }
    }

    NodeItem<T, V, end, Cmp> * addItem(T const & k) {
#ifdef HIGH_PERFORMANCE
        if (!mItems[k]) {
            mItems[k] = createNodeItem(k);
        }
        NodeItem<T, V, end, Cmp> & item = *mItems[k];
        return &item;
#else
        NodeItem<T, V, end, Cmp> tmp(k);
        ItemsContainerIter iter = mItems.find(&tmp);
        if (iter == mItems.end()) {
            NodeItem<T, V, end, Cmp> * v = createNodeItem(k);
            mItems.insert(v);
            return v;
        } else {
            return (NodeItem<T, V, end, Cmp> *) *iter;
        }
#endif
    }

    NodeItem<T, V, end, Cmp> * getItem(T const & k) {
#ifdef HIGH_PERFORMANCE
        return mItems[k];
#else
        NodeItem<T, V, end, Cmp> tmp(k);

        ItemsContainerIter iter = mItems.find(&tmp);
        if (iter == mItems.end()) {
            return 0;
        }
        return (NodeItem<T, V, end, Cmp> *) (*iter);
#endif
    }

private:

#ifdef HIGH_PERFORMANCE
    typedef std::vector< NodeItem<T, V, end, Cmp>* > ItemsContainer;
#else
    typedef std::set< NodeItem<T, V, end, Cmp>*, NodeItemPtrCompare<T, V, end, Cmp> > ItemsContainer;
#endif
    typedef typename ItemsContainer::iterator ItemsContainerIter;
    typedef typename ItemsContainer::const_iterator ItemsContainerConster;
    ItemsContainer mItems;
};

/*!
 * @brief Trie implementation class
 * @tparam T Type for each element in the key
 * @tparam V Type of the value that the key will be representing
 * @tparam end Key element representing end of key
 * @tparam Cmp Comparison functor
 */
template<typename T,
         typename V,
         T end,
         typename Cmp=std::less<T> > class Trie
{
public:
    Trie()
    {}

    /*!
     * @param max Number of elements in the key space
     */
    Trie(int max)
        : mRoot(max)
    {}

    /*!
     * Add a key with value in to the Trie
     * @param key The key, should be terminated by 'end' characters
     * @param value The value that is to be set with the key
     */
    void add(const T * key, V const & value) {
        mRoot.add(key, value);
    }

    /*!
     * Retrieves the value for the given key
     * @param key The key, should be terminated by 'end' characters
     * @return Pointer to value for the given key, 0 on failure
     */
    const V * get(const T * key) {
        return mRoot.get(key);
    }

    /*!
     * Checks whether the given key is present in the Trie
     * @param key The key which should be searched, should be terminated by 'end' characters
     * @return true if the key is present
     */
    bool hasKey(const T * key) {
        return mRoot.hasKey(key);
    }

    /*!
     * Retrieves all the key value pair which starts with the given key
     * @param key Part of the key which should be searched
     * @param values Vector of key, value pair
     * @param count Number of entries that should be returned
     */
    void startsWith(const T * key, std::vector< std::pair<std::vector<T>, V> > & values, int count = 5) {
        mRoot.startsWith(key, values, count);
    }

    /*!
     * Traverse through the Trie
     * @param c The functor class which will called when a node is reached
     */
    template <typename CB>
    void traverse(CB const & c) {
        mRoot.traverse(c);
    }

private:
    Trie(Trie const &);
    Trie & operator=(Trie const &);

private:
    Node<T, V, end, Cmp> mRoot;
};

}

#endif
