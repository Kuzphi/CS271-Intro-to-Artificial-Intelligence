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
const int                   Maxn=50010,Maxm=2500000,Mo=1e9 + 7,oo=INT_MAX >> 1;
const int sp[4][2] = {{-1,0},{0,1},{1,0},{0,-1}};
int T;
using namespace std;
int mp[9][9];
int use[9][9][10], vis[9][9], cnt[9][9];
string st;
int Put(int x,int y,int num){
	int ans = 0;
	mp[x][y] = num;
	vis[x][y] = 1;
	for (int i =0 ; i < 9 ; i++){
		if (!vis[x][i]){
			if (!use[x][i][num]) cnt[x][i] ++, ans ++;
			use[x][i][num] ++;	
			if (cnt[x][i] == 9) ans = -10000;
		}
		if (!vis[i][y]){
			if (!use[i][y][num]) cnt[i][y] ++, ans ++;
			use[i][y][num] ++;
			if (cnt[i][y] == 9) ans = -10000;
		}
	}
	int xx = x / 3 * 3, yy = y / 3 * 3;
	for (int i = xx; i < xx + 3; i++)
		for (int j = yy; j < yy + 3; j++)
			if (!vis[i][j]){
				if (!use[i][j][num]) cnt[i][j] ++, ans ++;
				use[i][j][num] ++;	
				if (cnt[i][j] == 9) ans = -10000;
			}	
	return ans;
}
void Take(int x, int y){
	int num = mp[x][y];
	int xx = x / 3 * 3, yy = y / 3 * 3;

	for (int i = xx; i < xx + 3; i++)
		for (int j = yy; j < yy + 3; j++){
			if (!vis[i][j]){
				if (1 == use[i][j][num]) cnt[i][j] --;
				use[i][j][num] --;
			}
		}

	for (int i =0 ; i < 9 ; i++){
		if (!vis[x][i]){
			if (1 == use[x][i][num]) cnt[x][i] --;
			use[x][i][num] --;	
		}
		if (!vis[i][y]){
			if (1 == use[i][y][num]) cnt[i][y] --;
			use[i][y][num] --;
		}		
	}
	
	mp[x][y] = vis[x][y] = 0;
}
int dfs(){
	// rep(i,0,8) {
	// 	rep(j,0,8) 
	// 	if (mp[i][j])cout << mp[i][j];else cout <<"."; 
	// 	cout <<" ";
	// 	rep(j,0,8) cout << use[i][j][8];
	// 	cout <<" ";
	// 	rep(j,0,8) cout << cnt[i][j];
	// 	cout << endl;
	// }
	// cout << endl;
	int nx=0, ny=0, mcnt = -1;
	rep(i,0,8) rep(j,0,8){
		if (vis[i][j]) continue;
		if (cnt[i][j] > mcnt){
			mcnt = cnt[i][j];
			nx = i, ny = j;
		}
	}
	// cout << nx << " "<< ny << " "<<cnt[nx][ny] <<  endl;
	// cout << mcnt << endl;
	if(mcnt == -1) return 1;
	vector<pair<int,int> > w;
	rep(num,1,9){
		if (!use[nx][ny][num]){
			// cout<< num << endl;
			int eff = Put(nx,ny,num);
			Take(nx,ny);
			if(eff < 0) continue;
			w.PB(MP(eff,num));
			
		}
	}	
	sort(w.begin(),w.end());
	vrep(i,w){
		Put(nx,ny,w[i].second);
		if (dfs()) return 1;
		Take(nx,ny);
	}
	return 0;
}
int main(){
	// cin >> T;
	while(cin >> st, st != "end"){
		CLR(vis);
		CLR(use);
		CLR(cnt);
		rep(i,0,8)
			rep(j,0,8)
				mp[i][j] = st[i * 9 + j] == '.'?0:st[i * 9 + j] - '0';

		rep(i,0,8)
			rep(j,0,8){
				if(mp[i][j]) {
					vis[i][j] = 1;
					Put(i,j,mp[i][j]);
					// cout << i <<" "<< j << endl;
					// cout <<use[1][2][1] <<endl;
				}
			}
		// rep(i,1,9) cout <<use[1][2][i] << " "; cout << endl;
		// cout << use[1][2][1] << endl;
		if (dfs()){
			rep(i,0,8) rep(j,0,8) cout << mp[i][j]; 
			cout << endl;
		}
		else puts("No Solution");
	}
}