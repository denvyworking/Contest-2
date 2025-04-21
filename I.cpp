#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct SegmentTree {
    int size;
    vector<int> tree;
    vector<int> lazy;

    SegmentTree(const vector<int>& array) {
        size = array.size();
        tree.resize(4 * size);
        lazy.resize(4 * size, 0);
        build(array, 0, 0, size - 1);
    }

    void build(const vector<int>& array, int node, int left, int right) {
        if (left == right) {
            tree[node] = array[left];
        } else {
            int mid = (left + right) / 2;
            build(array, 2 * node + 1, left, mid);
            build(array, 2 * node + 2, mid + 1, right);
            tree[node] = max(tree[2 * node + 1], tree[2 * node + 2]);
        }
    }

    void push(int node, int left, int right) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node];
            if (left != right) {
                lazy[2 * node + 1] += lazy[node];
                lazy[2 * node + 2] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update_range(int node, int left, int right, int l, int r, int add) {
        push(node, left, right);
        if (r < left || l > right) {
            return;
        }
        if (l <= left && right <= r) {
            lazy[node] += add;
            push(node, left, right);
            return;
        }
        int mid = (left + right) / 2;
        update_range(2 * node + 1, left, mid, l, r, add);
        update_range(2 * node + 2, mid + 1, right, l, r, add);
        tree[node] = max(tree[2 * node + 1], tree[2 * node + 2]);
    }

    int query_max(int node, int left, int right, int l, int r) {
        push(node, left, right);
        if (r < left || l > right) {
            return INT_MIN;
        }
        if (l <= left && right <= r) {
            return tree[node];
        }
        int mid = (left + right) / 2;
        int left_max = query_max(2 * node + 1, left, mid, l, r);
        int right_max = query_max(2 * node + 2, mid + 1, right, l, r);
        return max(left_max, right_max);
    }

    void update(int l, int r, int add) {
        update_range(0, 0, size - 1, l - 1, r - 1, add);
    }

    int get_max(int l, int r) {
        return query_max(0, 0, size - 1, l - 1, r - 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> array(N);
    for (int i = 0; i < N; ++i) {
        cin >> array[i];
    }

    SegmentTree st(array);

    int M;
    cin >> M;
    vector<int> results;

    for (int i = 0; i < M; ++i) {
        char type;
        cin >> type;
        if (type == 'm') {
            int l, r;
            cin >> l >> r;
            results.push_back(st.get_max(l, r));
        } else if (type == 'a') {
            int l, r, add;
            cin >> l >> r >> add;
            st.update(l, r, add);
        }
    }

    for (int res : results) {
        cout << res << " ";
    }
    cout << endl;

    return 0;
}
