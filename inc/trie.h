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

/*!
 * @mainpage trie
 * @section intro_sec Introduction
 * This is an implementation of Trie.
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
         typename Cmp,
         typename Items> class Node;

template<typename T,
         typename V,
         typename Cmp,
         typename Items> class NodeItem
{
private:
    typedef Node<T, V, Cmp, Items> NodeClass;
    typedef NodeItem<T, V, Cmp, Items> NodeItemClass;

public:
    NodeItem(const T & endSymbol, T const & key) 
     : mEndSymbol(endSymbol),
       mKey(key),
       mChilds(0) {
        createChilds();
    }

    virtual ~NodeItem() {
        delete mChilds;
    }

    bool operator<(NodeItemClass const & oth) const {
        return Cmp()(this->mKey, oth.mKey);
    }

    bool operator<(T const & key) const {
        return Cmp()(this->mKey, key);
    }

    bool operator==(T const & key) const {
        return !Cmp()(this->mKey, key) && !Cmp()(key, this->mKey);
    }

    bool operator==(NodeItemClass const & oth) const {
        return !Cmp()(this->mKey, oth.mKey) && !Cmp()(oth.mKey, this->mKey);
    }

    void set(T const & key) {
        mKey = key;
        createChilds();
    }

    const T & get() const {
        return mKey;
    }

    NodeClass * getChilds() {
        createChilds();
        return mChilds;
    }

private:
    void createChilds() {
        if (!mChilds) {
            mChilds = new NodeClass(mEndSymbol);
        }
    }

    NodeItem(NodeItem const &);
    NodeItem & operator=(NodeItem const &);

private:
    const T mEndSymbol;
    T mKey;
    NodeClass * mChilds;
};

template<typename T,
         typename V,
         typename Cmp,
         typename Items> class EndNodeItem: public NodeItem<T, V, Cmp, Items>
{
private:
    typedef NodeItem<T, V, Cmp, Items> ParentClass;

public:
    EndNodeItem(const T & endSymbol, T const & key) 
     : ParentClass(endSymbol, key)
    {}

    EndNodeItem(const T & endSymbol, T const & key, V const & value) 
     : ParentClass(endSymbol, key),
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
         typename Cmp,
         typename Items> class NodeItemPtrCompare
{
private:
    typedef NodeItem<T, V, Cmp, Items> NodeItemClass;

public:
    bool operator()(const NodeItemClass * v1, const NodeItemClass * v2) {
        return *v1 < *v2;
    }
};

template<typename T,
         typename V,
         typename Cmp,
         typename Items> class Node
{
private:
    typedef NodeItem<T, V, Cmp, Items> NodeItemClass;
    typedef EndNodeItem<T, V, Cmp, Items> EndNodeItemClass;

public:
    Node(const T & endSymbol)
        : mItems(0),
          mEndSymbol(endSymbol),
          mSize(0) {
        mItems = new Items(endSymbol, this);
    }

    ~Node() {
        clear();
        delete mItems;
    }

    void clear() {
        std::for_each(mItems->begin(), mItems->end(), deleteItem);
        mItems->clear();
        mSize = 0;
    }

    bool empty() const {
        return size() == 0;
    }

    unsigned int size() const {
        return mSize;
    }

    bool insert(const T * key, V const & value) {
        bool inserted = insertData(key, value, 0);
        if (inserted) {
            ++mSize;
        }
        return inserted;
    }

    bool erase(const T * key) {
        bool finished = false;
        bool erased = erase(key, 0, finished);
        if (erased) {
            --mSize;
        }
        return erased;
    }

    const V * get(const T * key) {
        return get(key, 0);
    }

    bool hasKey(const T * key) {
        return hasKey(key, 0);
    }

    void startsWith(const T * key, std::vector< std::pair<std::vector<T>, V> > & values, int count) {
        std::vector<T> keySoFar;
        startsWith(keySoFar, key, values, count, 0);
    }

    template <typename CB>
    void traverse(CB const & cb) {
        std::vector<T> v;
        traverse(v, cb);
    }

    NodeItemClass * createNodeItem(T const & k) {
        if (k == mEndSymbol) {
            return new EndNodeItemClass(mEndSymbol, k);
        } else {
            return new NodeItemClass(mEndSymbol, k);
        }
    }

private:
    Node(Node const &);
    Node & operator=(Node const &);

    static void deleteItem(NodeItemClass * item) {
        delete item;
    }

    template <typename CB>
    void traverse(std::vector<T> & key, CB const & cb) {
        ItemsContainerIter iterEnd = mItems->end();

        for (ItemsContainerIter iter = mItems->begin(); iter != iterEnd; ++iter) {
            if (*iter) {
                NodeItemClass & item = *(NodeItemClass *) *iter;
                if (item == mEndSymbol) {
                    key.push_back(item.get());
                    cb((const T *)&key[0], ((EndNodeItemClass&)item).getValue());
                    key.pop_back();
                } else {
                    key.push_back(item.get());
                    item.getChilds()->traverse(key, cb);
                    key.pop_back();
                }
            }
        }
    }

    void startsWith(std::vector<T> & keySoFar, const T * key, std::vector< std::pair<std::vector<T>, V> > & values, int count, int i) {
        
        if (key[i] == mEndSymbol) {
            accumulate(keySoFar, values, count);
            return;
        }

        NodeItemClass * item = mItems->getItem(key[i]);
        if (!item) {
            return;
        } else if (*item == key[i]) {
            keySoFar.push_back(item->get());
            item->getChilds()->startsWith(keySoFar, key, values, count, ++i);
        }
    }

    void accumulate(std::vector<T> key, std::vector< std::pair<std::vector<T>, V> > & values, int count) {
        ItemsContainerIter iterEnd = mItems->end();
        for (ItemsContainerIter iter = mItems->begin(); iter != iterEnd && count > 0; ++iter) {
            if (*iter != 0) {
                NodeItemClass & item = *(NodeItemClass *)*iter;
                if (item == mEndSymbol) {
                    std::vector<T> result;
                    result.assign(key.begin(), key.end());
                    values.push_back(std::make_pair(result, ((EndNodeItemClass&)item).getValue()));
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
        NodeItemClass * itemp = mItems->getItem(key[i]);
        if (!itemp) {
            return 0;
        }
        NodeItemClass & item = *itemp;
        if (key[i] == mEndSymbol && item == mEndSymbol) {
            return &(((EndNodeItemClass&)item).getValue());
        } else if (item == key[i]) {
            return item.getChilds()->get(key, ++i);
        } else {
            return 0;
        }
    }

    bool hasKey(const T * key, int i) {
        NodeItemClass * item = mItems->getItem(key[i]);
        if (!item) {
            return false;
        }

        if (key[i] == mEndSymbol && *item == mEndSymbol) {
            return true;
        } else if (*item == key[i]) {
            return item->getChilds()->hasKey(key, ++i);
        } else {
            return false;
        }
    }

    bool insertData(const T * key, V const & value, int i) {

        NodeItemClass * item = mItems->insertItem(key[i]);

        if (!item) {
            return false;
        }

        if (key[i] == mEndSymbol) {
            ((EndNodeItemClass*)item)->set(key[i], value);  
            return true;
        } else if (*item == key[i]) {
            return item->getChilds()->insertData(key, value, ++i);
        }
        return false;
    }

    bool erase(const T * key, int i, bool & finished) {
        bool erased = false;
        NodeItemClass * item = mItems->getItem(key[i]);
        if (!item) {
            return erased;
        }
        if (key[i] == mEndSymbol && *item == mEndSymbol) {
            int count = 0;
            ItemsContainerIter iterEnd = mItems->end();
            for (ItemsContainerIter iter = mItems->begin(); iter != iterEnd; ++iter) {
                if (*iter != 0) {
                    ++count;
                }
            }
            if (count > 1) {
                mItems->eraseItem(key[i]);
                finished = true;
            }
            erased = true;
        } else {
            erased = item->getChilds()->erase(key, i+1, finished);
            if (erased && !finished) {
                int count = 0;
                ItemsContainerIter iterEnd = mItems->end();
                for (ItemsContainerIter iter = mItems->begin(); iter != iterEnd; ++iter) {
                    if (*iter != 0) {
                        ++count;
                    }
                }
                if (count > 1) {
                    mItems->eraseItem(key[i]);
                    finished = true;
                } else if (count == 1) {
                    mItems->eraseItem(key[i]);
                }

            }
        }
        return erased;
    }

private:
    typedef typename Items::iterator ItemsContainerIter;
    typedef typename Items::const_iterator ItemsContainerConster;

    Items * mItems;
    const T mEndSymbol;
    unsigned int mSize;
};
template<typename T,
         typename V,
         typename Cmp,
         int Max = 256> class VectorItems
{
public:
    typedef NodeItem<T, V, Cmp, VectorItems<T, V, Cmp, Max> > Item;
    typedef std::vector<Item *> Items;
    typedef typename Items::iterator iterator;
    typedef typename Items::const_iterator const_iterator;

public:
    VectorItems(T const & endSymbol, Node<T, V, Cmp, VectorItems<T, V, Cmp, Max> > * node)
        : mEndSymbol(endSymbol),
          mItems(Max, (Item *)0),
          mNode(node)
    {}

    iterator begin() {
        return mItems.begin();
    }

    const_iterator begin() const {
        return mItems.begin();
    }

    iterator end() {
        return mItems.end();
    }

    const_iterator end() const {
        return mItems.end();
    }

    void clear() {
        std::fill(mItems.begin(), mItems.end(), (Item *)0);
    }

    Item * insertItem(T const & k) {
        if (!mItems[k]) {
            mItems[k] = mNode->createNodeItem(k);
            return mItems[k];
        } else {
            if (k != mEndSymbol) {
                return mItems[k];
            }
        }
        return 0;
    }

    bool eraseItem(T const & k) {
        if (mItems[k]) {
            delete mItems[k];
            mItems[k] = (Item *)0;
            return true;
        } else {
            return false;
        }
    }

    Item * getItem(T const & k) {
        return mItems[k];
    }

private:
    Items mItems;
    T mEndSymbol;
    Node<T, V, Cmp, VectorItems<T, V, Cmp, Max> > * mNode;
};

template<typename T,
         typename V,
         typename Cmp> class SetItems
{
public:
    typedef NodeItem<T, V, Cmp, SetItems<T, V, Cmp> > Item;
    typedef std::set<Item *, NodeItemPtrCompare<T, V, Cmp, SetItems<T, V, Cmp> > > Items;
    typedef typename Items::iterator iterator;
    typedef typename Items::const_iterator const_iterator;

public:
    SetItems(T const & endSymbol, Node<T, V, Cmp, SetItems<T, V, Cmp> > * node)
        : mEndSymbol(endSymbol),
          mNode(node)
    {}

    iterator begin() {
        return mItems.begin();
    }
    
    const_iterator begin() const {
        return mItems.begin();
    }

    iterator end() {
        return mItems.end();
    }

    const_iterator end() const {
        return mItems.end();
    }

    void clear() {
        mItems.clear();
    }

    Item * insertItem(T const & k) {
        Item tmp(mEndSymbol, k);
        iterator iter = mItems.find(&tmp);
        if (iter == mItems.end()) {
            Item * v = mNode->createNodeItem(k);
            mItems.insert(v);
            return v;
        } else {
            if (k != mEndSymbol) {
                return (Item*) *iter;
            }
        }
        return 0;
    }

    bool eraseItem(T const & k) {
        Item tmp(mEndSymbol, k);
        iterator iter = mItems.find(&tmp);
        if (iter != mItems.end()) {
            mItems.erase(iter);
            return true;
        } else {
            return false;
        }
    }

    Item * getItem(T const & k) {
        Item tmp(mEndSymbol, k);

        iterator iter = mItems.find(&tmp);
        if (iter == mItems.end()) {
            return 0;
        }
        return (Item *) (*iter);
    }

private:
    Items mItems;
    T mEndSymbol;
    Node<T, V, Cmp, SetItems<T, V, Cmp> > * mNode;
};

/*!
 * @brief Trie implementation class
 * @tparam T Type for each element in the key
 * @tparam V Type of the value that the key will be representing
 * @tparam Cmp Comparison functor
 * @tparam Items The data structure that represents each node in the Trie. 
 *               Items can be rtv::SetItems<T, V, Cmp> or rtv::VectorItems<T, V, Cmp, Max>,
 *               Max is the integer representing the size of each node.
 *
 * @section usage_sec Usage of the Trie
 * @subsection usage_declaration Declarating the Trie
 * A Trie with key as chars and value as std::string can be declared as given below
 * @code
 * #include <string>
 * #include <trie.h>
 *
 * int main(int argc, char ** argv) {
 *
 *     rtv::Trie<char, std::string> dictionary('$');
 *
 *     return 0
 * }
 * @endcode
 *
 * @subsection usage_population Populatiion and deletion from the Trie
 * Trie can be populated by using the Trie::insert method and element can be removed using Trie::erase.
 * @code
 * #include <trie.h>
 * #include <string>
 * #include <iostream>
 *
 * int main(int argc, char ** argv) {
 *
 *     rtv::Trie<char, std::string> dictionary('$');
 *     
 *     // adding key value pair to the Trie
 *     if (dictionary.insert("karma$", "Destiny or fate, following as effect from cause")) {
 *         std::cout << "added karma" << std::endl;
 *     }
 *
 *     // removing key from the Trie
 *     if (dictionary.erase("karma$")) {
 *         std::cout << "removed karma" << std::endl;
 *     }
 *
 *     return 0
 * }
 * @endcode
 * @subsection usage_retrieval Retrieval of Value
 * Value for a key can be retrieved using Trie::get method and
 * the existence of the key can be tested using Trie::hasKey method.
 * @code
 * #include <trie.h>
 * #include <string>
 * #include <iostream>
 *
 * int main(int argc, char ** argv) {
 *
 *     rtv::Trie<char, std::string> dictionary('$');
 *     
 *     dictionary.insert("karma$", "Destiny or fate, following as effect from cause");
 *
 *     if (dictionary.hasKey("karma$")) {
 *         std::cout << "key karma found" << std::endl;
 *     }
 *     std::string * result = dictionary.get("karma$");
 *     if (result) {
 *         std::cout << result->c_str() << std::endl;
 *     }
 *     return 0
 * }
 * @endcode
 *
 * @subsection usage_searching Searching keys which begins with the given charactars
 * Keys which begins with a specific charactars can be retrieved using Trie::startsWith method
 * @code
 * #include <trie.h>
 * #include <string>
 * #include <iostream>
 * #include <vector>
 *
 * int main(int argc, char ** argv) {
 *
 *     rtv::Trie<char, std::string> dictionary('$');
 *     
 *     dictionary.insert("karma$", "Destiny or fate, following as effect from cause");
 *
 *     typedef std::vector< std::pair < std::vector<char> , std::string > > TrieResult;
 *     TrieResult result;
 *
 *     dictionary.startsWith("kar$", result);
 *
 *     for (TrieResult::iterator iter = result.begin(); iter != result.end(); ++iter) {
 *         iter->first.push_back(0);
 *         std::cout << &((iter->first)[0]) << " : " << iter->second.c_str() << std::endl;
 *     }
 *
 *     return 0
 * }
 * @endcode
 *
 * @subsection usage_array_of_node Trie with each Node as an array
 * Here each node of the Trie is an array. The advantage is that the searching of a symbol in the array takes O(1) time (is constant time).
 * The disadvantage is that the array will have empty elements so the space used will more than actually required.
 *
 * @code
 *
 * #include <trie.h>
 * #include <string>
 * #include <iostream>
 * #include <vector>
 *
 * int main(int argc, char ** argv) {
 *
 *     // Here 256 is the size of array in each node
 *     rtv::Trie<char, std::string, std::less<char>, rtv::VectorItems<char, std::string, std::less<char>, 256> > dictionary('$');
 *
 *     return 0
 * }
 * @endcode
 *
 * @subsection usage_set_of_node Trie with each Node as a set
 * Here each node will be an ordered set.
 * Here there will be no extra usage of space but search for a symbol in the node takes logarithmic time.
 * Trie with this feature can also be used for caseinsensitive symbol handling.
 * @code
 *
 * #include <trie.h>
 * #include <string>
 * #include <iostream>
 * #include <set>
 *
 * int main(int argc, char ** argv) {
 *
 *     rtv::Trie<char, std::string, std::less<char>, rtv::SetItems<char, std::string, std::less<char> > > dictionary('$');
 *
 *     return 0
 * }
 * @endcode
 */
template<typename T,
         typename V,
         typename Cmp=std::less<T>,
         typename Items=SetItems<T, V, Cmp> > class Trie
{
public:
    /*!
     * @param endSymbol The symbol which marks the end of key input
     */
    Trie(const T & endSymbol)
        : mRoot(endSymbol)
    {}

    /*!
     * Add a key with value in to the Trie
     * @param key Key which should be inserted, should be terminated by 'end' symbol
     * @param value The value that is to be set with the key
     * @return true if the given key is inserted in to the Trie, false otherwise
     */
    bool insert(const T * key, V const & value) {
        return mRoot.insert(key, value);
    }

    /*!
     * Remove the entry with the given key from the Trie
     * @param key Key which should be erased, should be terminated by 'end' symbol
     * @return true if the given key is erased in to the Trie, false otherwise
     */
    bool erase(const T * key) {
        return mRoot.erase(key);
    }

    /*!
     * Retrieves the value for the given key
     * @param key Key to be searched for, should be terminated by 'end' symbol
     * @return Pointer to value for the given key, 0 on failure
     */
    const V * get(const T * key) {
        return mRoot.get(key);
    }

    /*!
     * Checks whether the given key is present in the Trie
     * @param key Key to be searched for, should be terminated by 'end' symol
     * @return true if the key is present
     */
    bool hasKey(const T * key) {
        return mRoot.hasKey(key);
    }

    /*!
     * Test whether Trie is empty
     * @return true if the Trie size is 0, false otherwise
     */
    bool empty() const {
        return mRoot.empty();
    }

    /*!
     * Returns the number of elements in the Trie
     * @return Number of key value pair in the Trie
     */
    unsigned int size() const {
        return mRoot.size();
    }

    /*!
     * All the elements in the Trie are dropped, leaving the Trie with a size of 0.
     */
    void clear() {
        mRoot.clear();
    }
    /*!
     * Retrieves all the key value pair which starts with the given key
     * @param key Part of the key which should be searched, should be terminated by 'end' symbol
     * @param values Vector of key, value pair
     * @param count Maximum number of entries that should be returned
     */
    void startsWith(const T * key, std::vector< std::pair<std::vector<T>, V> > & values, int count = 5) {
        mRoot.startsWith(key, values, count);
    }

    /*!
     * Traverse through the Trie
     * @param c The functor class which will be called when a node is reached. Functor takes two arguments, 
     *          first argument is const T * and second V(or const V &)
     */
    template <typename CB>
    void traverse(CB const & c) {
        mRoot.traverse(c);
    }

private:
    Trie(Trie const &);
    Trie & operator=(Trie const &);

private:
    Node<T, V, Cmp, Items> mRoot;
};

}

#endif
