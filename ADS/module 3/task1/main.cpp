/* Задача 1. «Представление графа»
 * Дан базовый интерфейс для представления ориентированного графа:
 * struct IGraph {
 *     virtual ~IGraph() {}
 *     // Добавление ребра от from к to.
 *     virtual void AddEdge(int from, int to) = 0;
 *     virtual int VerticesCount() const  = 0;
 *     virtual vector<int> GetNextVertices(int vertex) const = 0;
 *     virtual vector<int> GetPrevVertices(int vertex) const = 0;
 * };
* Необходимо написать несколько реализаций интерфейса:
* ListGraph, хранящий граф в виде массива списков смежности,
* MatrixGraph, хранящий граф в виде матрицы смежности,
* SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
* ArcGraph, хранящий граф в виде одного массива пар {from, to}.
* Также необходимо реализовать конструктор, принимающий const IGraph&. Такой конструктор должен скопировать переданный граф в создаваемый объект.
* Для каждого класса создавайте отдельные h и cpp файлы.
* Число вершин графа задается в конструкторе каждой реализации.
 * */

#include <iostream>
#include <vector>
#include <queue>
#include "ListGraph.h"
#include "MatrixGraph.h"
#include "ArcGraph.h"
#include "SetGraph.h"

using namespace std;

void BFS(const IGraph &graph, int vertex, vector<bool> &visited, function<void(int)> &func) {
    queue<int> q;
    q.push(vertex);
    visited[vertex] = true;

    while (!q.empty()) {
        int current_vertex = q.front();
        q.pop();
        func(current_vertex);
        for (int next_vertex: graph.GetNextVertices(current_vertex)) {
            if (!visited[next_vertex]) {
                q.push(next_vertex);
                visited[next_vertex] = true;
            }
        }
    }
}

void BFS(const IGraph &graph, function<void(int)> func) {
    vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i])
            BFS(graph, i, visited, func);
    }
}

void DFS(const IGraph &graph, int vertex, vector<bool> &visited, function<void(int)> &func) {
    visited[vertex] = true;
    func(vertex);

    for (int next_vertex: graph.GetNextVertices(vertex)) {
        if (!visited[next_vertex]) {
            DFS(graph, next_vertex, visited, func);
        }
    }
}

void DFS(const IGraph &graph, function<void(int)> func) {
    vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) {
            DFS(graph, i, visited, func);
        }
    }
}

int main() {
    ListGraph list_graph(8);

    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 2);
    list_graph.AddEdge(0, 4);
    list_graph.AddEdge(1, 3);
    list_graph.AddEdge(1, 5);
    list_graph.AddEdge(2, 3);
    list_graph.AddEdge(2, 6);
    list_graph.AddEdge(3, 4);
    list_graph.AddEdge(4, 5);
    list_graph.AddEdge(5, 6);
    list_graph.AddEdge(6, 7);

    cout << "ListGraph" << endl;
    cout << "BFS: ";
    BFS(list_graph, [](int vertex) { cout << vertex << " "; });
    cout << "\n" << "DFS: ";
    DFS(list_graph, [](int vertex) { cout << vertex << " "; });
    cout << "\n" << endl;

    MatrixGraph matrix_graph(list_graph);
    cout << "MatrixGraph" << endl;
    cout << "BFS: ";
    BFS(matrix_graph, [](int vertex) { cout << vertex << " "; });
    cout << "\n" << "DFS: ";
    DFS(matrix_graph, [](int vertex) { cout << vertex << " "; });
    cout << "\n" << endl;

    ArcGraph arc_graph(matrix_graph);
    cout << "ArcGraph" << endl;
    cout << "BFS: ";
    BFS(arc_graph, [](int vertex) { cout << vertex << " "; });
    cout << "\n" << "DFS: ";
    DFS(arc_graph, [](int vertex) { cout << vertex << " "; });
    cout << "\n" << endl;

    SetGraph set_graph(arc_graph);
    cout << "SetGraph" << endl;
    cout << "BFS: ";
    BFS(set_graph, [](int vertex) { cout << vertex << " "; });
    cout << "\n" << "DFS: ";
    DFS(set_graph, [](int vertex) { cout << vertex << " "; });

    return 0;
}