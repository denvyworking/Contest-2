#include <iostream>
#include <vector>
using namespace std;

class SegmentTree {
public:
    int n;
    vector<long long> tree;
    vector<long long> lazy;

    SegmentTree(int size) {
        n = size;
        tree.resize(4 * n, 0);
        lazy.resize(4 * n, 0);
    }

    void build(const vector<int>& data, int node, int start, int end) {
        if (start == end) {
            tree[node] = data[start];
        } else {
            int mid = (start + end) / 2;
            build(data, 2 * node, start, mid);
            build(data, 2 * node + 1, mid + 1, end);
            tree[node] = tree[2 * node] + tree[2 * node + 1];
        }
    }

    void apply_lazy(int node, int start, int end) {
        if (lazy[node] != 0) {
            tree[node] += lazy[node] * (end - start + 1);
            if (start != end) {
                lazy[2 * node] += lazy[node];
                lazy[2 * node + 1] += lazy[node];
            }
            lazy[node] = 0;
        }
    }

    void update_range(int node, int start, int end, int l, int r, long long add) {
        apply_lazy(node, start, end);
        if (start > r || end < l) {
            return;
        }
        if (start >= l && end <= r) {
            tree[node] += add * (end - start + 1);
            if (start != end) {
                lazy[2 * node] += add;
                lazy[2 * node + 1] += add;
            }
            return;
        }
        int mid = (start + end) / 2;
        update_range(2 * node, start, mid, l, r, add);
        update_range(2 * node + 1, mid + 1, end, l, r, add);
        tree[node] = tree[2 * node] + tree[2 * node + 1];
    }

    long long query_point(int node, int start, int end, int idx) {
        apply_lazy(node, start, end);
        if (start == end) {
            return tree[node];
        }
        int mid = (start + end) / 2;
        if (idx <= mid) {
            return query_point(2 * node, start, mid, idx);
        } else {
            return query_point(2 * node + 1, mid + 1, end, idx);
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> data(N);
    for (int i = 0; i < N; ++i) {
        cin >> data[i];
    }

    SegmentTree st(N);
    st.build(data, 1, 0, N - 1);

    int M;
    cin >> M;
    vector<long long> results;
    while (M--) {
        char type;
        cin >> type;
        if (type == 'g') {
            int idx;
            cin >> idx;
            --idx;
            results.push_back(st.query_point(1, 0, N - 1, idx));
        } else if (type == 'a') {
            int l, r;
            long long add;
            cin >> l >> r >> add;
            --l; --r;
            st.update_range(1, 0, N - 1, l, r, add);
        }
    }

    for (size_t i = 0; i < results.size(); ++i) {
        cout << results[i] << (i < results.size() - 1 ? " " : "\n");
    }

    return 0;
}
