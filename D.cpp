#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Node {
    int max_val;
};

class SegmentTree {
private:
    vector<Node> tree;
    int size;
    
    Node combine(const Node& a, const Node& b) {
        if (a.max_val > b.max_val) return a;
        else return b;
    }
    
    void build(const vector<int>& arr, int node, int l, int r) {
        if (l == r) {
            tree[node] = l < arr.size() ? Node{arr[l]} : Node{INT_MIN};
            return;
        }
        int mid = (l + r) / 2;
        build(arr, 2*node+1, l, mid);
        build(arr, 2*node+2, mid+1, r);
        tree[node] = combine(tree[2*node+1], tree[2*node+2]);
    }
    
    Node query(int node, int node_l, int node_r, int ql, int qr) {
        if (qr < node_l || node_r < ql) return {INT_MIN};
        if (ql <= node_l && node_r <= qr) return tree[node];
        
        int mid = (node_l + node_r) / 2;
        return combine(
            query(2*node+1, node_l, mid, ql, qr),
            query(2*node+2, mid+1, node_r, ql, qr)
        );
    }

public:
    SegmentTree(const vector<int>& arr, int extended_size) : size(extended_size) {
        tree.resize(2 * size - 1);
        build(arr, 0, 0, size - 1);
    }
    
    int query(int l, int r) {
        Node result = query(0, 0, size-1, l, r);
        return result.max_val;
    }
    void change_value(int index, int new_value, vector<int> &arr){
        int ind_tree = ((size - 1) + index - 1);
        tree[ind_tree] = {new_value};
        ind_tree = (ind_tree - 1) / 2;
        while(ind_tree > 0){
            tree[ind_tree] = combine(tree[2*ind_tree + 1], tree[2 * ind_tree + 2]);
            ind_tree = (ind_tree - 1) / 2;
        }
    }
};

int next_power_of_two(int n) {
    int power = 1;
    while (power < n) power <<= 1;
    return power;
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }
    int extended_size = next_power_of_two(n);
    SegmentTree st(arr, extended_size);
    int k;
    cin >> k;
    
    vector<int> result;
    while (k--) {
        char str;
        int l, r;
        cin >> str >> l >> r;
        if(str == 's'){
            auto max_val = st.query(l-1, r-1);
            result.push_back(max_val);
        }
        else if(str == 'u'){
            st.change_value(l, r, arr);
        }
    }
    for(auto r : result){
        cout << r << endl;
    }
    
    return 0;
}
