#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <stdexcept>

using std::vector;
using std::list;
using std::pair;
using std::cout;

template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
private:
    vector<list<typename list<pair<const KeyType, ValueType>>::iterator>> table;
    list<pair<const KeyType, ValueType>> elements;
    Hash hash;
    size_t tableSize = 0;

public:
    HashMap(Hash hash = Hash()): hash(hash) {
        tableSize = 0;
        table = vector<list<typename list<pair<const KeyType, ValueType>>::iterator>>(1);
    }

    template<typename Iter>
    HashMap(Iter begin, Iter end, Hash hash = Hash()): hash(hash) {
        tableSize = 0;
        table = vector<list<typename list<pair<const KeyType, ValueType>>::iterator>>(1);
        while (begin != end) {
            insert(*begin);
            ++begin;
        }
    }

    HashMap(const std::initializer_list<pair<const KeyType, ValueType>>& elements, Hash hash = Hash())
            :  HashMap(elements.begin(), elements.end(), hash) {}

    size_t size() const {
        return tableSize;
    }

    bool empty() const {
        return !size();
    }

    Hash hash_function() const {
        return hash;
    }

    void insert(const pair<const KeyType, ValueType>& elem) {
        size_t pos = hash(elem.first) % table.size();
        bool exist = false;
        for (const auto& p : table[pos]) {
            if (p->first == elem.first) exist = true;
        }
        if (!exist) {
            elements.emplace_front(elem); // в начало для того, чтобы брать указатель как begin()
            table[pos].push_back(elements.begin());
            ++tableSize;
        }

        if (3 * tableSize > 2 * table.size()) {
            table = vector<list<typename list<pair<const KeyType, ValueType>>::iterator>>(2 * table.size() + 1);
            for (auto it = elements.begin(); it != elements.end(); it++) {
                size_t pos = hash(it->first) % table.size();
                table[pos].push_back(it);
            }
        }

    }

    void erase(const KeyType& key) {
        size_t pos = hash(key) % table.size();
        for (auto it = table[pos].begin(); it != table[pos].end(); ++it) {
            if ((*it)->first == key) {
                elements.erase(*it);
                table[pos].erase(it);
                --tableSize;
                break;
            }
        }
    }

    using iterator = typename list<pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename list<pair<const KeyType, ValueType>>::const_iterator;

    iterator begin() {
        return elements.begin();
    }

    iterator end() {
        return elements.end();
    }

    const_iterator begin() const {
        return elements.begin();
    }

    const_iterator end() const {
        return elements.end();
    }

    ValueType& operator[](const KeyType& key) {
        insert({key, ValueType()});
        return find(key)->second;
    }

    const ValueType& at(const KeyType& key) const {
        if (this->find(key) == elements.end()) {
            throw std::out_of_range("");
        }
        size_t pos = hash(key) % table.size();
        for (auto it : table[pos]) {
            if (it->first == key) {
                return it->second;
            }
        }
    }

    HashMap<KeyType, ValueType, Hash>& operator=(const HashMap<KeyType, ValueType, Hash>& other) {
        this->table = other.table;
        this->hash = other.hash;
        this->elements.clear();
        this->tableSize = other.tableSize;
        for (const auto& it : other.elements) {
            this->elements.emplace_back(it);
        }
        return *this;
    }

    void clear() {
        tableSize = 0;
        elements.clear();
        table = vector<list<typename list<pair<const KeyType, ValueType>>::iterator>>(1);
    }

    iterator find(const KeyType &key) {
        size_t pos = hash(key) % table.size();
        for (const auto& it : table[pos]) {
            if (it->first == key) {
                return it;
            }
        }
        return elements.end();
    }

    const_iterator find(const KeyType &key) const {
        size_t pos = hash(key) % table.size();
        for (const auto& it : table[pos]) {
            if (it->first == key) {
                return it;
            }
        }
        return elements.end();
    }
};
