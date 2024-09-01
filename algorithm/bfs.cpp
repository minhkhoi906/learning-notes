#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Graph
{
  private:
    int vertices_;
    vector<vector<int>> neighbors_;

  public:
    explicit Graph(int v) : vertices_(v) { neighbors_.resize(vertices_); }

    void add_edge(int v, int neighbor) { neighbors_[v].emplace_back(neighbor); }

    void bfs(int v) const
    {
        vector<bool> visited(vertices_, false);
        queue<int> next_vertices;
        next_vertices.push(v);
        visited[v] = true;

        while (!next_vertices.empty()) {
            int s = next_vertices.front();
            cout << s << " ";
            next_vertices.pop();

            for (int neighbor : neighbors_[s]) {
                if (!visited[neighbor]) {
                    next_vertices.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
    }
};

int main()
{
    Graph g(5);

    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 4);
    g.add_edge(1, 2);
    g.add_edge(2, 4);
    g.add_edge(3, 4);
    g.add_edge(4, 4);

    g.bfs(0);

    return 0;
}
