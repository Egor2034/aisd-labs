#ifndef TABLE_H
#define TABLE_H

#include <iostream>

/*
Конструктор пустой хэш таблицы заданного размера +
Конструктор копирования; +
bool insert(int key, const T &value) – вставка значения по ключу; +
void insert_or_assign(int key, T &value) - вставка или присвоение значения по ключу. +
void print() – печать содержимого; +

Конструктор, заполняющий хэш таблицу случайными значениями согласно вашему заданию.
Деструктор;
Оператор присваивания;
bool contains(T &value) -  проверка наличия элемента;
T* search(int key) - поиск элемента;
bool erase(int key) – удаление элемента по значению;
int count(int key) - возвращает количество элементов, у которых значение хэш-функции совпадает с переданным.

*/

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
    size_t _count;

    const size_t A = 11400714819323198485;
    const size_t W = 64;
    size_t _l;

    size_t hash(int key) {
        return (key * A) >> (W - _l);
    }

public:
    HashTable(size_t size) : _count(0) {
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
        _buckets = new ListNode<T>*[other.get_size()]();
        _count = other.get_count();
        _size = other.get_size();
        _l = other._l;

        if (_count == 0) { return; }

        for (size_t i = 0; i < _size; i++) {
            ListNode<T>* current = other._buckets[i];
            ListNode<T>* tail;

            while (current != nullptr) {
                if (_buckets[i] == nullptr) {
                    _buckets[i] = new ListNode(current->key, current->value, nullptr);
                    tail = _buckets[i];
                }
                else {
                    tail->next = new ListNode(current->key, current->value, nullptr);
                    tail = tail->next;
                }
                current = current->next;
            }
        }
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

        ListNode<T>* to_add = new ListNode(key, value, _buckets[index]);
        _buckets[index] = to_add;
        _count++;

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

        ListNode<T>* to_add = new ListNode(key, value, _buckets[index]);
        _buckets[index] = to_add;
        _count++;
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
            whule (current != nullptr) {
                if (current->value == value) {
                    return true;
                }
                current = current->next;
            }
        }

        return false;
    }
    size_t get_size() const {
        return _size;
    }

    size_t get_count() const {
        return _count;
    }
};

#endif