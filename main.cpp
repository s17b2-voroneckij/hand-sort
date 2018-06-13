#include <vector>
#include <set>
#include <iostream>

using std::cin;
using std::vector;
using std::set;
using std::pair;
using std::cout;
using std::endl;
using std::make_pair;

struct edge {
    int flow = 0;
    int v, u;
    int t;
};

class flow {
    vector<edge> edges;
    vector<vector<int>> graph;
    vector<int> potential;
    vector<int> dist;
    vector<int> Prev;
    size_t n;
    long long cost = 0;

    void ford_bellman() {
        potential.assign(n + 1, 1e9);
        potential[1] = 0;
        for (size_t i = 0; i < n - 1; i++) {
            for (size_t j = 0; j < edges.size(); j++) {
                auto e = edges[j];
                if (potential[e.v] < 1e9 && potential[e.u] > potential[e.v] + e.t)
                    potential[e.u] = potential[e.v] + e.t, Prev[e.u] = j;
                if (potential[e.u] < 1e9 && potential[e.v] > potential[e.u] + e.t)
                    potential[e.v] = potential[e.u] + e.t, Prev[e.v] = j;
            }
        }
    }

    void dijkstra() {
        dist.assign(n + 1, 1e9);
        dist[1] = 0;
        set<pair<int, int>> heap;
        for (size_t i = 1; i <= n; i++)
            heap.insert({dist[i], i});
        while (!heap.empty()) {
            int curr = heap.begin()->second;
            heap.erase(heap.begin());
            for (auto num : graph[curr]) {
                auto edge = edges[num];
                int w = edge.t;
                int oth = curr ^ edge.v ^ edge.u;

                if ((edge.flow == 1 && edge.u == oth) || (edge.flow == -1 && edge.v == oth))
                    continue;
                if (edge.flow != 0)
                    w *= -1;
                w = w + potential[curr] - potential[oth];
                if (dist[oth] > dist[curr] + w) {
                    heap.erase({dist[oth], oth});
                    dist[oth] = dist[curr] + w;
                    Prev[oth] = num;
                    heap.insert({dist[oth], oth});
                }
            }
        }

        for (size_t i = 1; i <= n; i++)
            potential[i] += dist[i];
    }

public:
    void set_n(int a) {
        n = a;
        graph.resize(n + 1);
        dist.resize(n + 1);
        Prev.resize(n + 1);
        potential.resize(n + 1);
    }

    void add_edge(edge curr) {
        graph[curr.u].push_back(edges.size());
        graph[curr.v].push_back(edges.size());
        edges.push_back(curr);
    }

    bool add_flows(int k) {
        ford_bellman();
        for (size_t i = 0; i < k; i++) {
            if (potential[n] > 9e8 || dist[n] > 9e8) {
                return false;
            }

            int curr = n;
            while (curr != 1) {
                int road = Prev[curr];
                if (abs(edges[road].flow) == 1)
                    edges[road].flow = 0;
                else if (edges[road].u == curr)
                    edges[road].flow = 1;
                else
                    edges[road].flow = -1;
                if (edges[road].flow == 0)
                    cost -= edges[road].t;
                else
                    cost += edges[road].t;
                curr = curr ^ edges[road].v ^ edges[road].u;

            }
            dijkstra();
        }
        return true;
    }

    vector<vector<int>> get_ways(int k) {
        vector<vector<int>> res(k);
        for (size_t i = 0; i < k; i++) {
            int curr = 1;
            while (curr != n) {
                for (auto num : graph[curr]) {
                    auto edge = edges[num];
                    if ((edge.v == curr && edge.flow == 1) || (edge.u == curr && edge.flow == -1)) {
                        res[i].push_back(num);
                        curr = edge.u ^ edge.v ^ curr;
                        edges[num].flow = 0;
                        break;
                    }
                }
            }
        }
        return res;
    }

    long long get_cost() const {
        return cost;
    }

};

auto read_graph() {
    int n, m, k;
    cin >> n >> m >> k;
    flow graph;
    graph.set_n(n);

    for (size_t i = 0; i < m; i++) {
        edge temp;
        cin >> temp.v >> temp.u >> temp.t;
        graph.add_edge(temp);
    }
    return make_pair(k, graph);
}

void solve(int k, flow graph) {
    if (!graph.add_flows(k)) {
        cout << -1;
        return ;
    }


    cout.precision(20);
    cout << (double)graph.get_cost() / k << endl;
    for (const auto& way : graph.get_ways(k)) {
        cout << way.size() << " ";
        for (int v : way)
            cout << v + 1 << " ";
        cout << "\n";
    }
}

int main() {
    auto input = read_graph();
    solve(input.first, input.second);
}
