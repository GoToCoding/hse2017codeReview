#include <bits/stdc++.h>
typedef long double ld;
typedef long long ll;
#define v1 (2 * v + 1)
#define v2 (2 * v + 2)
#define d(x) cerr << #x << ": " << x << endl;
using namespace std;
template<class T> ostream& operator<<(ostream& os, vector<T>& v) {for (auto i : v) os << i << " "; return os;}
template<class T> ostream& operator<<(ostream& os, set<T>& v) {for (auto i : v) os << i << " "; return os;}
template<class T, class R> ostream& operator<<(ostream& os, pair<T, R>& v) {os << '(' << v.first << ' ' << v.second << ')' << ' '; return os;}

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

const int inf = 2e9;
vector<int> d, p;
vector<pair<int, int>> parent;

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
        int capacity = edges[id].capacity;
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

int main() {
    ios_base::sync_with_stdio(0);
    //freopen("input.txt", "r", stdin);

    int n, m, k;
    cin >> n >> m >> k;
    source = 1;
    sink = n;

    g.resize(n + 1, vector<int>(0));
    used.resize(n + 1, false);
    p.resize(n + 1, 0);
    d.resize(n + 1, 0);
    parent.resize(n + 1);

    for (int i = 0, now = 0; i < m; i++, now += 4) {
        int a, b, c;
        cin >> a >> b >> c;
        g[a].push_back(now + 0);
        g[b].push_back(now + 1);
        edges.push_back({b, 1, 0, c});
        edges.push_back({a, 0, 0, -c});

        g[b].push_back(now + 2);
        g[a].push_back(now + 3);
        edges.push_back({a, 1, 0, c});
        edges.push_back({b, 0, 0, -c});

    }

    dijkstra();
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
        cout << "-1\n";
        return 0;
    }
    cout << fixed << setprecision(10) << (long double)c / k << endl;
    for (int pathNum = 0; pathNum < k; pathNum++) {
        for (int i = 1; i <= sink; i++) used[i] = false;
        vector<pair<int, int>> way = find_way(source);
        reverse(way.begin(), way.end());
        cout << way.size() - 1 << " ";
        for (auto& p : way)
            if (p.second != -1)
                cout << p.second / 4 + 1 << " ";
        cout << endl;
    }
}
