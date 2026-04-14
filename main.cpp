#include <iostream>
#include "tree.hpp"

int main() {
    std::vector<int> vect { 3, 2, 2, 4, 2, 3 };
    std::vector<int> result = uniq_elements(vect);

    for (int i = 0; i < result.size(); i++) {
        std::cout << result[i] << " ";
    }

    return 0;
}