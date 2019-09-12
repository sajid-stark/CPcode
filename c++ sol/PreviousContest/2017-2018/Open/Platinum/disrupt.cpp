#include <iostream>
#include <cstdio>
#include <algorithm>
#include <set>
#include <vector>
using namespace std;
#define endl '\n'
#define pi pair<int, int>

const int inf = 1000000007;
struct segTree{
	int l, r;
	segTree *left, *right;
	int val = inf;
	
	segTree(int a, int b){
		l = a;
		r = b;
		
		if(l != r){
			int mid = (l + r) / 2;
			left = new segTree(l, mid);
			right = new segTree(mid + 1, r);
		}
	}
	
	void add(int x, int v){
		if(x < l || r < x) return;
		if(l == r){
			val = v;
			return;
		}
		
		left->add(x, v);
		right->add(x, v);
		val = min(left->val, right->val);
	}
	
	int qry(int a, int b){
		if(r < a || b < l) return inf;
		if(a <= l && r <= b) return val;
		
		return min(left->qry(a, b), right->qry(a, b));
	}
};

const int maxn = 50000;
int n, m;
int l[maxn], r[maxn];
int a[maxn], b[maxn], w[maxn];
multiset<int> s[maxn];
vector<pi> graph[maxn];
segTree *tre;
int ret[maxn];

bool visited[maxn], used[maxn];
int rnk[maxn], prv[maxn], par[maxn], p[maxn];
vector<int> add[maxn], del[maxn];
vector<pi> query[maxn];
 
int find(int x){
	return x == par[x] ? x : par[x] = find(par[x]);
}
 
void unionf(int x, int y){
	x = find(x), y = find(y);
	if(x == y) return;
	if(rnk[x] < rnk[y]) swap(x, y);
	if(rnk[x] == rnk[y]) rnk[x]++;
	par[y] = x;
}
 
int dfs1(int c){
	visited[c] = 1;
	par[c] = prv[c] = c;
	r[c] = l[c];
	
	for(pi i : graph[c]){
		if(i.first == p[c]) continue;
		p[i.first] = c;
		l[i.first] = r[c] + 1;
		r[c] = dfs1(i.first);
		unionf(i.first, c);
		prv[find(c)] = c;
	}
	
	for(pi i : query[c]){
		if(visited[i.first] && !used[i.second]){
			int x = prv[find(i.first)];
			del[x].push_back(i.second);
			used[i.second] = 1;
		}
	}
	
	return r[c];
}
 
void dfs2(int c){
	for(pi i : graph[c]){
    if(i.first == p[c]) continue;
		dfs2(i.first);
		ret[i.second] = tre->qry(l[i.first], r[i.first]);
		if(ret[i.second] == inf) ret[i.second] = -1;
	}
  
	for(int i : add[c]){
		s[c].insert(w[i]);
		tre->add(l[c], *s[c].begin());
	}
	
	for(int i : del[c]){
		s[a[i]].erase(s[a[i]].find(w[i]));
		s[b[i]].erase(s[b[i]].find(w[i]));
		
		tre->add(l[a[i]], s[a[i]].empty() ? inf : *s[a[i]].begin());
		tre->add(l[b[i]], s[b[i]].empty() ? inf : *s[b[i]].begin());
	}
}

int main(){
	freopen("disrupt.in", "r", stdin);
	freopen("disrupt.out", "w", stdout);
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	cin >> n >> m;
	
	tre = new segTree(0, n - 1);
	
	for(int i = 0; i < n; i++){
		par[i] = i;
		if(i < n - 1){
			int x, y;
			cin >> x >> y;
			x--, y--;
			graph[x].push_back({y, i});
			graph[y].push_back({x, i});
		}
	}
	
	for(int i = 0; i < m; i++){
		cin >> a[i] >> b[i] >> w[i];
		a[i]--, b[i]--;
		
		add[a[i]].push_back(i);
		add[b[i]].push_back(i);
		
		query[a[i]].push_back({b[i], i});
		query[b[i]].push_back({a[i], i});
	}
	
	dfs1(0);
	dfs2(0);
	
	for(int i = 0; i < n - 1; i++) cout << ret[i] << endl;

	return 0;
}