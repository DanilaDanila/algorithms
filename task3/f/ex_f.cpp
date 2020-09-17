#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

// Stoer-Wagner alg
void findBestSlice(vector<vector<int>> &g, vector<int> &best_cut) {
    int n = g.size();
    unsigned best_w = -1;
    vector<vector<int>> v(n);  // compressed vertexes
    vector<bool> exist(n, true);
    int max_w, pre;

    for (int i = 0; i < n; ++i)
        v[i].assign(1, i);

    for (int j = 0; j < n - 1; ++j) {
        vector<bool> in_a(n, false);
        vector<int> w(n, 0);

        for (int x = 0; x < n - j; ++x) {
            max_w = -1;
            for (int i = 0; i < n; ++i)
                if (exist[i] &&
                   !in_a[i] &&
                   (max_w == -1  || w[i] > w[max_w]))
                    max_w = i;

            if (x == n - j - 1) {
                if (w[max_w] < best_w) {
                    best_w = w[max_w];
                    best_cut = v[max_w];
                }

                v[pre].insert(v[pre].end(), v[max_w].begin(), v[max_w].end());


                for (int i = 0; i < n; ++i) {
                    g[i][pre] += g[max_w][i];
                    g[pre][i] = g[i][pre];
                }

                exist[max_w] = false;
            } else {
                in_a[max_w] = true;
                for (int i = 0; i < n; ++i)
                    w[i] += g[max_w][i];

                pre = max_w;
            }
        }
    }
}

void printParts(const vector<int> &best_cut, int n) {
    vector<bool> parts(n, false);  // [0=false] in 'part0'; [1=true] in [part1]
    for (int i = 0; i < best_cut.size(); ++i)
        parts[best_cut[i]] = true;

    for (int i = 0; i < n; ++i)
        if (!parts[i])
            cout << i + 1 << ' ';

    cout << '\n';

    for (int i = 0; i < n; ++i)
        if (parts[i])
            cout << i + 1 << ' ';

    cout << '\n';
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> graph(n);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            char c;
            cin >> c;
            graph[i].push_back(c-'0');
        }

    vector<int> best_slice;
    findBestSlice(graph, best_slice);

    printParts(best_slice, n);

    return 0;
}
