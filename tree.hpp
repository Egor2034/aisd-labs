#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include <iostream>

/*
конструктор копирования +;
деструктор +;
оператор присваивания +;
void print() – печать содержимого;
bool insert(int key) – вставка элемента +;
bool contains(int key) -  проверка наличия элемента +;
bool erase(int key) – удаление элемента;
bool strictly_balanced() - проверка, является ли дерево строго сбалансированным.
а также вспомогательные функции для балансировки дерева.
*/

size_t lcg(){
    static size_t x = 0;
    x = (1021*x+24631) % 116640;
    return x;
}

class AvlTree {
private:
    struct Node {
        int key, height;
        Node* left, *right;
        
        Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    int get_height(Node* node) const {
        if (node == nullptr) { return 0; }
        return node->height;
    }

    void update_height(Node* node) {
        if (node == nullptr) { return; }

        int lh = get_height(node->left);
        int rh = get_height(node->right);

        if (lh > rh) {
            node->height = 1 + lh;
        }
        else {
            node->height = 1 + rh;
        }
    }

    int get_bal_fact(Node* node) const {
        if (node == nullptr) { return 0; }
        return get_height(node->left) - get_height(node->right);
    }

    Node* rotate_left(Node* node) {
        Node* right_node = node->right;
        Node* rl_node = right_node->left;

        right_node->left = node;
        node->right = rl_node;

        update_height(node);
        update_height(right_node);

        return right_node;
    }
    
    Node* rotate_right(Node* node) {
        Node* left_node = node->left;
        Node* rl_node = left_node->right;

        left_node->right = node;
        node->left = rl_node;

        update_height(node);
        update_height(left_node);

        return left_node;
    }

    Node* balance(Node* node) {
        if (node == nullptr) { return nullptr; }

        update_height(node);
        int balance_factor = get_bal_fact(node);

        if (balance_factor > 1) {
            if (get_bal_fact(node->left) < 0) {
                node->left = rotate_left(node->left);
            }
            node = rotate_right(node);
        }

        else if (balance_factor < -1) {
            if (get_bal_fact(node->right) > 0) {
                node->right = rotate_right(node->right);
            }
            node = rotate_left(node);
        }

        return node;
    }

    Node* insert_rec(Node* node, int key, bool& inserted) {
        if (node == nullptr) { 
            inserted = true;
            return new Node(key);
        }

        if (key < node->key) {
            node->left = insert_rec(node->left, key, inserted);
        }
        else if (key > node->key) {
            node->right = insert_rec(node->right, key, inserted);
        }
        else {
            inserted = false;
            return node;
        }

        return balance(node);
    }

    Node* copy_node(Node* node) {
        if (node == nullptr) { return nullptr; }

        Node* copy = new Node(node->key);
        copy->right = copy_node(node->right);
        copy->left = copy_node(node->left);
        copy->height = node->height;

        return copy;
    }

    void clear(Node* node) {
        if (root == nullptr) { return; }

        clear(node->right);
        clear(node->left);
        delete node;
    }

public:
    AvlTree() : root(nullptr) {}

    AvlTree(const AvlTree& other) {
        root = copy_node(other.root);
    }

    ~AvlTree() { clear(root); }

    AvlTree& operator=(const AvlTree& other) {
        if (this == &other) {
            return *this;
        }

        clear(root);
        root = copy_node(other.root);
        return *this;
    }

    bool insert(int key) {
        bool inserted = false;
        insert_rec(root, key, inserted);
        return inserted;
    }

    bool contains(int key) {
        Node* current = root;

        while (current != root) {
            if (key > current->key) {
                current = current->right;
            }
            else if (key < current->key) {
                current = current->left;
            }
            else {
                return true;
            }
        }

        return false;
    }
};

#endif