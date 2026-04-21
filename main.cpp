#include <iostream>
#include <windows.h>
#include "table.hpp"
#include "task.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    HashTable<int> roman_map(16);

    roman_map.insert('I', 1);
    roman_map.insert('V', 5);
    roman_map.insert('X', 10);
    roman_map.insert('L', 50);
    roman_map.insert('C', 100);
    roman_map.insert('D', 500);
    roman_map.insert('M', 1000);

    std::string str;
    std::cin >> str;

    int dec = rom_to_dec(str, roman_map);
    std::cout << dec;
    
    return 0;
}