#ifndef TABLE_H
#define TABLE_H

#include <iostream>

template <typename T>
struct ListNode {
        T value;
        int key;

        ListNode* next;
        ListNode(int k, T v, ListNode* n = nullptr) : key(k), value(v), next(n) {}
    };

template <typename T>
class HashTable {
private:
    ListNode<T>** _buckets;
    size_t _size;

    const size_t A = 11400714819323198485;
    const size_t W = 64;
    size_t _l;

    size_t hash(int key) const {
        return (key * A) >> (W - _l);
    }

    ListNode<T>** copy() const {
        ListNode<T>** new_buckets = new ListNode<T>*[_size]();

        for (size_t i = 0; i < _size; i++) {
            ListNode<T>* current = _buckets[i];
            ListNode<T>* tail;

            while (current != nullptr) {
                if (new_buckets[i] == nullptr) {
                    new_buckets[i] = new ListNode<T>(current->key, current->value, nullptr);
                    tail = new_buckets[i];
                }
                else {
                    tail->next = new ListNode<T>(current->key, current->value, nullptr);
                    tail = tail->next;
                }
                current = current->next;
            }
        }

        return new_buckets;
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

            _buckets = new ListNode<T>*[_size]();
        }
    }

    HashTable(const HashTable<T>& other) {
        _buckets = other.copy();
        _size = other.get_size();
        _l = other._l;
    }
    
    ~HashTable() {
        clear();
        delete[] _buckets;
    }

    HashTable& operator=(const HashTable<T>& other) {
        if (this == &other) { return *this; }

        clear();
        delete[] _buckets;

        _buckets = other.copy();
        _size = other.get_size();
        _l = other._l;

        return *this;
    }
    
    bool insert(int key, const T &value) {
        size_t index = hash(key);

        ListNode<T>* current = _buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return false;
            }
            current = current->next;
        }

        ListNode<T>* to_add = new ListNode<T>(key, value, _buckets[index]);
        _buckets[index] = to_add;

        return true;
    }

    void insert_or_assign(int key, T &value) {
        size_t index = hash(key);

        ListNode<T>* current = _buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        ListNode<T>* to_add = new ListNode<T>(key, value, _buckets[index]);
        _buckets[index] = to_add;
    }

    bool erase(int key) {
        size_t index = hash(key);

        if (_buckets[index] == nullptr) { return false; }
        
        if (_buckets[index]->key == key) {
            ListNode<T>* to_del = _buckets[index];
            _buckets[index] = _buckets[index]->next;
            delete to_del;
            return true;
        }

        
        ListNode<T>* prev = _buckets[index];
        ListNode<T>* current = prev->next;
        
        while (current != nullptr) {
            if (current->key == key) {
                ListNode<T>* to_del = current;
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
            ListNode<T>* current = _buckets[i];
            while (current != nullptr) {
                std::cout << "{key: " << current->key << " , value: " << current->value << "} ";
                current = current->next;
            }

            std::cout << "\n";
        }
    }

    bool contains(const T& value) const {
        for (size_t i = 0; i < _size; i++) {
            ListNode<T>* current = _buckets[i];
            while (current != nullptr) {
                if (current->value == value) {
                    return true;
                }
                current = current->next;
            }
        }

        return false;
    }
    
    T* search(int key) {
        size_t index = hash(key);

        ListNode<T>* current = _buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }

        throw "The table does not contain an element for the given key!"; 
    }
    
    const T* search(int key) const {
        size_t index = hash(key);

        ListNode<T>* current = _buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return &(current->value);
            }
            current = current->next;
        }

        throw "The table does not contain an element for the given key!"; 
    }

    size_t count(int key) const {
        size_t index = hash(key);
        size_t count = 0;

        ListNode<T>* current = _buckets[index];
        while (current != nullptr) { 
            count++;
            current = current->next;
        }

        return count;
    }

    void clear() {
        if (_buckets == nullptr) return;

        for (size_t i = 0; i < _size; i++) {
            ListNode<T>* current = _buckets[i];
            while (current != nullptr) {
                ListNode<T>* to_del = current;
                current = current->next;
                delete to_del;
            }

            _buckets[i] = nullptr;
        }
    }

    size_t get_size() const { return _size; }
};

#endif