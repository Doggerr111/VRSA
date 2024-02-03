#ifndef LIPVORONOIDIAGRAM_H
#define LIPVORONOIDIAGRAM_H


class LIPVoronoiDiagram
{
public:
    LIPVoronoiDiagram();
};

#endif // LIPVORONOIDIAGRAM_H


#include <iostream>
#include <vector>
#include <algorithm>

// Точка в двумерном пространстве
struct Point {
    int x;
    int y;
};

// Ребро полигона
struct Edge {
    int start; // Индекс точки начала ребра
    int end; // Индекс точки конца ребра
};

// Функция для нахождения полигональной декомпозиции Вороного
std::vector<std::vector<int>> voronoiDecomposition(const std::vector<Point>& points) {
    std::vector<std::vector<int>> result; // Результат - список полигонов
    std::vector<Edge> edges; // Список ребер

    // Создание полигона вокруг каждой точки
    for (int i = 0; i < points.size(); i++) {
        std::vector<int> polygon;

        // Добавление точек полигона
        polygon.push_back(i);

        // Добавление ребер полигона
        for (int j = 0; j < points.size(); j++) {
            if (j != i) {
                Edge edge;
                edge.start = i;
                edge.end = j;
                edges.push_back(edge);
            }
        }

        result.push_back(polygon);
    }

    // Сортировка ребер по расстоянию от начальной точки
    for (auto& edge : edges) {
        edge.start = points[edge.start].x + points[edge.start].y;
        edge.end = points[edge.end].x + points[edge.end].y;
    }

    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.start < b.start;
    });

    // Объединение ребер для построения полигонов
    for (int i = 0; i < edges.size(); i++) {
        Edge& edge = edges[i];

        for (int j = i + 1; j < edges.size(); j++) {
            if (edge.end == edges[j].start) {
                edge.end = edges[j].end;
                edges.erase(edges.begin() + j);
                j = i;
            }
        }
    }

    // Извлечение точек полигонов
    for (auto& polygon : result) {
        std::vector<int> points;

        for (auto& edge : edges) {
            if (std::find(polygon.begin(), polygon.end(), edge.start) != polygon.end()
                && std::find(polygon.begin(), polygon.end(), edge.end) == polygon.end()) {
                points.push_back(edge.end);
            }
        }

        polygon.insert(polygon.end(), points.begin(), points.end());
    }

    return result;
}
