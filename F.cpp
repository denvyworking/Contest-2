#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Node {
    int max_val;
    int ind;
};

class SegmentTree {
private:
    vector<Node> tree;
    int size;
    int original_size;
    
    Node combine(const Node& a, const Node& b) {
        if (a.max_val > b.max_val) return a;
        else return b;
    }
    
    void build(const vector<int>& arr, int node, int l, int r) {
        if (l == r) {
            tree[node] = l < arr.size() ? Node{arr[l], l} : Node{INT_MIN, -1};
            return;
        }
        int mid = (l + r) / 2;
        build(arr, 2*node+1, l, mid);
        build(arr, 2*node+2, mid+1, r);
        tree[node] = combine(tree[2*node+1], tree[2*node+2]);
    }
    
    int query_first_greater_or_equal(int node, int l, int r, int start, int x) {
        if (r < start) return -1;
        if (tree[node].max_val < x) return -1;
        if (l == r) return l + 1;

        int mid = (l + r) / 2;
        int left_result = query_first_greater_or_equal(2 * node + 1, l, mid, start, x);
        if (left_result != -1) return left_result;
        return query_first_greater_or_equal(2 * node + 2, mid + 1, r, start, x);
    }

    void update_value(int index, int new_value){
        int ind_tree = ((size - 1) + index - 1);
        tree[ind_tree].max_val = new_value;
        ind_tree = (ind_tree - 1) / 2;
        while(ind_tree > 0){
            tree[ind_tree].max_val = max(tree[2*ind_tree + 1].max_val, tree[2 * ind_tree + 2].max_val);
            ind_tree = (ind_tree - 1) / 2;
        }
    }

    public:
    SegmentTree(const vector<int>& arr) : original_size(arr.size()) {
        size = 1;
        while (size < arr.size()) size <<= 1;
        tree.resize(2 * size);
        build(arr, 0, 0, size - 1);
    }

    void set(int pos, int x) {
        update_value(pos, x);
    }

    int get(int i, int x) {
        return query_first_greater_or_equal(0, 0, size - 1, i - 1, x);
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    SegmentTree st(arr);

    vector<int> result;
    while (m--) {
        int t, i, x;
        cin >> t >> i >> x;
        if (t == 0) {
            st.set(i, x);
        } else {
            result.push_back(st.get(i, x));
        }
    }
    for(int res : result){
        cout << res << endl;
    }

    return 0;
}
