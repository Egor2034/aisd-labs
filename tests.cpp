#include <iostream>
#include <chrono>
#include <vector>
#include "tree.hpp"

void tests() {
    std::vector<int> sizes { 1000, 10000, 100000 };
    
    for (int size : sizes) {
        std::cout << "Для массива размером " << size << ":\n";
        std::vector<int> rand_vect;

        for (int i = 0; i < size; i++) {
            rand_vect.push_back((int)lcg());
        }

        double total_tree_insert = 0;
        double total_vec_insert = 0;
        int insert_attempts = 100;

        for (int k = 0; k < insert_attempts; ++k) {
            auto start = std::chrono::high_resolution_clock::now();
            AvlTree tree;

            for (int value : rand_vect) {
                tree.insert(value);
            }

            auto end = std::chrono::high_resolution_clock::now();
            total_tree_insert += std::chrono::duration<double, std::milli>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            std::vector<int> vec;

            for (int value : rand_vect) {
                vec.push_back(value);
            }

            end = std::chrono::high_resolution_clock::now();
            total_vec_insert += std::chrono::duration<double, std::milli>(end - start).count();
        }

        std::cout << "Среднее время заполнения дерева: " << (total_tree_insert / insert_attempts) << " мс" << "\n";
        std::cout << "Среднее время заполнения вектора:  " << (total_vec_insert / insert_attempts) << " мс" << "\n";

        AvlTree tree;
        std::vector<int> vect;

        for (int value : rand_vect) {
            tree.insert(value);
            vect.push_back(value);
        }

        double total_tree_search = 0;
        double total_vec_search = 0;
        int search_attempts = 1000;

        for (int k = 0; k < search_attempts; k++) {
            int rand_num = (int)lcg();

            auto start = std::chrono::high_resolution_clock::now();
            tree.contains(rand_num);
            auto end = std::chrono::high_resolution_clock::now();
            total_tree_search += std::chrono::duration<double, std::milli>(end - start).count();
            
            start = std::chrono::high_resolution_clock::now();
            for (int value: rand_vect) {
                if (value == rand_num) {
                    break;
                }
            }
            end = std::chrono::high_resolution_clock::now();
            total_vec_search += std::chrono::duration<double, std::milli>(end - start).count();
        }

        std::cout << "Среднее время поиска в дереве: " << (total_tree_search / search_attempts) << " мс" << "\n";
        std::cout << "Среднее время поиска в массиве:  " << (total_vec_search / search_attempts) << " мс" << "\n";

        double total_tree_erase = 0;
        double total_vec_erase = 0;
        int erase_attempts = 1000;

        for (int k = 0; k < erase_attempts; k++) {
            int rand_num = (int)lcg();
            
            auto start = std::chrono::high_resolution_clock::now();
            tree.erase(rand_num);
            auto end = std::chrono::high_resolution_clock::now();
            total_tree_erase += std::chrono::duration<double, std::milli>(end - start).count();

            start = std::chrono::high_resolution_clock::now();
            for (auto it = vect.begin(); it != vect.end(); it++) {
                if (*it == rand_num) {
                    vect.erase(it);
                    break;
                }
            }
            end = std::chrono::high_resolution_clock::now();
            total_vec_erase += std::chrono::duration<double, std::milli>(end - start).count();
        }

        std::cout << "Среднее время удаления из дерева:  " << (total_tree_erase / erase_attempts) << " мс" << "\n";
        std::cout << "Среднее время удаления из вектора:   " << (total_vec_erase / erase_attempts) << " мс" << "\n";
        
        std::cout << "\n";
    }
}