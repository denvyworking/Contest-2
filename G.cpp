#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Node {
    int max_seq;
    int prefix;
    int suffix;
    int total;
};

class SegmentTree {
private:
    vector<Node> tree;
    int size;
    int original_size;

    Node combine(const Node& left, const Node& right) {
        Node res;
        res.total = left.total + right.total;
         
        int middle_seq = left.suffix + right.prefix;
        res.max_seq = max({left.max_seq, right.max_seq, middle_seq});
        res.prefix = (left.prefix== left.total) ? left.total + right.prefix : left.prefix;
        res.suffix = (right.suffix == right.total) ? right.total + left.suffix : right.suffix;
        
        return res;
    }

    void build(const vector<int>& arr, int node, int l, int r) {
        if (l == r) {
            if (l < arr.size()) {
                int val = (arr[l] == 0) ? 1 : 0;
                tree[node] = {val, val, val, 1};
            } else {
                tree[node] = {0, 0, 0, 1};
            }
            return;
        }
        int mid = (l + r) / 2;
        build(arr, 2*node+1, l, mid);
        build(arr, 2*node+2, mid+1, r);
        tree[node] = combine(tree[2*node+1], tree[2*node+2]);
    }
    
    Node query(int node, int node_l, int node_r, int ql, int qr) {
        if (ql <= node_l && node_r <= qr) {
            return tree[node];
        }
        
        int mid = (node_l + node_r) / 2;
        if (qr <= mid) {
            return query(2*node+1, node_l, mid, ql, qr);
        }
        if (ql > mid) {
            return query(2*node+2, mid+1, node_r, ql, qr);
        }
        
        Node left = query(2*node+1, node_l, mid, ql, qr);
        Node right = query(2*node+2, mid+1, node_r, ql, qr);
        return combine(left, right);
    }
    
    void update_value(int node, int l, int r, int pos, int new_val) {
        if (l == r) {
            int val = (new_val == 0) ? 1 : 0;
            tree[node] = {val, val, val, 1};
            return;
        }
        
        int mid = (l + r) / 2;
        if (pos <= mid) {
            update_value(2*node+1, l, mid, pos, new_val);
        } else {
            update_value(2*node+2, mid+1, r, pos, new_val);
        }
        
        tree[node] = combine(tree[2*node+1], tree[2*node+2]);
    }

public:
    SegmentTree(const vector<int>& arr) : original_size(arr.size()) {
        size = 1;
        while (size < arr.size()) size <<= 1;
        tree.resize(2 * size - 1);
        build(arr, 0, 0, size - 1);
    }
    
    int query_max_zeros(int l, int r) {
        Node res = query(0, 0, size - 1, l, r);
        return res.max_seq;
    }
    
    void update(int pos, int new_val) {
        update_value(0, 0, size - 1, pos, new_val);
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
    
    int m;
    cin >> m;
    vector<int> results;
    
    while (m--) {
        string type;
        cin >> type;
        if (type == "QUERY") {
            int l, r;
            cin >> l >> r;
            results.push_back(st.query_max_zeros(l-1, r-1));
        } else if (type == "UPDATE") {
            int pos, new_val;
            cin >> pos >> new_val;
            st.update(pos-1, new_val);
        }
    }
    
    for (int res : results) {
        cout << res << '\n';
    }
    
    return 0;
}
 
 
