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

class SegmentTree {
  private:
    const int size;
    vector <int> tree;
    vector <int> updates;

    // [l, r)
    void buildFromVec(vector <int>& vec, int id, int l, int r) {
        if (l + 1 == r) {
            tree[id] = vec[l];
        } else {
            buildFromVec(vec, id * 2, l, (l + r) / 2);
            buildFromVec(vec, id * 2 + 1, (l + r) / 2, r);
            tree[id] = min(tree[id * 2], tree[id * 2 + 1]);
        }
    }

    void push(int id) {
        if (updates[id] != 1000) {
            tree[id * 2] = tree[id];
            tree[id * 2 + 1] = tree[id];

            updates[id * 2] = updates[id];
            updates[id * 2 + 1] = updates[id];
            updates[id] = 1000;
        }
    }

    // [l, r) && [l_in, r_in)
    void updateArr(int id, int l, int r, int l_in, int r_in, int c) {
        if (l_in >= r_in)
            return;

        if (l == l_in && r == r_in) {
            tree[id] = c;
            updates[id] = c;
        } else  {
            push(id);
            int m = (l + r) / 2;
            updateArr(id * 2, l, m, l_in, min(m, r_in), c);
            updateArr(id * 2 + 1, m, r, max(l_in, m), r_in, c);
            tree[id] = min(tree[id * 2], tree[id * 2 + 1]);
        }
    }

    // [l, r) && [l_in, r_in)
    int rmq(int id, int l, int r, int l_in, int r_in) {
        if (l_in >= r_in)
            return 1000;

        if (l == l_in && r == r_in)
            return tree[id];

        push(id);
        int m = (l + r) / 2;
        return min(rmq(id * 2, l, m, l_in, min(r_in, m)), rmq(id * 2 + 1, m, r, max(l_in, m), r_in));
    }

  public:
    SegmentTree(vector <int>& vec):
        size(vec.size()) {
        tree.assign(4 * size, 1000);
        buildFromVec(vec, 1, 0, size);
        updates.assign(4 * size, 1000);
    }

    ~SegmentTree() = default;

    // [l, r)
    int query(int l, int r) {
        return rmq(1, 0, size, l, r);
    }

    // [l, r)
    void chArr(int l, int r, int c) {
        updateArr(1, 0, size, l, r, c);
    }
};

int main() {
    int len;
    cin >> len;

    vector <int> segment(len);
    for (int i = 0; i < len; ++i) {
        int r, g, b;
        cin >> r >> g >> b;
        segment[i] = r+g+b;
    }

    SegmentTree tree(segment);

    int photos_count;
    cin >> photos_count;

    for (int i = 0; i < photos_count; ++i) {
        int line_l, line_r, r, g, b, photo_l, photo_r;
        cin >> line_l >> line_r >> r >> g >> b >> photo_l >> photo_r;

        tree.chArr(line_l, line_r+1, r+g+b);

        cout << tree.query(photo_l, photo_r+1) << '\n';
    }

    return 0;
}
