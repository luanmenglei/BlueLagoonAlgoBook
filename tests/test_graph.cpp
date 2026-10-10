#include "Test.h"
#include "../content/graph/SCC.h"
#include "../content/graph/TwoSat.h"
#include "../content/graph/BCC.h"
#include "../content/graph/EulerWalk.h"
#include "../content/graph/ShortestPath.h"
#include "../content/graph/DirectedMST.h"
#include "../content/graph/Dominator.h"
#include "../content/graph/GlobalMinCut.h"
#include "../content/graph/TreeIsomorphism.h"
#include "../content/graph/DynamicConnectivity.h"

int main() {
 for(int it=0;it<500;it++) {
  int n=ri(1,9); SCC s(n); Dominator d(n);
  vector<vector<bool>> reach(n,vector<bool>(n));
  for(int u=0;u<n;u++) { reach[u][u]=true; for(int v=0;v<n;v++) if(ri(0,4)==0) s.g[u].push_back(v),d.g[u].push_back(v),reach[u][v]=true; }
  for(int k=0;k<n;k++) for(int u=0;u<n;u++) for(int v=0;v<n;v++) reach[u][v]=reach[u][v] || (reach[u][k] && reach[k][v]);
  s.solve(); for(int u=0;u<n;u++) for(int v=0;v<n;v++) CHECK((s.comp[u]==s.comp[v])==(reach[u][v] && reach[v][u]));
  int root=ri(0,n-1); auto idom=d.solve(root);
  vector<vector<bool>> dom(n,vector<bool>(n));
  for(int ban=0;ban<n;ban++) {
   vector<bool> vis(n); queue<int> q; if(root!=ban) vis[root]=true,q.push(root);
   while(!q.empty()) { int u=q.front();q.pop(); for(int v:d.g[u]) if(v!=ban && !vis[v]) vis[v]=true,q.push(v); }
   for(int v=0;v<n;v++) if(reach[root][v]) dom[v][ban]=!vis[v];
  }
  for(int v=0;v<n;v++) {
   if(!reach[root][v]) { CHECK(idom[v]==-1); continue; }
   if(v==root) { CHECK(idom[v]==root); continue; }
   int best=-1,depth=-1;
   for(int u=0;u<n;u++) if(u!=v && dom[v][u]) { int z=count(dom[u].begin(),dom[u].end(),true); if(z>depth) depth=z,best=u; }
   CHECK(idom[v]==best);
  }
 }
 for(int it=0;it<1000;it++) {
  int n=ri(1,8),m=ri(0,20); TwoSat s(n); vector<pair<int,int>> clauses;
  for(int i=0;i<m;i++) { int a=ri(0,2*n-1),b=ri(0,2*n-1); s.either(a,b); clauses.push_back({a,b}); }
  auto valid=[&](int mask) { for(auto [a,b]:clauses) if((bool)(mask>>(a/2)&1)!=(bool)(a&1) && (bool)(mask>>(b/2)&1)!=(bool)(b&1)) return false; return true; };
  bool brute=false; for(int mask=0;mask<(1<<n);mask++) brute|=valid(mask);
  CHECK(s.solve()==brute); if(brute) { int mask=0;for(int i=0;i<n;i++) mask|=int(s.ans[i])<<i; CHECK(valid(mask)); }
 }
 for(int it=0;it<300;it++) {
  int n=ri(1,8); BCC b(n);
  for(int u=0;u<n;u++) for(int v=u+1;v<n;v++) for(int k=0;k<2;k++) if(ri(0,5)==0) b.add(u,v);
  auto comp=[&](int banv,int bane) { vector<int> c(n,-1);int z=0; for(int s=0;s<n;s++) if(s!=banv && c[s]==-1) {
   queue<int> q;q.push(s);c[s]=z++; while(!q.empty()){int u=q.front();q.pop();for(auto [v,id]:b.g[u]) if(v!=banv && id!=bane && c[v]==-1)c[v]=c[u],q.push(v);}
  } return pair<int,vector<int>>{z,c}; };
  int base=comp(-1,-1).first; b.solve();
  for(int i=0;i<(int)b.ed.size();i++) CHECK((bool)b.bridge[i]==(comp(-1,i).first>base));
  for(int u=0;u<n;u++) CHECK((bool)b.cut[u]==(comp(u,-1).first>base));
  auto tree=b.block_tree();
  vector<int> p(tree.size());iota(p.begin(),p.end(),0);
  function<int(int)> find=[&](int x){return p[x]==x?x:p[x]=find(p[x]);};
  for(int u=0;u<(int)tree.size();u++) for(int v:tree[u]) if(u<v){CHECK(find(u)!=find(v));p[find(u)]=find(v);}
  for(int i=0;i<(int)b.ed.size();i++){auto [u,v]=b.ed[i];CHECK((b.ec[u]!=b.ec[v])==(bool)b.bridge[i]);}
 }
 for(int it=0;it<300;it++) {
  int n=ri(1,7); vector<vector<pair<int,i64>>> g(n);vector<tuple<int,int,i64>> ed;
  for(int u=0;u<n;u++) for(int v=0;v<n;v++) if(ri(0,4)==0) {int w=ri(0,20);g[u].push_back({v,w});ed.push_back({u,v,w});}
  int s=ri(0,n-1);auto d=dijkstra(g,s);vector<i64> a(n,LLONG_MAX/4);a[s]=0;
  for(int k=0;k<n;k++)for(auto [u,v,w]:ed)if(a[u]!=LLONG_MAX/4)a[v]=min(a[v],a[u]+w);
  CHECK(a==d);
  for(auto &[u,v,w]:ed) w=ri(-10,10);
  vector<vector<i64>> fw(n,vector<i64>(n,100000));for(int i=0;i<n;i++)fw[i][i]=0;
  for(auto [u,v,w]:ed)fw[u][v]=min(fw[u][v],w);
  for(int k=0;k<n;k++)for(int u=0;u<n;u++)for(int v=0;v<n;v++)if(fw[u][k]!=100000 && fw[k][v]!=100000)fw[u][v]=min(fw[u][v],fw[u][k]+fw[k][v]);
  bool neg=false;for(int i=0;i<n;i++)neg|=fw[i][i]<0;auto cyc=negative_cycle(n,ed);CHECK(neg==!cyc.empty());
  if(neg){i64 w=0;for(int i=0;i<(int)cyc.size();i++){int u=cyc[i],v=cyc[(i+1)%cyc.size()];i64 z=LLONG_MAX;for(auto [x,y,c]:ed)if(x==u && y==v)z=min(z,c);CHECK(z!=LLONG_MAX);w+=z;}CHECK(w<0);}
 }
 for(int it=0;it<300;it++) {
  int n=ri(2,7);vector<vector<i64>> a(n,vector<i64>(n));
  for(int u=0;u<n;u++)for(int v=0;v<u;v++)a[u][v]=a[v][u]=ri(0,50);
  i64 ans=LLONG_MAX;for(int mask=1;mask<(1<<n)-1;mask++){i64 z=0;for(int u=0;u<n;u++)for(int v=0;v<u;v++)if((mask>>u&1)!=(mask>>v&1))z+=a[u][v];ans=min(ans,z);}
  auto [got,side]=min_cut(a);CHECK(got==ans);CHECK(!side.empty() && side.size()<a.size());
 }
 for(int it=0;it<300;it++) {
  int n=ri(2,5),root=ri(0,n-1);vector<DEdge> ed;vector<vector<int>> incoming(n);
  for(int u=0;u<n;u++)for(int v=0;v<n;v++)if(u!=v && ri(0,2)==0){incoming[v].push_back(ed.size());ed.push_back({u,v,ri(-10,20)});}
  i64 best=LLONG_MAX;vector<int> choice(n,-1);
  auto dfs=[&](auto self,int u,i64 val)->void {if(u==n){vector<bool>vis(n);vis[root]=true;for(int k=0;k<n;k++)for(int v=0;v<n;v++)if(v!=root && vis[ed[choice[v]].u])vis[v]=true;if(count(vis.begin(),vis.end(),true)==n)best=min(best,val);return;}if(u==root){self(self,u+1,val);return;}for(int id:incoming[u])choice[u]=id,self(self,u+1,val+ed[id].w);};
  dfs(dfs,0,0);auto got=directed_mst(n,root,ed);CHECK(bool(got)==(best!=LLONG_MAX));if(got)CHECK(*got==best);
 }
 for(int it=0;it<300;it++)for(bool directed:{false,true}) {
  int n=ri(1,10),m=ri(0,30),u=0;vector<vector<pair<int,int>>>g(n);map<pair<int,int>,int> counts;
  for(int i=0;i<m;i++){int v=ri(0,n-1);g[u].push_back({v,i});if(!directed)g[v].push_back({u,i});counts[directed?pair{u,v}:pair<int,int>(minmax(u,v))]++;u=v;}
  auto path=eulerWalk(g,m,0);CHECK(path.size()==(size_t)m+1);
  for(int i=1;i<(int)path.size();i++){pair<int,int> e=directed?pair{path[i-1],path[i]}:pair<int,int>(minmax(path[i-1],path[i]));CHECK(counts[e]>0);counts[e]--;}
  for(auto [e,c]:counts)CHECK(c==0);
 }
 for(int it=0;it<100;it++) {
  int n=ri(1,30);vector<vector<int>>a(n),b(n);vector<int>perm(n);iota(perm.begin(),perm.end(),0);shuffle(perm.begin(),perm.end(),gen);
  for(int i=1;i<n;i++){int p=ri(0,i-1);a[i].push_back(p);a[p].push_back(i);b[perm[i]].push_back(perm[p]);b[perm[p]].push_back(perm[i]);}CHECK(tree_iso(a,b));
 }
 for(int it=0;it<100;it++) {
  int n=ri(1,10),t=20;DynamicConnectivity d(n,t);vector<vector<pair<int,int>>>active(t);
  for(int j=0;j<30;j++){int l=ri(0,t-1),r=ri(l+1,t),u=ri(0,n-1),v=ri(0,n-1);d.add(1,0,t,l,r,{u,v});for(int k=l;k<r;k++)active[k].push_back({u,v});}
  auto f=[&](int tm,RollbackUF &dsu){vector<vector<bool>>reach(n,vector<bool>(n));for(int i=0;i<n;i++)reach[i][i]=true;for(auto [u,v]:active[tm])reach[u][v]=reach[v][u]=true;for(int k=0;k<n;k++)for(int u=0;u<n;u++)for(int v=0;v<n;v++)reach[u][v]=reach[u][v]||(reach[u][k]&&reach[k][v]);for(int u=0;u<n;u++)for(int v=0;v<n;v++)CHECK((dsu.find(u)==dsu.find(v))==reach[u][v]);};d.solve(1,0,t,f);
 }
 done("graph");
}
