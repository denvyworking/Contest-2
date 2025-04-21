#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int BASE = 911382629;
const int MOD = 1e9 + 7;

vector<long long> pow_base;
vector<long long> inv_pow;

long long binpow(long long a, long long b) {
    long long res = 1;
    while (b > 0) {
        if (b & 1)
            res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

void init_pow_base(int max_len) {
    pow_base.resize(max_len + 1);
    pow_base[0] = 1;
    for (int i = 1; i <= max_len; ++i) {
        pow_base[i] = (pow_base[i - 1] * BASE) % MOD;
    }
    long long inv_base_minus_1 = binpow(BASE - 1, MOD - 2);
    inv_pow.resize(max_len + 1);
    inv_pow[0] = 1;
    for (int i = 1; i <= max_len; ++i) {
        inv_pow[i] = (inv_pow[i - 1] * inv_base_minus_1) % MOD;
    }
}

struct SegmentTree {
    struct Node {
        long long hash = 0;
        int value = 0;
        bool has_promise = false;
    };

    vector<Node> tree;
    vector<int> arr;
    int size;

    SegmentTree(const vector<int>& data) : arr(data) {
        size = 1;
        while (size < arr.size()) size <<= 1;
        tree.resize(2 * size);
        build(1, 0, size - 1);
    }
    void build(int v, int tl, int tr) {
        if (tl == tr) {
            if (tl < arr.size()) {
                tree[v].hash = arr[tl];
                tree[v].value = arr[tl];
            }
            return;
        }
        int tm = (tl + tr) / 2;
        build(2 * v, tl, tm);
        build(2 * v + 1, tm + 1, tr);
        tree[v].hash = (tree[2 * v].hash * pow_base[tr - tm] + tree[2 * v + 1].hash) % MOD;
    }
    void push(int v, int tl, int tr) {
        if (tree[v].has_promise && tl != tr) {
            int tm = (tl + tr) / 2;
            apply(2 * v, tl, tm, tree[v].value);
            apply(2 * v + 1, tm + 1, tr, tree[v].value);
            tree[v].has_promise = false;
        }
    }
    void apply(int v, int l, int r, int value) {
        int len = r - l + 1;
        tree[v].value = value;
        tree[v].hash = (value * (pow_base[len] - 1) % MOD) * inv_pow[1] % MOD;
        tree[v].has_promise = true;
    }

    void range_assign(int l, int r, int value) {
        range_assign(1, 0, size - 1, l - 1, r - 1, value);
    }

    void range_assign(int v, int tl, int tr, int l, int r, int value) {
        if (l > r) return;
        if (l == tl && r == tr) {
            apply(v, tl, tr, value);
            return;
        }
        push(v, tl, tr);
        int tm = (tl + tr) / 2;
        range_assign(2 * v, tl, tm, l, min(r, tm), value);
        range_assign(2 * v + 1, tm + 1, tr, max(l, tm + 1), r, value);
        tree[v].hash = (tree[2 * v].hash * pow_base[tr - tm] + tree[2 * v + 1].hash) % MOD;
    }

    long long get_hash(int l, int r) {
        return get_hash(1, 0, size - 1, l - 1, r - 1);
    }

    long long get_hash(int v, int tl, int tr, int l, int r) {
        if (l > r) return 0;
        if (l == tl && r == tr) {
            return tree[v].hash;
        }
        push(v, tl, tr);
        int tm = (tl + tr) / 2;
        long long left_hash = get_hash(2 * v, tl, tm, l, min(r, tm));
        long long right_hash = get_hash(2 * v + 1, tm + 1, tr, max(l, tm + 1), r);
        return (left_hash * pow_base[max(0, r - tm)] + right_hash) % MOD;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> arr(N);
    for (int i = 0; i < N; ++i) {
        cin >> arr[i];
    }

    init_pow_base(N);

    SegmentTree st(arr);

    int Q;
    cin >> Q;
    string result;

    while (Q--) {
        int T, L, R, K;
        cin >> T >> L >> R >> K;
        if (T == 0) {
            st.range_assign(L, R, K);
        } else {
            long long hash1 = st.get_hash(L, L + K - 1);
            long long hash2 = st.get_hash(R, R + K - 1);
            result += (hash1 == hash2) ? '+' : '-';
        }
    }

    cout << result << endl;

    return 0;
}
