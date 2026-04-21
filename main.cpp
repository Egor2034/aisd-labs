#include <iostream>
#include <windows.h>
#include "table.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    HashTable<int> table(8);
    table.insert(4, 13);
    table.insert(5, 11);
    table.insert(6, 113);
    table.insert(7, 1);
    table.insert(8, 6);
    table.insert(9, 3);
    table.insert(1, 3135);
    table.insert(13, 351);
    table.insert(141, 1113);

    *table.search(4) = 124; 
    table.print();

    std::cout << "\n\n";
    HashTable<int> t1 = table;
    t1.print();
    std::cout << "\n";

    return 0;
}