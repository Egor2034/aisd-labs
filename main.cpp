#include <iostream>
#include <windows.h>
#include "tree.hpp"
#include "tests.cpp"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    tests();
    
    std::vector<int> vect { 2, 2, 3, 4, 5, 5 };
    std::vector<int> uniq = uniq_elements(vect);

    for (int i = 0; i < uniq.size(); i++) {
        std::cout << uniq[i] << " ";
    }

    std::cout << "\n";

    return 0;
}