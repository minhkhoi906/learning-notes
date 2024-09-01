#include <iostream>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

class Graph
{
  private:
    using vectex_dt = pair<int, int>;
    int vertices_;
    vector<vector<pair<int, int>>> neighbors_;

  public:
    explicit Graph(int v) : vertices_(v) { neighbors_.resize(v); }

    void add_edge(int v, int neighbor, int distance)
    {
        neighbors_[v].emplace_back(neighbor, distance);
    }

    vector<int> dijkstra(int start) const
    {
        auto compare = [](const vectex_dt &a, const vectex_dt &b) -> bool {
            return a.second > b.second;
        };
        vector<int> dists(vertices_, numeric_limits<int>::max());
        vector<bool> visited(vertices_, false);
        priority_queue<vectex_dt, vector<vectex_dt>, decltype(compare)>
            min_dist_queue(compare);

        min_dist_queue.push({start, 0});
        dists[start] = 0;

        while (!min_dist_queue.empty()) {
            int u = min_dist_queue.top().first;
            cout << u << endl;
            min_dist_queue.pop();
            if (visited[u]) {
                continue;
            }

            visited[u] = true;

            for (auto &&neighbor : neighbors_[u]) {
                int v = neighbor.first;
                int weight = neighbor.second;

                if (!visited[v] && dists[u] + weight < dists[v]) {
                    dists[v] = dists[u] + weight;
                    min_dist_queue.push({v, dists[v]});
                }
            }
        }

        return dists;
    }
};

int main()
{
    Graph g(5);

    g.add_edge(0, 1, 10);
    g.add_edge(0, 2, 2);
    g.add_edge(1, 4, 5);
    g.add_edge(1, 2, 3);
    g.add_edge(2, 4, 1);
    g.add_edge(4, 3, 6);

    auto dists = g.dijkstra(0);
    int idx{0};
    for (auto &&dist : dists) {
        cout << idx++ << ": " << dist << endl;
    }

    return 0;
}
