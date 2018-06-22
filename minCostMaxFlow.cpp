#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>

using std::pair;
using std::priority_queue;
using std::vector;

static constexpr int inf = std::numeric_limits<int>::max();

class Graph {
private:
    struct Edge {
        int to;
        int capacity;
        int flow;
        int cost;
        Edge (int to, int capacity, int flow, int cost):
            to(to),
            capacity(capacity),
            flow(flow),
            cost(cost) {}
    };

    int source;
    int sink;
    vector<Edge> edges;
    vector<vector<int>> g;
    vector<bool> used;
    vector<int> d, p;
    vector<pair<int, int>> parent;

public:
    Graph(int vertexNumber):
        sink(vertexNumber),
        source(1) {
        g.resize(vertexNumber + 1, vector<int>(0));
        used.resize(vertexNumber + 1, false);
        p.resize(vertexNumber + 1, 0);
        d.resize(vertexNumber + 1, 0);
        parent.resize(vertexNumber + 1);
    }

    pair<int, int> getParent(int v) {
        return parent[v];
    }

    void addEdge(int from, int to, int cost, int capacity = 1) {
        g[from].push_back(edges.size());
        edges.push_back(Edge(to, capacity, 0, cost));
        g[to].push_back(edges.size());
        edges.push_back(Edge(from, 0, 0, -cost));
    }

    void fordBellman() {
        for (int i = 1; i <= sink; i++) d[i] = inf;
        d[source] = 0;
        parent[source].first = -1;

        int minDist[sink + 1][sink + 1];
        for (int i = 1; i <= sink; i++) {
            for (int j = 0; j <= sink; j++)
                minDist[i][j] = inf;
        }
        minDist[source][0] = 0;
        for (int i = 1; i <= sink; i++) {
            for (int edgeId = 0; edgeId < (int)edges.size(); edgeId++) {
                int v = edges[edgeId].to;
                int u = edges[edgeId ^ 1].to;
                int costUV = edges[edgeId].cost;
                if (edges[edgeId].capacity == edges[edgeId].flow) continue;
                if (minDist[u][i - 1] < inf && minDist[v][i] > minDist[u][i - 1] + costUV) {
                    minDist[v][i] = minDist[u][i - 1] + costUV;
                    if (minDist[v][i] < d[v]) {
                        d[v] = minDist[v][i];
                        parent[v] = {u, edgeId};
                    }
                }
            }
        }
        for (int i = 0; i <= sink; i++) p[i] = d[i];
    }

    void dijkstra() {
        std::fill(d.begin(), d.end(), inf);
        d[source] = 0;
        parent[source].first = -1;
        priority_queue<pair<int, int>> q;
        q.push({0, source});
        while (q.size()) {
            int dist = -q.top().first;
            int v = q.top().second;
            q.pop();
            if (dist > d[v]) continue;
            for (int id : g[v]) {
                if (edges[id].capacity == edges[id].flow) continue;
                int to = edges[id].to;
                int c = edges[id].cost;
                if (d[to] > dist + c + p[v] - p[to]) {
                    d[to] = dist + c + p[v] - p[to];
                    parent[to] = {v, id};
                    q.push({-d[to], to});
                }
            }
        }
        for (int i = 0; i <= sink; i++) p[i] = d[i];
    }


    vector<pair<int, int>> find_way(int v) {
        if (v == source) {
            std::fill(used.begin(), used.end(), false);
        }
        used[v] = true;
        if (v == sink) {
            vector<pair<int, int>> ans(1);
            ans[0] = {v, -1};
            return ans;
        }
        for (int id : g[v]) {
            int to = edges[id].to;
            int flow = edges[id].flow;
            if (used[to]) continue;
            if (flow > 0) {
                auto ans = find_way(to);
                if (ans.empty()) continue;
                edges[id].flow--;
                edges[id ^ 1].flow++;
                ans.push_back({v, id});
                return ans;
            }
        }
        return vector<pair<int, int>>(0);
    }

    bool canAddFlow() const {
        return d[sink] != inf;
    }

    void addFlowToEdge(int id, int delta) {
        edges[id].flow += delta;
        edges[id ^ 1].flow -= delta;
    }

    int getEdgeCost(int id) const {
        return edges[id].cost;
    }

    int getSource() const {
        return source;
    }
};


int main() {

    int n, m, k;
    std::cin >> n >> m >> k;

    Graph minCostFlow = Graph(n);

    for (int i = 0; i < m; i++) {
        int a, b, c;
        std::cin >> a >> b >> c;
        minCostFlow.addEdge(a, b, c);
        minCostFlow.addEdge(b, a, c);
    }

    minCostFlow.fordBellman();

    int c = 0;
    int f = 0;

    while (minCostFlow.canAddFlow() && f < k) {
        f++;
        int v = n;
        while (minCostFlow.getParent(v).first != -1) {
            int u = minCostFlow.getParent(v).first;
            int id = minCostFlow.getParent(v).second;
            minCostFlow.addFlowToEdge(id, 1);
            c += minCostFlow.getEdgeCost(id);
            v = u;
        }
        minCostFlow.dijkstra();
    }
    if (f < k) {
        std::cout << "-1\n";
        return 0;
    }
    std::cout << std::fixed << std::setprecision(10) << static_cast<long double>(c) / k << '\n';
    for (int pathNum = 0; pathNum < k; pathNum++) {
        vector<pair<int, int>> way = minCostFlow.find_way(minCostFlow.getSource());
        std::reverse(way.begin(), way.end());
        std::cout << way.size() - 1 << " ";
        for (auto& p : way)
            if (p.second != -1)
                std::cout << p.second / 4 + 1 << " ";
        std::cout << '\n';
    }
}
