#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::min;
using std::max;

class SegTree {
  private:
    const int size;
    vector<int> tree;
    int (*func)(int, int);
    int leafs_count;

    inline int getParentId(int id) {
        return (id-1)/2;
    }

    inline int getChildLeftId(int id) {
        return id*2 + 1;
    }

    inline int getChildRightId(int id) {
        return id*2 + 2;
    }

  public:
    SegTree(const int arr_size, int (*f)(int, int) =[](int a, int b) { return min(a, b); }):
        size(arr_size),
        func(f) {
        leafs_count = 1 << int(floor(log2(size - 1)) + 1);
        tree.resize(leafs_count*2 - 1, 0);
    }

    ~SegTree() = default;

    // leafs (array)
    const int operator[](const int id) const {
        return tree[tree.size()-leafs_count+id];
    }

    int &operator[](const int id) {
        return tree[tree.size()-leafs_count+id];
    }

    void build() {
        for (int cur_id = tree.size() / 2 - 1; cur_id >= 0; --cur_id)
            tree[cur_id] = func(tree[getChildLeftId(cur_id)], tree[getChildRightId(cur_id)]);
    }

    void incArr(int left, int right, int c) {
        for (int id = left; id <= right; ++id)
            (*this)[id] += c;

        // tree.size() - leafs_count + right = id
        for (int id = getParentId(tree.size() - leafs_count + right); id >= 0; --id) {
            tree[id] = func(tree[getChildLeftId(id)], tree[getChildRightId(id)]);
        }
    }

    void decArr(int left, int right, int c) {
        incArr(left, right, -c);
    }

    int value() const {
        return tree[0];
    }

    friend std::ostream &operator<<(std::ostream&, const SegTree&);
};

std::ostream &operator<<(std::ostream &out, const SegTree &tree) {
    for (int i = 0, t = 0, c = 1; i < tree.tree.size(); ++i) {
        out << tree.tree[i];
        ++t;
        if (t == c) {
            out << '\n';
            c *= 2;
            t = 0;
        }
    }
    return out;
}

int main() {
    int stations_count;
    cin >> stations_count;

    SegTree tree(stations_count - 1, [](int a, int b) { return max(a, b); });

    for (int i = 0; i < stations_count - 1; ++i)
        cin >> tree[i];

    tree.build();

    int train_cap;
    cin >> train_cap;

    int transactions;
    cin >> transactions;
    for (int i = 0; i < transactions; ++i) {
        int station_from, station_to, count;
        cin >> station_from >> station_to >> count;
        tree.incArr(station_from, station_to-1, count);

        if (tree.value() > train_cap) {
            tree.decArr(station_from, station_to-1, count);
            cout << i << '\n';
        }
    }

    return 0;
}
