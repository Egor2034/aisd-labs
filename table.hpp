#ifndef TABLE_H
#define TABLE_H

#include <iostream>

template <typename K, typename T>
struct ListNode {
        T value;
        K key;

        ListNode* next;
        ListNode(K k, T v, ListNode* n = nullptr) : key(k), value(v), next(n) {}
    };

template <typename K, typename T>
class HashTable {
private:
    ListNode<K, T>** _buckets;
    size_t _size;

    const size_t A = 11400714819323198485;
    const size_t W = 64;
    size_t _l;

    size_t hash(K key) const {
        return (key * A) >> (W - _l);
    }

    size_t hash(std::string key) const {
        return (key.length() * A) >> (W - _l);
    }

    ListNode<K, T>** copy() const {
        ListNode<K, T>** new_buckets = new ListNode<K, T>*[_size]();

        for (size_t i = 0; i < _size; i++) {
            ListNode<K, T>* current = _buckets[i];
            ListNode<K, T>* tail;

            while (current != nullptr) {
                if (new_buckets[i] == nullptr) {
                    new_buckets[i] = new ListNode<K, T>(current->key, current->value, nullptr);
                    tail = new_buckets[i];
                }
                else {
                    tail->next = new ListNode<K, T>(current->key, current->value, nullptr);
                    tail = tail->next;
                }
                current = current->next;
            }
        }

        return new_buckets;
    }

    int lcg() {
        static int x = 0;
        x = (1021*x+24631) % 116640;
        return x;
    }

public:
    HashTable(size_t size) {
        if (size == 0) {
            _size = 0;
            _l = 0;
            _buckets = nullptr;
        }
        else {
            _size = 1;
            _l = 0;

            while (_size < size) {
                _size <<= 1;
                _l++;
            }

            _buckets = new ListNode<K, T>*[_size]();
        }
    }

    HashTable(const HashTable<K, T>& other) {
        _buckets = other.copy();
        _size = other.get_size();
        _l = other._l;
    }

    ~HashTable() {
        clear();
        delete[] _buckets;
    }

    HashTable& operator=(const HashTable<K, T>& other) {
        if (this == &other) { return *this; }

        clear();
        delete[] _buckets;

        _buckets = other.copy();
        _size = other.get_size();
        _l = other._l;

        return *this;
    }
    
    bool insert(K key, const T &value) {
        size_t index = hash(key);

        ListNode<K, T>* current = _buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return false;
            }
            current = current->next;
        }

        ListNode<K, T>* to_add = new ListNode<K, T>(key, value, _buckets[index]);
        _buckets[index] = to_add;

        return true;
    }

    void insert_or_assign(K key, const T &value) {
        size_t index = hash(key);

        ListNode<K, T>* current = _buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        ListNode<K, T>* to_add = new ListNode<K, T>(key, value, _buckets[index]);
        _buckets[index] = to_add;
    }

    bool erase(K key) {
        size_t index = hash(key);

        if (_buckets[index] == nullptr) { return false; }
        
        if (_buckets[index]->key == key) {
            ListNode<K, T>* to_del = _buckets[index];
            _buckets[index] = _buckets[index]->next;
            delete to_del;
            return true;
        }

        ListNode<K, T>* prev = _buckets[index];
        ListNode<K, T>* current = prev->next;
        
        while (current != nullptr) {
            if (current->key == key) {
                ListNode<K, T>* to_del = current;
                prev->next = current->next;
                delete to_del;
                return true;
            }

            prev = current;
            current = current->next;
        }

        return false;
    }

    void print() const {
        for (size_t i = 0; i < _size; i++) {
            ListNode<K, T>* current = _buckets[i];
            while (current != nullptr) {
                std::cout << "{key: " << current->key << " , value: " << current->value << "} ";
                current = current->next;
            }

            std::cout << "\n";
        }
    }

    bool contains(const T& value) const {
        for (size_t i = 0; i < _size; i++) {
            ListNode<K, T>* current = _buckets[i];
            while (current != nullptr) {
                if (current->value == value) {
                    return true;
                }
                current = current->next;
            }
        }

        return false;
    }
    
    T* search(K key) {
        size_t index = hash(key);

        ListNode<K, T>* current = _buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }

        throw "The table does not contain an element for the given key!"; 
    }
    
    const T* search(K key) const {
        size_t index = hash(key);

        ListNode<K, T>* current = _buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }

        throw "The table does not contain an element for the given key!"; 
    }

    size_t count(K key) const {
        size_t index = hash(key);
        size_t count = 0;

        ListNode<K, T>* current = _buckets[index];
        while (current != nullptr) { 
            count++;
            current = current->next;
        }

        return count;
    }

    void clear() {
        if (_buckets == nullptr) return;

        for (size_t i = 0; i < _size; i++) {
            ListNode<K, T>* current = _buckets[i];
            while (current != nullptr) {
                ListNode<K, T>* to_del = current;
                current = current->next;
                delete to_del;
            }

            _buckets[i] = nullptr;
        }
    }

    size_t get_size() const { return _size; }
};

#endif