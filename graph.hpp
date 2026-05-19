#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>    
#include <functional>
#include <list>
#include <vector>
#include <unordered_map>

template<typename Vertex, typename Distance = double>
class Graph{
private:
    std::unordered_map<Vertex, size_t> _indices;
    std::vector<Vertex> _vertices;
    std::vector<std::list<Distance>> _matrix;
    size_t _vert_count;

    static constexpr Distance NO_EDGE = std::numeric_limits<Distance>::infinity();

    size_t get_index(const Vertex& vert) const {
        auto it = _indices.find(vert);
        if (it == _indices.end()) {
            throw std::runtime_error("Vertex not found!");
        }

        return it->second;
    }
    
    Distance& at(std::list<Distance>& l, size_t index) {
        auto it = l.begin();
        std::advance(it, index);
        return *it;
    }
    
    const Distance& at(const std::list<Distance>& l, size_t index) const {
        auto it = l.cbegin();
        std::advance(it, index);
        return *it;
    }

    void dfs_forward_recursive(size_t u, std::vector<bool>& visited) const {
        visited[u] = true;
        for (size_t v = 0; v < _vert_count; v++) {
            if (!visited[v] && at(_matrix[u], v) != NO_EDGE) {
                dfs_forward_recursive(v, visited);
            }
        }
    }

    std::vector<bool> dfs_forward(const Vertex& start) const {
        std::vector<bool> visited(_vert_count, false);
        dfs_forward_recursive(get_index(start), visited);
        return visited;
    }

    void dfs_reverse_recursive(size_t u, std::vector<bool>& visited) const {
        visited[u] = true;
        for (size_t v = 0; v < _vert_count; v++) {
            if (!visited[v] && at(_matrix[v], u) != NO_EDGE) {
                dfs_reverse_recursive(v, visited);
            }
        }
    }
    
    std::vector<bool> dfs_reverse(const Vertex& start) const {
        std::vector<bool> visited(_vert_count, false);
        dfs_reverse_recursive(get_index(start), visited);
        return visited;
    }

    void walk_recursive(size_t u, 
                        std::vector<bool>& visited, 
                        std::vector<Vertex>& result, 
                        const std::function<void(const Vertex&)>& action) const {
        visited[u] = true;
        action(_vertices[u]);
        result.push_back(_vertices[u]);

        for (size_t v = 0; v < _vert_count; v++) {
            if (!visited[v] && at(_matrix[u], v) != NO_EDGE) {
                walk_recursive(v, visited, result, action);
            }
        }
    }

public:
    //vertex - вершина 
    //edge - ребро 

    struct Edge{
        Vertex from;
        Vertex to;
        Distance weight;

        Edge(const Vertex& f, const Vertex& t, Distance w) : from(f), to(t), weight(w) {}
    };

    bool has_vertex(const Vertex& v) const {
        if (_indices.find(v) != _indices.end()) {
            return true;
        } 

        return false;
    }

    bool add_vertex(const Vertex& v) {
        if (has_vertex(v)) {
            return false;
        }

        size_t index = _vert_count++;
        _indices[v] = index;
        _vertices.push_back(v);

        for (auto& row : _matrix) {
            row.push_back(NO_EDGE);
        }

        _matrix.emplace_back(_vert_count, NO_EDGE);

        return true;
    }

    bool remove_vertex(const Vertex& v) {
        if (!has_vertex(v)) {
            return false;
        }

        size_t index = get_index(v);

        _matrix.erase(_matrix.begin() + index);

        for (auto& row : _matrix) {
            auto it = row.begin();
            std::advance(it, index);
            row.erase(it);
        }

        _indices.erase(v);
        _vertices.erase(_vertices.begin() + index);

        for (auto& pair : _indices) {
            if (pair.second > index) {
                pair.second--;
            }
        }

        _vert_count--;

        return true;
    }

    std::vector<Vertex> vertices() const {
        return _vertices;
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
        if (!has_vertex(from) || !has_vertex(to)) {
            throw std::runtime_error("Vertex not found"); 
        }

        size_t i = get_index(from);
        size_t j = get_index(to);

        at(_matrix[i], j) = d;
    }

    bool remove_edge(const Vertex& from, const Vertex& to) {
        if (!has_edge(from, to)) {
            return false;
        }

        size_t i = get_index(from);
        size_t j = get_index(to);
        at(_matrix[i], j) = NO_EDGE;

        return true;
    }

    bool remove_edge(const Edge& e) {
        if (!has_edge(e.from, e.to)) {
            return false;
        }

        size_t i = get_index(e.from);
        size_t j = get_index(e.to);

        if (at(_matrix[i], j) == e.weight) {
            at(_matrix[i], j) = NO_EDGE;
            return true;
        }

        return false;
    }

    bool has_edge(const Vertex& from, const Vertex& to) const {
        size_t i = get_index(from);
        size_t j = get_index(to);

        return !(at(_matrix[i], j) == NO_EDGE);
    }

    bool has_edge (const Edge& e) const {
        if (has_edge(e.from, e.to)) {
            size_t i = get_index(e.from);
            size_t j = get_index(e.to);

            if (at(_matrix[i], j) == e.weight) {
                return true;
            }
        }
        return false;
    } 
    
    std::vector<Edge> edges(const Vertex& vertex) {
        size_t i = get_index(vertex);
        std::vector<Edge> result;

        for (size_t j = 0; j < _vert_count; j++) {
            Edge edge(vertex, _vertices[j], at(_matrix[i], j));

            if (edge.weight != NO_EDGE) {
                result.push_back(edge);
            }
        }

        return result;
    }

    size_t order() const {
        return _vert_count;
    }

    size_t degree(const Vertex& v) const {
        size_t count = 0;
        size_t index = get_index(v);

        for (const Distance& weight : _matrix[index]) {
            if (weight != NO_EDGE) {
                count++;
            }
        }

        return count;
    } 

    bool is_connected() const {
        if (_vert_count == 0) {
            return true;
        }

        Vertex start = _vertices[0];

        std::vector<bool> forward_visited = dfs_forward(start);
        for (size_t i = 0; i < _vert_count; i++) {
            if (!forward_visited[i]) {
                return false;
            }
        }

        std::vector<bool> reverse_visited = dfs_reverse(start);
        for (size_t i = 0; i < _vert_count; i++) {
            if (!reverse_visited[i]) {
                return false;
            }
        }

        return true;
    } //является ли граф сильно связным

    //поиск кратчайшего пути
    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
        if (!has_vertex(from) || !has_vertex(to)) {
            throw std::runtime_error("Vertex not found");
        }

        size_t src = get_index(from);
        size_t dst = get_index(to);

        std::vector<Distance> dist(_vert_count, NO_EDGE);
        std::vector<size_t> parent(_vert_count, _vert_count);
        dist[src] = 0;

        for (size_t k = 0; k < _vert_count - 1; ++k) {
            bool changed = false;
            for (size_t u = 0; u < _vert_count; ++u) {
                if (dist[u] != NO_EDGE) {
                    for (size_t v = 0; v < _vert_count; ++v) {
                        Distance w = at(_matrix[u], v);
                        if (w != NO_EDGE && dist[u] + w < dist[v]) {
                            dist[v] = dist[u] + w;
                            parent[v] = u;
                            changed = true;
                        }
                    }
                }
            }
            if (!changed) {
                break;
            }
        }

        if (dist[dst] == NO_EDGE) return {};

        std::vector<Edge> path;
        for (size_t cur = dst; cur != src; cur = parent[cur]) {
            path.emplace_back(_vertices[parent[cur]], _vertices[cur], at(_matrix[parent[cur]], cur));
        }
        std::reverse(path.begin(), path.end());
        return path;
    }
    
    //обход
    std::vector<Vertex>  walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action)const {
        if (!has_vertex(start_vertex)) {
            throw std::runtime_error("Vertex not found");
        }

        std::vector<bool> visited(_vert_count, false);
        std::vector<Vertex> result;
        walk_recursive(get_index(start_vertex), visited, result, action);
        return result;
    }

    void print() const {
        std::cout << "=== Graph (" << _vert_count << " vertices) ===\n";
        
        for (size_t i = 0; i < _vert_count; ++i) {
            std::cout << _vertices[i] << " -> ";
            bool first = true;
            
            for (size_t j = 0; j < _vert_count; ++j) {
                Distance w = at(_matrix[i], j);
                if (w != NO_EDGE) {
                    if (!first) std::cout << ", ";
                    std::cout << _vertices[j] << "[" << w << "]";
                    first = false;
                }
            }
            
            if (first) {
                std::cout << "(no edges)";
            }
            std::cout << "\n";
        }
    }
};

#endif