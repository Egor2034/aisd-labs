#ifndef TASK_H
#define TASK_H

#include <string>
#include "table.hpp"

int rom_hash_func(char symb, const HashTable<int>& table) {
    int key = (int)symb;
    try {
        return *(table.search(key));
    } catch (const char* err) {
        throw "Uncorrect enter";
    }
}

int rom_to_dec(const std::string& rom, HashTable<int>& table) {
    int result = 0;
    int prev_value = 0;

    for (size_t i = 0; i < rom.length(); i++) {
        int value = rom_hash_func(rom[i], table);  

        result += value;

        if (prev_value < value) {
            result -= 2 * prev_value;
        }

        prev_value = value;
    }

    return result;
}

#endif