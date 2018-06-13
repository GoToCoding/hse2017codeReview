#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>

using std::pair;
using std::priority_queue;
using std::vector;

struct Edge {
    int to;
    int capacity;
    int flow;
    int cost;
};

vector<Edge> edges;
vector<vector<int>> g;
int source;
int sink;

const int inf = 2000000000; //  компилятор на ЯКе ругается, если написать 2'000'000'000
vector<int> d, p;
vector<pair<int, int>> parent;

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
}

void dijkstra() {
    for (int i = 1; i <= sink; i++) d[i] = inf;
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
}

vector<bool> used;

vector<pair<int, int>> find_way(int v) {
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
            ans.emplace_back({v, id});
            return ans;
        }
    }
    return vector<pair<int, int>>(0);
}

int main() {

    int n, m, k;
    std::cin >> n >> m >> k;
    source = 1;
    sink = n;

    g.resize(n + 1, vector<int>(0));
    used.resize(n + 1, false);
    p.resize(n + 1, 0);
    d.resize(n + 1, 0);
    parent.resize(n + 1);

    for (int i = 0, now = 0; i < m; i++, now += 4) {
        int a, b, c;
        std::cin >> a >> b >> c;
        g[a].push_back(now);
        g[b].push_back(now + 1);

        edges.push_back({b, 1, 0, c});
        edges.push_back({a, 0, 0, -c});

        g[b].push_back(now + 2);
        g[a].push_back(now + 3);
        edges.push_back({a, 1, 0, c});
        edges.push_back({b, 0, 0, -c});

    }

    fordBellman();

    for (int i = 0; i <= sink; i++) p[i] = d[i];

    int c = 0;
    int f = 0;

    while (d[sink] != inf && f < k) {
        f++;
        int v = sink;
        while (parent[v].first != -1) {
            int u = parent[v].first;
            int id = parent[v].second;
            edges[id].flow++;
            edges[id ^ 1].flow--;
            c += edges[id].cost;
            v = u;
        }
        dijkstra();
        for (int i = 0; i <= sink; i++) p[i] = d[i];
    }
    if (f < k) {
        std::cout << "-1\n";
        return 0;
    }
    std::cout << std::fixed << std::setprecision(10) << static_cast<long double>(c) / k << '\n';
    for (int pathNum = 0; pathNum < k; pathNum++) {
        for (int i = 1; i <= sink; i++) used[i] = false;
        vector<pair<int, int>> way = find_way(source);
        std::reverse(way.begin(), way.end());
        std::cout << way.size() - 1 << " ";
        for (auto& p : way)
            if (p.second != -1)
                std::cout << p.second / 4 + 1 << " ";
        std::cout << '\n';
    }
}
