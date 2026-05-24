#include "graph.hpp"
#include <iostream>
#include <string>
#include <windows.h>
#include <vector>

std::string find_optimal_warehouse(Graph<std::string, double>& g) {
    std::string best_vertex;
    double min_max_dist = std::numeric_limits<double>::infinity();

    for (const auto& v : g.vertices()) {
        double current_max = 0.0;
        bool all_reachable = true;

        for (const auto& target : g.vertices()) {
            if (v == target) continue;
            auto path = g.shortest_path(v, target);
            if (path.empty()) {
                all_reachable = false;
                break;
            }
            double dist = 0.0;
            for (const auto& e : path) dist += e.weight;
            if (dist > current_max) current_max = dist;
        }

        if (all_reachable && current_max < min_max_dist) {
            min_max_dist = current_max;
            best_vertex = v;
        }
    }
    return best_vertex;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Graph<std::string, double> g;

    g.add_vertex("A"); 
    g.add_vertex("B");
    g.add_vertex("C"); 
    g.add_vertex("D"); 
    g.add_vertex("E");

    g.add_edge("A", "B", 4.0); g.add_edge("A", "C", 2.0);
    g.add_edge("B", "C", 1.0); g.add_edge("B", "D", 5.0);
    g.add_edge("C", "D", 8.0); g.add_edge("C", "E", 10.0);
    g.add_edge("D", "E", 2.0);  g.add_edge("E", "A", 3.0);
    g.add_edge("D", "A", 6.0);

    g.print();

    std::cout << "\nОбход в глубину, начиная с вершины A\n";
    std::vector<std::string> visited_order;
    g.walk("A", [&visited_order](const std::string& v) {
        std::cout << "Вершина: " << v << "\n";
        visited_order.push_back(v);
    });

    std::cout << "\nПоиск кратчайшего пути (A -> E)\n";
    auto path = g.shortest_path("A", "E");

    if (path.empty()) {
        std::cout << "Путь не найден!\n";
    } 
    else {
        double total = 0.0;
        for (const auto& e : path) {
            std::cout << "  " << e.from << " --[" << e.weight << "]--> " << e.to << "\n";
            total += e.weight;
        }
        std::cout << "Расстояние: " << total << "\n";
    }

    std::cout << "\n" << (g.is_connected() ? "Граф сильно связный" : "Граф не является сильно связным") << "\n";

    std::cout << "\nЗадание: Поиск оптимальной точки для склада\n";
    std::string optimal = find_optimal_warehouse(g);
    std::cout << "Оптимальная точка: " + optimal << "\n";

}