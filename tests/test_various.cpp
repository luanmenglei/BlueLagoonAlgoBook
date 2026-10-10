#include "Test.h"
#include "../content/various/DivideDP.h"
#include "../content/various/KnuthDP.h"
#include "../content/various/SMAWK.h"
#include "../content/various/Simpson.h"
#include "../content/various/DeBruijn.h"
int main(){
 for(int it=0;it<400;it++){
  int n=ri(1,70);vector<i64>a(n+1),pre(n+1),old(n+1);for(int i=1;i<=n;i++)a[i]=ri(0,20),pre[i]=pre[i-1]+a[i];
  for(int layer=0;layer<5;layer++){
   auto get=[&](int i,int j){i64 v=pre[i]-pre[j];return old[j]+v*v;};auto got=divide_dp(n,get);vector<i64>w(n+1);
   for(int i=1;i<=n;i++){w[i]=LLONG_MAX;for(int j=0;j<i;j++)w[i]=min(w[i],get(i,j));}CHECK(got==w);old=w;
  }
  vector<vector<i64>>d(n+1,vector<i64>(n+1));auto cost=[&](int l,int r){return pre[r]-pre[l];};
  for(int len=2;len<=n;len++)for(int l=0;l+len<=n;l++){int r=l+len;d[l][r]=LLONG_MAX;for(int k=l+1;k<r;k++)d[l][r]=min(d[l][r],d[l][k]+d[k][r]+cost(l,r));}CHECK(knuth_dp(n,cost)==d[0][n]);
 }
 for(int it=0;it<1000;it++){
  int n=ri(0,70),m=ri(1,100);vector<int>target(n);for(int &x:target)x=ri(-20,m+20);sort(target.begin(),target.end());vector<i64>offset(m);for(auto &x:offset)x=ri(-50,50);
  auto get=[&](int i,int j)->i64 {i64 x=target[i]-j;return x*x+offset[j];};auto got=smawk(n,m,get);
  for(int i=0;i<n;i++){int best=0;for(int j=1;j<m;j++)if(get(i,j)<get(i,best))best=j;CHECK(got[i]==best);}
 }
 CHECK(abs(integrate([](double x){return x*x;},0.,1.)-1./3)<1e-9);
 CHECK(abs(integrate([](double x){return sin(x);},0.,acos(-1.))-2)<1e-8);
 CHECK(abs(integrate([](double x){return exp(x);},0.,1.)-(exp(1.)-1))<1e-8);
 for(int k=1;k<=5;k++)for(int n=1;n<=5;n++){
  auto s=de_bruijn(k,n);int len=1;for(int i=0;i<n;i++)len*=k;CHECK(s.size()==(size_t)len);set<vector<int>>seen;
  for(int i=0;i<len;i++){vector<int>a;for(int j=0;j<n;j++)a.push_back(s[(i+j)%len]);seen.insert(a);}CHECK(seen.size()==(size_t)len);
 }
 done("DP, SMAWK and misc");
}
