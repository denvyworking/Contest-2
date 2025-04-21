#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Node {
    int max_val;
    int count;
};

class SegmentTree {
private:
    vector<Node> tree;
    int n;
    Node combine(const Node& a, const Node& b) {
        if (a.max_val > b.max_val) {
            return {a.max_val, a.count};
        } else if (a.max_val < b.max_val) {
            return {b.max_val, b.count};
        } else {
            return {a.max_val, a.count + b.count};
        }
    }
    void build(const vector<int>& arr, int node, int l, int r) {
        if (l == r) {
            tree[node] = {arr[l], 1};
            return;
        }
        int mid = (l + r) / 2;
        build(arr, 2 * node + 1, l, mid);
        build(arr, 2 * node + 2, mid + 1, r);
        tree[node] = combine(tree[2 * node + 1], tree[2 * node + 2]);
    }
    Node query(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) {
            return {INT_MIN, 0};
        }
        if (ql <= l && r <= qr) {
            return tree[node];
        }
        int mid = (l + r) / 2;
        Node left_result = query(2 * node + 1, l, mid, ql, qr);
        Node right_result = query(2 * node + 2, mid + 1, r, ql, qr);
        return combine(left_result, right_result);
    }

public:
    SegmentTree(const vector<int>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(arr, 0, 0, n - 1);
    }
    pair<int, int> query(int ql, int qr) {
        Node result = query(0, 0, n - 1, ql, qr);
        return {result.max_val, result.count};
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    SegmentTree st(arr);
    int k;
    cin >> k;
    int t = k;
    vector<pair<int, int>> result;
    while (k--) {
        int l, r;
        cin >> l >> r;
        result.push_back(st.query(l - 1, r - 1));
    }
    for(int i = 0; i < t; ++i){
        cout << result[i].first << " " << result[i].second << "\n";
    }
    return 0;
}
