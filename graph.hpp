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

    size_t get_index(const Vertex& vert) const {
        auto it = _indices.find(vert);
        if (it == _indices.end()) {
            throw std::exception("Вершины нет в графе!");
        }

        return it->second;
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
        if (_vertices.contains(v)) {
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
            row.push_back(std::list<Distance>());
        }

        _matrices.push_back(std::list<Distance>(_vert_count));
        return true;
    }

    bool remove_vertex(const Vertex& v) {
        if (!has_vertex(v)) {
            return false;
        }

        size_t index = get_index(v);

        _matrix.erase(_matrix.begin() + index);

        for (auto& row : _matrices) {
            auto it = row.begin();
            std::advance(it, index);
            row.erase(it);
        }

        _indices.erase(v);
        _vertices.erase(_vertices.begin() + index);

        for (auto& pair : _indices) {
            if (pair->second > index) {
                pair->second--;
            }
        }

        _vert_count--;
        return true;
    }

    std::vector<Vertex> vertices() const {
        return _vertices;
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d);
    bool remove_edge(const Vertex& from, const Vertex& to);
    bool remove_edge(const Edge& e); //c учетом расстояния
    bool has_edge(const Vertex& from, const Vertex& to) const;
    bool has_edge (const Edge& e) const; //c учетом расстояния в Edge
    
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