#include <iostream>
#include <algorithm>
#include <cmath>
#include <utility>

using std::cin;
using std::cout;
using std::pair;
using std::make_pair;
using std::min;

pair<int, int> min(pair<int, int> a, pair<int, int> b) {
    return (a.first < b.first) ? a : b;
}

class STable {
  private:
    pair<int, int> **tab;  // <elem, id>
    int len;
    int log2_len;

  public:
    STable(int seq_len):
        len(seq_len) {
        log2_len = ceil(log2(len));

        tab = new pair<int, int>*[len];
        for (int i = 0; i < len; ++i)
            tab[i] = new pair<int, int>[log2_len + 1];
    }

    ~STable() {
        for (int i = 0; i < len; ++i)
            delete [] tab[i];
        delete [] tab;
    }

    void setElem(int id, int elem) {
        tab[id][0] = make_pair(elem, id);
    }

    void buildTable() {
        for (int j = 1; j <= log2_len; ++j)
            for (int i = 0; i + (1 << j) <= len; ++i)
                tab[i][j] = min(tab[i][j - 1], tab[i + (1 << (j - 1))][j - 1]);
    }

    pair<int, int> query(int l, int r) {
        int j = floor(log2(r - l + 1));
        return min(tab[l][j], tab[r - (1 << j) + 1][j]);
    }

    int query2(int l, int r) {
        int min_id = query(l, r).second;
        if (min_id == l)
            return query(l + 1, r).first;
        if (min_id == r)
            return query(l, r - 1).first;
        return min(query(l, min_id - 1).first, query(min_id + 1, r).first);
    }
};

int main() {
    int N, M;
    cin >> N >> M;

    STable seq(N);

    for (int i = 0; i < N; ++i) {
        int elem = 0;
        cin >> elem;
        seq.setElem(i, elem);
    }

    seq.buildTable();

    for (int i = 0; i < M; ++i) {
        int l, r;
        cin >> l >> r;
        cout << seq.query2(l-1, r-1) << '\n';
    }

    return 0;
}
