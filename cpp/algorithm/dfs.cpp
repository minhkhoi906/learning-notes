#include <iostream>
#include <vector>

using namespace std;

class Graph
{
  private:
    int vertices_;
    vector<vector<int>> neighbors_;

    void dfs_util(int vertex, vector<bool> &visited) const
    {
        visited[vertex] = true;
        cout << vertex << " ";

        for (int neighbor : neighbors_[vertex]) {
            if (!visited[neighbor]) {
                dfs_util(neighbor, visited);
            }
        }
    }

  public:
    explicit Graph(int v) : vertices_(v) { neighbors_.resize(v); }

    void add_edge(int v, int neighbor) { neighbors_[v].emplace_back(neighbor); }

    void dfs(int start_vertex) const
    {
        vector<bool> visited(vertices_, false);
        dfs_util(start_vertex, visited);
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
    g.add_edge(4, 3);

    g.dfs(1);

    return 0;
}
