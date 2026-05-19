#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <iostream>
#include <iterator>
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

    static constexpr Distance NO_EDGE = std::numeric_limits<Distance>::max();

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

        _matrix.push_back(_vert_count, NO_EDGE);

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
        if (!has_edge(from, to)) {
            return false;
        }

        size_t i = get_index(from);
        size_t j = get_index(to);

        if (at(_matrix[i], j) == e.weight) {
            at(_matrix[i], j) = NO_EDGE;
            return true;
        }

        return false;
    }

    bool has_edge(const Vertex& from, const Vertex& to) const {
        size_t i = get_index(from);
        size_t j = get_index(to);

        auto it = _matrix[i].begin();
        std::advance(it, j);

        return !(*it == NO_EDGE);
    }

    bool has_edge (const Edge& e) const {
        if (has_edge(e.from, e.to)) {
            size_t i = get_index(e.from);
            size_t j = get_index(e.to);

            auto it = _matrix[i].begin();
            std::advance(it, j);

            if (*it == e.weight) {
                return true;
            }
        }
        return false;
    } 
    
    //получение всех ребер, выходящих из вершины
    std::vector<Edge> edges(const Vertex& vertex);

    size_t order() const; //порядок 
    size_t degree(const Vertex& v) const; //степень вершины
    bool is_connected() const; //является ли граф сильно связным

    //поиск кратчайшего пути
    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const; 
    //обход
    std::vector<Vertex>  walk(const Vertex& start_vertex, std::function<void(const Vertex&)> action)const; 
};

#endif