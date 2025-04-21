#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Node {
    int zero_count;
};

class SegmentTree {
private:
    vector<Node> tree;
    int size;
    int original_size;
    

    /*Базовый случай: когда мы дошли до листа (l == r)

Если это реальный элемент массива (l < arr.size()), проверяем равен ли он нулю

Для дополненных элементов (если размер не степень двойки) считаем нулей 0*/

    void build(const vector<int>& arr, int node, int l, int r) {
        if (l == r) {
            if (l < arr.size()) {
                tree[node].zero_count = (arr[l] == 0) ? 1 : 0;
            } else {
                tree[node].zero_count = 0;
            }
            return;
        }
        int mid = (l + r) / 2;
        build(arr, 2*node+1, l, mid);
        build(arr, 2*node+2, mid+1, r);
        tree[node].zero_count = tree[2*node+1].zero_count + tree[2*node+2].zero_count;
    }
    // функция для поика k-го нуля на отрезке от node_l до node_r
    int find_kth_zero(int node, int node_l, int node_r, int k) {
        if (node_l == node_r) {
            return node_l;
        }
        
        int mid = (node_l + node_r) / 2;
        if (tree[2*node+1].zero_count >= k) {
            return find_kth_zero(2*node+1, node_l, mid, k);
        } else {
            return find_kth_zero(2*node+2, mid+1, node_r, k - tree[2*node+1].zero_count);
        }
    }
    
    int count_zeros(int node, int node_l, int node_r, int ql, int qr) {
        if (qr < node_l || node_r < ql) return 0;
        if (ql <= node_l && node_r <= qr) return tree[node].zero_count;
        
        int mid = (node_l + node_r) / 2;
        return count_zeros(2*node+1, node_l, mid, ql, qr) + 
               count_zeros(2*node+2, mid+1, node_r, ql, qr);
    }
    
    void update_value(int index, int new_value){
        int ind_tree = ((size - 1) + index - 1);
        if(new_value == 0) tree[ind_tree].zero_count = 1;
        else tree[ind_tree].zero_count = 0;
        ind_tree = (ind_tree - 1) / 2;
        while(ind_tree > 0){
            tree[ind_tree].zero_count = tree[2*ind_tree + 1].zero_count + tree[2 * ind_tree + 2].zero_count;
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
    
    int query_kth_zero(int l, int r, int k) {
        int pos = find_kth_zero(0, 0, size - 1, k);
        
        return (pos >= l && pos <= r) ? pos : -1;
    }
    
    void update(int pos, int new_val) {
        update_value(pos, new_val);
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
        char type;
        cin >> type;
        if (type == 's') {
            int l, r, k;
            cin >> l >> r >> k;
            int pos = st.query_kth_zero(l-1, r-1, k);
            results.push_back(pos != -1 ? pos + 1 : -1);
        } else if (type == 'u') {
            int pos, new_val;
            cin >> pos >> new_val;
            st.update(pos, new_val);
        }
    }
    
    for (int res : results) {
        cout << res << ' ';
    }
    
    return 0;
}
