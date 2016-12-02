#include <math.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <map>
#include <set>
#include <queue>
#include <limits.h>
#include <algorithm>
#include <sstream>
#define X first 
#define Y second 
#define MP make_pair
#define PB push_back
#define ll long long
#define CLR(x) memset(x,0,sizeof(x))
#define vrep(i, v)    for(int i = 0; i < v.size(); i++)
#define rep(i, a, b)  for(int i = a; i <= b; i++)
#define drep(i, a, b) for(int i = a; i >= b; i--)
using namespace std;
const double pi = acos(-1.), eps = 1e-6;
const int                   Maxn=5010,Maxm=2500000,Mo=1e9 + 7,oo=INT_MAX >> 1;
const int sp[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
int T;
using namespace std;

struct DanceLink{
	struct Node{
		int x,y,cnt;
		Node *l, *r, *u, *d;
		void Init(int _x, int _y){
			x = _x, y = _y;
			cnt = 0;
			l = r = u = d = NULL;
		}
		Node(){}
	}node[Maxn];
	Node *c[Maxn], *Head;
	struct Data{
		int x, y;
		Data(){}
		Data(int x, int y):x(x),y(y){}
		bool operator <  (const Data& a)const{
			return x == a.x? y <a.y : x < a.x;
		}
	}v[Maxn];
	int n, m, cnt, vcnt, ans[Maxn];
	void Init(int _n, int _m){
		n = _n, m = _m;
		vcnt = ans[0] = cnt = 0;
	}
	void add(int x,int y){//0-index
		v[vcnt++] = Data(x,y);
	}
	Node* NewNode(int x, int y){
		node[cnt].Init(x,y);
		return node + (cnt ++);
	}
	void Build(){
		Head = NewNode(0,0);
		Node *pre = Head;
		for (int i = 0; i < m; i++){//0-index
			c[i] = NewNode(0,0); 
			pre -> r = c[i];
			c[i] -> l = pre;
			pre = c[i];
			c[i]->u = c[i]->d = c[i];
		}
		pre  -> r = Head;
		Head -> l = pre;
		// sort(v, v + vcnt); //if input is disored, sort it
		for (int i = 0;i < vcnt; i++){
			int x = v[i].x, y = v[i].y;
			Node* vi = NewNode(x,y);
			c[y] -> u -> d = vi;
			vi -> u = c[y]->u;
			vi -> d = c[y];
			c[y] -> u = vi;
			c[y] -> cnt ++;
			if (i == 0 || x != v[i-1].x)
				vi -> l = vi -> r = vi;
			else{
				vi -> l = pre;
				vi -> r = pre -> r;
				pre -> r -> l = vi;
				pre -> r = vi;
			}
			pre = vi;
		}
	}
	void Remove(Node* col){
		col -> l -> r = col -> r;
		col -> r -> l = col -> l;
		for (Node *i = col -> d; i != col; i = i -> d){
			for (Node * j = i -> r; j != i; j = j -> r){
				j -> u -> d = j -> d;
				j -> d -> u = j -> u;
				c[j->y]->cnt --;
			}
		}

	}
	void Restore(Node* col){
		col->l -> r = col -> r ->l = col;
		for (Node *i = col -> u; i != col; i = i -> u){
			for (Node * j = i -> l; j != i; j = j -> l){
				j -> u -> d = j -> d -> u = j;
				c[j->y]->cnt ++;
			}
		}
	}
	bool Solve(){
		if (Head -> r == Head)
			return 1;
		Node *min = NULL;
		for (Node* i = Head->r; i!=Head; i = i -> r)
			if (min == NULL|| i -> cnt < min -> cnt)
				min = i;
		if(min->cnt <= 0) return 0;
		Remove(min);
		for (Node* i = min -> u; i!= min; i = i->u){
			ans[++ans[0]] = i -> x;			
			for(Node* j = i->r; j != i; j = j -> r)
				Remove(c[j->y]);
			if (Solve()) return 1;
			for(Node* j = i->l; j != i; j = j -> l)
				Restore(c[j->y]);
			ans[0] --;
		}
		Restore(min);
		return 0;
	}	
};
struct Sudoku{
	DanceLink DLX;
	Sudoku(){Init();}
	void Init(){DLX.Init(729,324);}
	void Build(int mp[][9]){
		int k;
		rep(i,0,8) rep(j,0,8){			
			int id = i * 9 + j;
			if(mp[i][j]){
				k = mp[i][j] - 1;
				int line = id * 9 + k;
				DLX.add(line, id);
				DLX.add(line, 81  + i * 9 + k);
				DLX.add(line, 162 + j * 9 + k);
				DLX.add(line, 243 + (i / 3 * 3 + j / 3) * 9 + k);
			}			
			else{
				rep(k,0,8) {
					int line = id * 9 + k;
					DLX.add(line, id);
					DLX.add(line, 81  + i * 9 + k);
					DLX.add(line, 162 + j * 9 + k);
					DLX.add(line, 243 + (i / 3 * 3 + j / 3) * 9 + k);
				}
			}
		}
		DLX.Build();
	}
	bool Solve(int mp[][9]){
		if (!DLX.Solve()) return 0;
		rep(i,1,DLX.ans[0]){
			int num = DLX.ans[i] % 9;
			DLX.ans[i] /= 9;
			int x = DLX.ans[i] / 9, y = DLX.ans[i] % 9;
			mp[x][y] = num + 1;
		}
		return 1;
	}
	
}sudoku;
int mp[9][9];
string st;
int main(){
	while(cin >> st, st != "end"){	
		rep(i,0,8) rep(j,0,8)
			mp[i][j] = st[i * 9 + j] == '.'?0:st[i * 9 + j] - '0';
		sudoku.Init();
		sudoku.Build(mp);
		if (!sudoku.Solve(mp))
			printf("impossible");
		else 
			rep(i,0,8) rep(j,0,8) cout << mp[i][j];
		cout << endl;
	}
}