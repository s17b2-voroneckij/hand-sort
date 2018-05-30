#include <vector>
#include <set>
#include <iostream>

using std::cin;
using std::vector;
using std::set;
using std::pair;
using std::cout;
using std::endl;

struct edge {
    int flow = 0;
    int v, u;
    int t;
};

class flow {
    vector<edge> our;
    vector<vector<int>> g;
    vector<int> p;
    vector<int> d;
    vector<int> Prev;
    int n;
    long long cost = 0;

    void ford_bellman() {
        p.assign(n + 1, 1e9);
        p[1] = 0;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < our.size(); j++) {
                auto e = our[j];
                if (p[e.v] < 1e9 && p[e.u] > p[e.v] + e.t)
                    p[e.u] = p[e.v] + e.t, Prev[e.u] = j;
                if (p[e.u] < 1e9 && p[e.v] > p[e.u] + e.t)
                    p[e.v] = p[e.u] + e.t, Prev[e.v] = j;
            }
        }
    }

    void dijkstra() {
        d.assign(n + 1, 1e9);
        d[1] = 0;
        set<pair<int, int>> heap;
        for (int i = 1; i <= n; i++)
            heap.insert({d[i], i});
        while (!heap.empty()) {
            int curr = heap.begin()->second;
            heap.erase(heap.begin());
            for (auto num : g[curr]) {
                auto edge = our[num];
                int w = edge.t;
                int oth = curr ^ edge.v ^ edge.u;

                if ((edge.flow == 1 && edge.u == oth) || (edge.flow == -1 && edge.v == oth))
                    continue;
                if (edge.flow != 0)
                    w *= -1;
                w = w + p[curr] - p[oth];
                if (d[oth] > d[curr] + w) {
                    heap.erase({d[oth], oth});
                    d[oth] = d[curr] + w;
                    Prev[oth] = num;
                    heap.insert({d[oth], oth});
                }
            }
        }

        for (int i = 1; i <= n; i++)
            p[i] += d[i];
    }

public:
    void set_n(int a) {
        n = a;
        g.resize(n + 1);
        d.resize(n + 1);
        Prev.resize(n + 1);
        p.resize(n + 1);
    }

    void add_edge(edge curr) {
        g[curr.u].push_back(our.size());
        g[curr.v].push_back(our.size());
        our.push_back(curr);
    }

    bool add_flows(int k) {
        for (int i = 0; i < k; i++) {
            if (p[n] > 9e8 || d[n] > 9e8) {
                return false;
            }

            int curr = n;
            while (curr != 1) {
                int road = Prev[curr];
                if (abs(our[road].flow) == 1)
                    our[road].flow = 0;
                else if (our[road].u == curr)
                    our[road].flow = 1;
                else
                    our[road].flow = -1;
                if (our[road].flow == 0)
                    cost -= our[road].t;
                else
                    cost += our[road].t;
                curr = curr ^ our[road].v ^ our[road].u;

            }
            dijkstra();
        }
        return true;
    }

    vector<vector<int>> get_ways(int k) {
        vector<vector<int>> res(k);
        for (int i = 0; i < k; i++) {
            int curr = 1;
            while (curr != n) {
                for (auto num : g[curr]) {
                    auto edge = our[num];
                    if ((edge.v == curr && edge.flow == 1) || (edge.u == curr && edge.flow == -1)) {
                        res[i].push_back(num);
                        curr = edge.u ^ edge.v ^ curr;
                        our[num].flow = 0;
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

    void start() {
        ford_bellman();
    }
};

int main() {
    int n, m, k;
    cin >> n >> m >> k;
    flow our;
    our.set_n(n);

    for (int i = 0; i < m; i++) {
        edge temp;
        cin >> temp.v >> temp.u >> temp.t;
        our.add_edge(temp);
    }

    our.start();

    if (!our.add_flows(k)) {
        cout << -1;
        return 0;
    }


    cout.precision(20);
    cout << (double)our.get_cost() / k << endl;
    for (const auto& way : our.get_ways(k)) {
        cout << way.size() << " ";
        for (int v : way)
            cout << v + 1 << " ";
        cout << "\n";
    }
}
