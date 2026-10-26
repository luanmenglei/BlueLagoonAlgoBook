#include "Test.h"
#include "../content/contest/Int128.h"
#include "../content/contest/OrderedSet.h"
#include "../content/contest/RandomHash.h"
#include "../content/data-structures/Fenwick.h"
#include "../content/data-structures/SegmentTree.h"
#include "../content/data-structures/SparseTable.h"
#include "../content/data-structures/Cartesian.h"
#include "../content/data-structures/Persistent.h"
#include "../content/data-structures/DSU.h"
#include "../content/data-structures/UnionFindRollback.h"
#include "../content/data-structures/Treap.h"
#include "../content/data-structures/VirtualTree.h"
#include "../content/data-structures/HLD.h"
#include "../content/data-structures/Centroid.h"
#include "../content/data-structures/LCT.h"
#include "../content/data-structures/LiChao.h"
#include "../content/data-structures/LineContainer.h"

int main() {
 for(int a=-50;a<=50;a++) for(int b=-13;b<=13;b++) if(b) {
  CHECK(floor_div(a,b)==(i64)floor((double)a/b));
  CHECK(ceil_div(a,b)==(i64)ceil((double)a/b));
 }
 for(string s:{"0","-123456789012345678901234567890","170141183460469231731687303715884105727","-170141183460469231731687303715884105728"}) {
  stringstream in(s),out; i128 x; in>>x; out<<x; CHECK(out.str()==s);
 }
 OrderedSet<pair<int,int>> os;
 os.insert({2,0}); os.insert({2,1}); os.insert({5,2});
 CHECK(os.order_of_key({2,-1})==0 && os.order_of_key({3,0})==2);
 CHECK(os.find_by_order(2)->first==5); CHECK(Hash{}(17)==Hash{}(17));
 for(int it=0;it<100;it++) {
  int n=ri(1,40); vector<i64> a(n+1); Fenwick f(n); SegTree s(n);
  for(int t=0;t<200;t++) {
   int l=ri(1,n),r=ri(l,n),v=ri(0,20);
   if(ri(0,1)) {
    bool set=ri(0,1); s.update(1,1,n,l,r,v,set);
    for(int i=l;i<=r;i++) { i64 x=set?v:a[i]+v; f.add(i,x-a[i]); a[i]=x; }
   } else {
    i64 ans=accumulate(a.begin()+l,a.begin()+r+1,0LL);
    CHECK(s.query(1,1,n,l,r)==ans && f.sum(r)-f.sum(l-1)==ans);
   }
   i64 tot=f.sum(n); if(tot) {
    i64 k=ri(1,min<i64>(tot,INT_MAX)); int j=1; i64 z=k;
    while(z>a[j]) z-=a[j++]; CHECK(f.kth(k)==j);
   }
  }
  vector<int> b(n); for(int &x:b) x=ri(-20,20); ST st(b);
  auto [root,ch]=cartesian(b); vector<int> seq;
  auto dfs=[&](auto self,int u)->void { if(u==-1) return; self(self,ch[u][0]); seq.push_back(u); self(self,ch[u][1]); for(int v:ch[u]) if(v!=-1) CHECK(b[u]<=b[v]); };
  dfs(dfs,root); for(int i=0;i<n;i++) CHECK(seq[i]==i);
  for(int t=0;t<50;t++) { int l=ri(0,n-1),r=ri(l,n-1); CHECK(st.query(l,r)==*min_element(b.begin()+l,b.begin()+r+1)); }
  Persistent p; vector<int> roots(n+1),v(n);
  for(int i=0;i<n;i++) v[i]=ri(1,50),roots[i+1]=p.add(roots[i],1,50,v[i]);
  for(int t=0;t<50;t++) { int l=ri(0,n-1),r=ri(l+1,n),k=ri(1,r-l); vector<int> z(v.begin()+l,v.begin()+r); sort(z.begin(),z.end()); CHECK(p.kth(roots[l],roots[r],1,50,k)==z[k-1]); }
 }
 for(int it=0;it<100;it++) {
  int n=30; DSU d(n); vector<i64> pot(n); for(auto &x:pot) x=ri(-100,100);
  RollbackUF r(n); vector<pair<int,int>> ed;
  for(int t=0;t<100;t++) { int x=ri(0,n-1),y=ri(0,n-1); CHECK(d.join(x,y,pot[y]-pot[x])); CHECK(d.find(x)==d.find(y)); CHECK(d.d[y]-d.d[x]==pot[y]-pot[x]);
   int tm=r.time(); r.join(x,y); CHECK(r.find(x)==r.find(y)); r.rollback(tm); CHECK(r.time()==tm); r.join(x,y);
  }
  CHECK(!d.join(0,0,1));
 }
 for(int it=0;it<100;it++) {
  Treap t; int root=0; vector<i64> a;
  for(int j=0;j<100;j++) {
   if(a.empty() || ri(0,2)==0) { int p=ri(0,a.size()),v=ri(-50,50); auto [x,y]=t.split(root,p); root=t.merge(t.merge(x,t.node(v)),y); a.insert(a.begin()+p,v); }
   else { int l=ri(0,a.size()-1),r=ri(l+1,a.size()); auto [x,z]=t.split(root,r); auto [y,w]=t.split(x,l); CHECK(t.tr[w].sum==accumulate(a.begin()+l,a.begin()+r,0LL)); t.flip(w); reverse(a.begin()+l,a.begin()+r); root=t.merge(t.merge(y,w),z); }
   CHECK(t.tr[root].sum==accumulate(a.begin(),a.end(),0LL)); CHECK(t.tr[root].sz==(int)a.size());
  }
 }
 for(int it=0;it<100;it++) {
  int n=ri(1,50); HLD h(n); Centroid c(n);
  for(int i=1;i<n;i++) { int p=ri(0,i-1); h.g[p].push_back(i); h.g[i].push_back(p); }
  h.init(); c.g=h.g;
  for(int t=0;t<100;t++) {
   int u=ri(0,n-1),v=ri(0,n-1),x=u,y=v;
   while(h.dep[x]>h.dep[y]) x=h.p[x]; while(h.dep[y]>h.dep[x]) y=h.p[y]; while(x!=y) x=h.p[x],y=h.p[y];
   CHECK(h.lca(u,v)==x); int got=0;
   h.path(u,v,[&](int l,int r){got+=r-l;}); CHECK(got==h.dep[u]+h.dep[v]-2*h.dep[x]+1);
   got=0; h.path(u,v,[&](int l,int r){got+=r-l;},true); CHECK(got==h.dep[u]+h.dep[v]-2*h.dep[x]);
  }
  vector<int> a; for(int i=0;i<n;i++) if(ri(0,1)) a.push_back(i);
  auto ed=virtual_tree(a,h); set<int> verts(a.begin(),a.end()); for(auto [u,v]:ed) { CHECK(h.lca(u,v)==u && u!=v); verts.insert(u); verts.insert(v); }
  if(!a.empty()) CHECK(ed.size()+1==verts.size());
  int processed=0;
  auto work=[&](int u,auto &g,auto &dead) { processed++; vector<int> sizes;
   auto dfs=[&](auto self,int x,int p)->int { int s=1; for(int y:g[x]) if(y!=p && !dead[y]) s+=self(self,y,x); return s; };
   int total=1; for(int v:g[u]) if(!dead[v]) sizes.push_back(dfs(dfs,v,u)),total+=sizes.back();
   for(int s:sizes) CHECK(2*s<=total);
  }; c.solve(0,work); CHECK(processed==n);
 }
 for(int it=0;it<20;it++) {
  int n=20; LCT t(n); vector<set<int>> g(n+1); vector<i64> val(n+1);
  for(int i=1;i<=n;i++) t.set(i,val[i]=ri(0,100));
  auto path=[&](int x,int y) { vector<int> p(n+1,-1); queue<int> q; q.push(x); p[x]=0; while(!q.empty()) { int u=q.front();q.pop(); for(int v:g[u]) if(p[v]==-1) p[v]=u,q.push(v); } vector<int> a; if(p[y]!=-1) for(int u=y;u;u=p[u]) a.push_back(u); return a; };
  for(int j=0;j<500;j++) {
   int x=ri(1,n),y=ri(1,n),op=ri(0,3); auto a=path(x,y);
   if(op==0) { bool ok=t.link(x,y); CHECK(ok==a.empty()); if(ok) g[x].insert(y),g[y].insert(x); }
   else if(op==1) { bool expected=g[x].count(y),ok=t.cut(x,y); CHECK(ok==expected); if(ok) g[x].erase(y),g[y].erase(x); }
   else if(op==2) { t.set(x,val[x]=ri(0,100)); }
   else { CHECK((t.findroot(x)==t.findroot(y))==!a.empty()); if(!a.empty()) { i64 z=0; for(int u:a) z^=val[u]; CHECK(t.query(x,y)==z); } }
  }
 }
 for(int it=0;it<100;it++) {
  vector<i64> xs; for(int x=-25;x<=25;x++) xs.push_back(x);
  LiChao li(xs); LineContainer lc; vector<pair<i64,i64>> ed;
  for(int j=0;j<50;j++) { int k=ri(-100,100),b=ri(-1000,1000); li.add(k,b); lc.add(k,b); ed.push_back({k,b});
   int x=ri(-25,25); i64 mn=LLONG_MAX,mx=LLONG_MIN;
   for(auto [a,b]:ed) mn=min(mn,a*x+b),mx=max(mx,a*x+b);
   CHECK(li.query(x+25,1,0,50)==mn); CHECK(lc.query(x)==mx);
  }
 }
 done("data and contest");
}
