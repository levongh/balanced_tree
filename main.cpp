#include "balanced_tree.h"

#include <iostream>
#include <string>

int main()
{
    std::balanced_tree<int> empty;
    std::balanced_tree<int> filled = {1, 2, 3, 4};

    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }
    
    std::cout << "Printing the filled one with size " << filled.size() << "\n";
    for (const auto& val : filled) {
        std::cout << val << std::endl;
    }

    empty.insert(345);
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }

    empty.insert({10, 20, 948, 1234, 4, 10, 23});
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }

    empty.erase(10);
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }

    empty.erase(21);
    std::cout << "Printing the empty one with size " << empty.size() << "\n";
    for (const auto& val : empty) {
        std::cout << val << std::endl;
    }
}

