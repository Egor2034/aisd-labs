// main.cpp
#include "graph.hpp"
#include <iostream>

int main() {
    try {
        Graph<std::string, double> g;
        g.add_vertex("A"); g.add_vertex("B"); 
        g.add_vertex("C"); g.add_vertex("D");

        g.add_edge("A", "B", 4.0);
        g.add_edge("A", "C", 2.0);
        g.add_edge("B", "C", 1.0);
        g.add_edge("B", "D", 5.0);
        g.add_edge("C", "D", 8.0);
        g.add_edge("D", "A", 3.0);

        g.print();

        std::cout << "\nDFS walk from A:\n";
        g.walk("A", [](const std::string& v) { 
            std::cout << "  Visited: " << v << "\n"; 
        });

        std::cout << "\nShortest path A->D:\n";
        auto path = g.shortest_path("A", "D");
        for (const auto& e : path) {
            std::cout << "  " << e.from << " --[" << e.weight << "]--> " << e.to << "\n";
        }

        std::cout << "\nIs strongly connected? " 
                  << (g.is_connected() ? "Yes" : "No") << "\n";

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}