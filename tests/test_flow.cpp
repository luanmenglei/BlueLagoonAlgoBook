#include "Test.h"
#include "../content/flow/Dinic.h"
#include "../content/flow/LowerBound.h"
#include "../content/flow/MinCostFlow.h"
#include "../content/flow/GomoryHu.h"
#include "../content/flow/HopcroftKarp.h"
#include "../content/flow/MinimumVertexCover.h"
#include "../content/flow/WeightedMatching.h"
#include "../content/flow/Blossom.h"
#include "../content/flow/BipartiteEdgeColor.h"

int main() {
 for(int it=0;it<500;it++) {
  int n=ri(2,7);Dinic d(n);vector<tuple<int,int,i64>>ed;
  for(int u=0;u<n;u++)for(int v=0;v<n;v++)if(ri(0,4)==0){i64 c=ri(0,20);if(it==0)c<<=40;d.add(u,v,c);ed.push_back({u,v,c});}
  i64 best=LLONG_MAX;for(int s=1;s<(1<<n);s++)if((s&1) && !(s>>(n-1)&1)){i64 z=0;for(auto [u,v,w]:ed)if((s>>u&1)&&!(s>>v&1))z+=w;best=min(best,z);}
  CHECK(d.flow(0,n-1)==best); d.bfs(0,n-1);CHECK(d.dep[n-1]==-1);
 }
 for(int it=0;it<400;it++) {
  int n=ri(2,4),m=ri(0,6);vector<tuple<int,int,int,int>>ed;
  for(int i=0;i<m;i++){int l=ri(0,1),r=ri(l,2);ed.push_back({ri(0,n-1),ri(0,n-1),l,r});}
  bool feasible=false; i64 mn=LLONG_MAX,mx=-1;vector<int>bal(n);
  auto dfs=[&](auto self,int i)->void {if(i==m){bool ok=true;for(int u=1;u<n-1;u++)ok&=bal[u]==0;if(ok && bal[0]>=0 && bal[0]==-bal[n-1])mn=min<i64>(mn,bal[0]),mx=max<i64>(mx,bal[0]);if(all_of(bal.begin(),bal.end(),[](int x){return x==0;}))feasible=true;return;}
   auto [u,v,l,r]=ed[i];for(int z=l;z<=r;z++){bal[u]+=z;bal[v]-=z;self(self,i+1);bal[u]-=z;bal[v]+=z;}};dfs(dfs,0);
  LowerBound a(n),b(n),c(n);for(auto [u,v,l,r]:ed)a.add(u,v,l,r),b.add(u,v,l,r),c.add(u,v,l,r);
  CHECK(a.feasible()==feasible);auto x=b.solve(0,n-1),y=c.solve(0,n-1,false);CHECK(bool(x)==(mx!=-1));CHECK(bool(y)==(mx!=-1));if(x)CHECK(*x==mx && *y==mn);
 }
 for(int it=0;it<400;it++) {
  int n=ri(2,5),lim=ri(0,10);MCF d(n);vector<tuple<int,int,int,int>>ed;
  for(int u=0;u<n;u++)for(int v=u+1;v<n;v++)if(ri(0,2)==0){int c=ri(0,2),w=ri(-10,10);d.add(u,v,c,w);ed.push_back({u,v,c,w});}
  vector<int>bal(n);i64 bf=-1,bc=LLONG_MAX;
  auto dfs=[&](auto self,int i,i64 cost)->void {if(i==(int)ed.size()){for(int u=1;u<n-1;u++)if(bal[u])return;int f=bal[0];if(f<0 || f>lim || f!=-bal[n-1])return;if(f>bf)bf=f,bc=cost;else if(f==bf)bc=min(bc,cost);return;}
   auto [u,v,c,w]=ed[i];for(int z=0;z<=c;z++){bal[u]+=z;bal[v]-=z;self(self,i+1,cost+i64(z)*w);bal[u]-=z;bal[v]+=z;}};dfs(dfs,0,0);
  auto [f,c]=d.flow(0,n-1,lim);CHECK(f==bf && c==bc);
 }
 for(int it=0;it<300;it++) {
  int l=ri(1,7),r=ri(1,7);vector<vector<int>>g(l);vector<pair<int,int>>ed;
  for(int u=0;u<l;u++)for(int v=0;v<r;v++)if(ri(0,2)==0)g[u].push_back(v),ed.push_back({u,v});
  auto dfs=[&](auto self,int u,int mask)->int {if(u==l)return 0;int z=self(self,u+1,mask);for(int v:g[u])if(!(mask>>v&1))z=max(z,1+self(self,u+1,mask|(1<<v)));return z;};
  int best=dfs(dfs,0,0);vector<int>match(r,-1);CHECK(hopcroftKarp(g,match)==best);
  set<int>seen;for(int v=0;v<r;v++)if(match[v]!=-1){CHECK(seen.insert(match[v]).second);CHECK(find(g[match[v]].begin(),g[match[v]].end(),v)!=g[match[v]].end());}
  auto cv=cover(g,l,r);CHECK(cv.size()==(size_t)best);set<int>s(cv.begin(),cv.end());for(auto [u,v]:ed)CHECK(s.count(u)||s.count(l+v));
  for(int j=0;j<5 && !ed.empty();j++)ed.push_back(ed[ri(0,ed.size()-1)]);
  auto col=bipartite_edge_color(l,r,ed);vector<int>deg(l+r);for(auto [u,v]:ed)deg[u]++,deg[l+v]++;int delta=*max_element(deg.begin(),deg.end());
  vector<set<int>>used(l+r);for(int i=0;i<(int)ed.size();i++){auto [u,v]=ed[i];CHECK(0<=col[i] && col[i]<delta);CHECK(used[u].insert(col[i]).second && used[l+v].insert(col[i]).second);}
 }
 for(int it=0;it<500;it++) {
  int n=ri(0,5),m=ri(max(1,n),7);vector<vector<i64>>cost(n,vector<i64>(m));for(auto &r:cost)for(auto &x:r)x=ri(-30,30);
  i64 best=LLONG_MAX;
  auto dfs=[&](auto self,int u,int mask,i64 val)->void {if(u==n){best=min(best,val);return;}for(int v=0;v<m;v++)if(!(mask>>v&1))self(self,u+1,mask|(1<<v),val+cost[u][v]);};dfs(dfs,0,0,0);
  auto [val,mat]=weightedMatching(cost);CHECK(val==best);set<int>seen; i64 z=0;for(int u=0;u<n;u++){CHECK(seen.insert(mat[u]).second && mat[u]>=0 && mat[u]<m);z+=cost[u][mat[u]];}CHECK(z==val);
 }
 for(int it=0;it<700;it++) {
  int n=ri(0,12);auto b=make_unique<Blossom>();b->init(n);vector<vector<bool>>g(n,vector<bool>(n));
  for(int u=0;u<n;u++)for(int v=0;v<u;v++)if(ri(0,2)==0)g[u][v]=g[v][u]=true,b->addEdge(u+1,v+1);
  vector<int>dp(1<<n,-1);auto dfs=[&](auto self,int s)->int {if(!s)return 0;if(dp[s]!=-1)return dp[s];int u=__builtin_ctz(s),z=self(self,s^(1<<u));for(int v=u+1;v<n;v++)if((s>>v&1)&&g[u][v])z=max(z,1+self(self,s^(1<<u)^(1<<v)));return dp[s]=z;};
  int best=dfs(dfs,(1<<n)-1);CHECK(b->Match()==best);int cnt=0;for(int u=1;u<=n;u++)if(b->match[u]){int v=b->match[u];CHECK(b->match[v]==u && g[u-1][v-1]);cnt++;}CHECK(cnt==2*best);
 }
 for(int it=0;it<100;it++) {
  int n=ri(2,7);vector<tuple<int,int,i64>>ed;for(int u=0;u<n;u++)for(int v=0;v<u;v++)if(ri(0,2)==0)ed.push_back({u,v,ri(0,30)});
  auto tree=gomory_hu(n,ed);vector<vector<pair<int,i64>>>g(n);for(auto [u,v,w]:tree)g[u].push_back({v,w}),g[v].push_back({u,w});
  for(int s=0;s<n;s++)for(int t=0;t<s;t++) {
   i64 best=LLONG_MAX;for(int mask=0;mask<(1<<n);mask++)if((mask>>s&1)&&!(mask>>t&1)){i64 z=0;for(auto [u,v,w]:ed)if((mask>>u&1)!=(mask>>v&1))z+=w;best=min(best,z);}
   auto dfs=[&](auto self,int u,int p,i64 val)->i64 {if(u==t)return val;for(auto [v,w]:g[u])if(v!=p){i64 z=self(self,v,u,min(val,w));if(z!=-1)return z;}return -1;};CHECK(dfs(dfs,s,-1,LLONG_MAX)==best);
  }
 }
 done("flow and matching");
}
