#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct BinaryIndexedTree {
    int n;                      // Size of the array
    vector<long long> values;   // Values of the BIT

    BinaryIndexedTree(int n) : n(n), values(n + 1, 0) {}

    // Add v to the i-th element
    void update(int i, long long v){
        while (i <= n) {
            values[i] += v;
            i += (i & -i);
        }
    }

    // Prefix sum of the i-th element
    long long sum(int i){
        long long s = 0; 
        while (i > 0) {
            s += values[i];
            i -= (i & -i);
        }
        return s;
    }

    // Add v to the range [l,r]
    void rangeUpdate(int l, int r, long long v) {
        update(l,v);
        if(r + 1 <= n) update(r + 1, -v);
    }

    // Get the value of the i-th element (height = 1 + prefix sum)
    long long getH(int i) { 
        return 1 + sum(i);
    }
};

struct Node{
    int prefix, suffix, best, length;
};

struct SegTree {
    int size;
    vector<Node> st;

    SegTree(int n){
        size = n; 
        st.resize(4 * n); 
    }

    Node make_data(int val){
        Node res;
        res.length=1;
        if(val==1) res.prefix=res.suffix=res.best=1;
        else res.prefix=res.suffix=res.best=0;
        return res;
    }

    // Update node by combining two children
    Node combine(const Node &l, const Node &r){
        Node res;
        res.length = l.length + r.length;
        res.prefix = (l.prefix==l.length ? l.prefix + r.prefix : l.prefix);
        res.suffix = (r.suffix==r.length ? r.suffix + l.suffix : r.suffix);
        res.best = max({(long long)l.best, (long long)r.best, (long long)l.suffix + r.prefix});
        return res;
    }

    void build(int idx,int start,int end){
        if(start==end){
            st[idx]=make_data(1); // initially 0>=0 => val=1
            return;
        }
        int mid=(start+end)/2;
        build(idx*2,start,mid);
        build(idx*2+1,mid+1,end);
        st[idx]=combine(st[idx*2],st[idx*2+1]);
    }

    void init(){ if(size>0) build(1,1,size); }

    void pointUpdate(int idx,int start,int end,int pos,int val){
        if(start==end){
            st[idx]=make_data(val);
            return;
        }
        int mid=(start+end)/2;
        if(pos<=mid) pointUpdate(idx*2,start,mid,pos,val);
        else pointUpdate(idx*2+1,mid+1,end,pos,val);
        st[idx]=combine(st[idx*2],st[idx*2+1]);
    }

    void updatePos(int pos,int val){
        pointUpdate(1,1,size,pos,val);
    }

    Node queryRange(int idx,int start,int end,int l,int r){
        if(r<start||end<l) return {0,0,0,0};
        if(l<=start && end<=r) return st[idx];
        int mid=(start+end)/2;
        Node left=queryRange(idx*2,start,mid,l,r);
        Node right=queryRange(idx*2+1,mid+1,end,l,r);
        if(left.length==0) return right;
        if(right.length==0) return left;
        return combine(left,right);
    }

    int getBest(int l,int r){
        if(l>r) return 0;
        Node ans=queryRange(1,1,size,l,r);
        return ans.best;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m; 
    cin >> n >> m;

    // Build BIT for heights:
    BinaryIndexedTree tree(n);

    // Build segment tree for diffs:
    // d[i] = H[i+1]-H[i], i=1..n-1. We'll store Booleans: d[i]>=0 => 1 else 0.
    SegTree seg(n > 1 ? n - 1 : 0);
    if(n > 1) seg.init();

    auto getSign = [&](int i){
        // sign of H[i+1]-H[i]
        long long h1=tree.getH(i), h2=tree.getH(i+1);
        return (h2>=h1) ? 1 : 0;
    };

    // Initially all heights=1 => differences=0 => sign=1 (if n>1).
    // seg.init() sets them all to 1.

    while(m--){
        char c; cin>>c;
        if(c=='N'){ // N a b k
            int a,b; long long k;
            cin>>a>>b>>k;
            tree.rangeUpdate(a,b,k);
            // update boundary diffs if exist
            if(a>1){
                int s = getSign(a-1);
                seg.updatePos(a-1,s);
            }
            if(b<n){
                int s = getSign(b);
                seg.updatePos(b,s);
            }
        } else {
            int a,b; cin>>a>>b;
            if(a==b) cout<<1<<"\n";
            else {
                int ans=seg.getBest(a,b-1)+1;
                cout<<ans<<"\n";
            }
        }
    }
    return 0;
}