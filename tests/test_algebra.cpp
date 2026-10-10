#include "Test.h"
#include "../content/algebra/NTT.h"
#include "../content/algebra/ArbitraryConvolution.h"
#include "../content/algebra/FPS.h"
#include "../content/algebra/FPSSqrt.h"
#include "../content/algebra/Polynomial.h"
#include "../content/algebra/ConsecutiveInterpolation.h"
#include "../content/algebra/Transforms.h"
#include "../content/algebra/Recurrence.h"
#include "../content/algebra/Gauss.h"
#include "../content/algebra/XorBasis.h"
#include "../content/algebra/MatrixTree.h"
#include "../content/algebra/Simplex.h"
using V=vector<int>;
V naive(V a,V b,int p=998244353){if(a.empty()||b.empty())return {};V c(a.size()+b.size()-1);for(int i=0;i<(int)a.size();i++)for(int j=0;j<(int)b.size();j++)c[i+j]=(c[i+j]+i64(a[i])*b[j])%p;return c;}
int eval(const V &a,int x){i64 z=0;for(int i=(int)a.size()-1;i>=0;i--)z=(z*x+a[i])%poly::mod;return z;}
int main(){
 const int p=poly::mod;
 for(int it=0;it<500;it++){
  V a(ri(0,80)),b(ri(0,80));for(int &x:a)x=ri(0,p-1);for(int &x:b)x=ri(0,p-1);CHECK(poly::conv(a,b)==naive(a,b));
  int mod=ri(1,2000000000);for(int &x:a)x%=mod;for(int &x:b)x%=mod;CHECK(conv_mod(a,b,mod)==naive(a,b,mod));
 }
 for(int k=0;k<=14;k++){V a(1<<k);for(int &x:a)x=ri(0,p-1);V b=a;poly::ntt(b,false);poly::ntt(b,true);CHECK(a==b);}
 for(int it=0;it<300;it++){
  int n=ri(1,70);V a(n);for(int &x:a)x=ri(0,p-1);a[0]=ri(1,p-1);
  V inv=poly::inverse(a,n),c=poly::cut(naive(a,inv),n);CHECK(c[0]==1);for(int i=1;i<n;i++)CHECK(c[i]==0);
  a[0]=1;CHECK(poly::exponential(poly::logarithm(a,n),n)==a);
  V b(n);for(int &x:b)x=ri(0,p-1);b[0]=0;CHECK(poly::logarithm(poly::exponential(b,n),n)==b);
  int e=ri(0,7);a[0]=ri(0,1)?0:ri(1,p-1);if(n>1 && ri(0,1))a[1]=0;
  V want(n);want[0]=1;for(int i=0;i<e;i++)want=poly::cut(naive(want,a),n);CHECK(poly::fps_power(a,e,n)==want);
  for(int &x:b)x=ri(0,p-1);if(ri(0,1))b[0]=0;if(n>1 && ri(0,1))b[1]=0;
  V sq=poly::cut(naive(b,b),n);auto z=poly::fps_sqrt(sq,n);CHECK(bool(z));CHECK(poly::cut(naive(*z,*z),n)==sq);
  a.resize(ri(0,n));b.resize(ri(1,n));b.back()=ri(1,p-1);
  auto [q,r]=poly::divmod(a,b);V res=naive(q,b);res.resize(max(res.size(),r.size()));for(int i=0;i<(int)r.size();i++)res[i]=(res[i]+r[i])%p;poly::trim(res);poly::trim(a);CHECK(res==a && r.size()<b.size());
 }
 CHECK(poly::fps_sqrt({0,1},2)==nullopt);
 int nonsquare=2;while(pow_mod(nonsquare,(p-1)/2,p)==1)nonsquare++;CHECK(poly::fps_sqrt({nonsquare},1)==nullopt);
 for(int it=0;it<200;it++){
  int n=ri(0,50);V x(n),a(n),y;for(int i=0;i<n;i++)x[i]=i*13+7;for(int &v:a)v=ri(0,p-1);for(int v:x)y.push_back(eval(a,v));
  poly::ProductTree t(x);CHECK(t.evaluate(a)==y);CHECK(t.interpolate(y)==a);
  if(n){V v(n);for(int i=0;i<n;i++)v[i]=eval(a,i);int at=ri(-100,100);CHECK(interpolate_consecutive(v,at)==eval(a,(at%p+p)%p));}
 }
 for(int it=0;it<200;it++){
  int k=ri(0,7),n=1<<k;V a(n),b(n);for(int &x:a)x=ri(0,p-1);for(int &x:b)x=ri(0,p-1);
  for(int type=0;type<3;type++){
   V x=a,y=b,want(n);for(int i=0;i<n;i++)for(int j=0;j<n;j++){int at=type==0?(i^j):type==1?(i|j):(i&j);want[at]=(want[at]+i64(a[i])*b[j])%p;}
   subset::fwt(x,type);subset::fwt(y,type);for(int i=0;i<n;i++)x[i]=i64(x[i])*y[i]%p;subset::fwt(x,type,true);CHECK(x==want);
  }
  V want(n);for(int s=0;s<n;s++){for(int t=s;;t=(t-1)&s){want[s]=(want[s]+i64(a[t])*b[s^t])%p;if(!t)break;}}
  CHECK(subset::convolution(a,b)==want);V x=a;subset::sos(x);subset::sos(x,true);CHECK(x==a);
 }
 for(int it=0;it<300;it++){
  int n=ri(1,10);V c(n),s(150);for(int &x:c)x=ri(0,p-1);for(int i=0;i<n;i++)s[i]=ri(0,p-1);for(int i=n;i<(int)s.size();i++)for(int j=1;j<=n;j++)s[i]=(s[i]+i64(c[j-1])*s[i-j])%p;
  auto bm=poly::berlekamp_massey(V(s.begin(),s.begin()+2*n+2));for(int i=0;i<150;i++){CHECK(poly::linear_rec(s,c,i)==s[i]);CHECK(poly::linear_rec(V(s.begin(),s.begin()+2*n+2),bm,i)==s[i]);}
 }
 CHECK(poly::berlekamp_massey({}).empty());CHECK(poly::linear_rec({}, {}, 100)==0);
 for(int it=0;it<400;it++){
  int n=ri(1,4),m=ri(1,4),mod=5;vector<vector<int>>a(n,V(m+1));for(auto &r:a)for(int &x:r)x=ri(0,4);V ans;int got=gauss_mod(a,ans,mod),count=0,total=1;for(int i=0;i<m;i++)total*=5;
  for(int mask=0;mask<total;mask++){V x(m);int z=mask;for(int &v:x)v=z%5,z/=5;bool ok=true;for(auto r:a){int val=0;for(int j=0;j<m;j++)val+=r[j]*x[j];ok&=val%5==r[m];}count+=ok;}
  CHECK(got==(count==0?0:count==1?1:2));if(got)for(auto r:a){int val=0;for(int j=0;j<m;j++)val+=r[j]*ans[j];CHECK(val%5==r[m]);}
 }
 for(int it=0;it<200;it++){
  int n=ri(0,6);vector<vector<int>>a(n,V(n));for(auto &r:a)for(int &x:r)x=ri(0,100);vector<int>perm(n);iota(perm.begin(),perm.end(),0);i64 ans=0;do{int inv=0;i64 z=1;for(int i=0;i<n;i++){z=z*a[i][perm[i]]%p;for(int j=0;j<i;j++)inv+=perm[j]>perm[i];}ans=(ans+((inv&1)?p-z:z))%p;}while(next_permutation(perm.begin(),perm.end()));CHECK(determinant(a,p)==ans);
  vector<vector<double>>r(4,vector<double>(4));vector<double>x={1,2,3};for(int i=0;i<4;i++)for(int j=0;j<3;j++)r[i][j]=ri(-10,10),r[i][3]+=r[i][j]*x[j];vector<double>sol;CHECK(gauss_real(r,sol)!=0);for(auto row:r){double z=0;for(int j=0;j<3;j++)z+=row[j]*sol[j];CHECK(abs(z-row[3])<1e-6);}
 }
 for(int it=0;it<300;it++){
  XorBasis b;V a(ri(0,12));set<u64>s={0};for(int &x:a){x=ri(0,1023);b.insert(x);auto old=s;for(u64 v:old)s.insert(v^x);}CHECK(b.maximum()==*s.rbegin());int k=0;for(u64 x:s)CHECK(b.kth(k++)==x);for(int j=0;j<30;j++){int x=ri(0,1023);CHECK(b.contains(x)==(bool)s.count(x));}
 }
 for(int it=0;it<200;it++){
  int n=ri(1,5);vector<pair<int,int>>ed;for(int u=0;u<n;u++)for(int v=0;v<u;v++)if(ri(0,1))ed.push_back({u,v});int count=0;
  for(int mask=0;mask<(1<<(int)ed.size());mask++)if(__builtin_popcount((unsigned)mask)==n-1){V p(n);iota(p.begin(),p.end(),0);function<int(int)>f=[&](int x){return p[x]==x?x:p[x]=f(p[x]);};bool ok=true;for(int j=0;j<(int)ed.size();j++)if(mask>>j&1){auto[u,v]=ed[j];if(f(u)==f(v))ok=false;else p[f(u)]=f(v);}count+=ok;}CHECK(spanning_trees(n,ed,p)==count);
 }
 for(int it=0;it<300;it++){
  vvd a={{1,0},{0,1}};vd b={(double)ri(1,20),(double)ri(1,20)},c={(double)ri(-10,10),(double)ri(-10,10)};
  for(int i=0;i<3;i++)a.push_back({(double)ri(-3,4),(double)ri(-3,4)}),b.push_back(ri(-3,20));
  vvd lines=a;vd rhs=b;lines.push_back({-1,0});rhs.push_back(0);lines.push_back({0,-1});rhs.push_back(0);double best=-INFINITY;
  for(int i=0;i<(int)lines.size();i++)for(int j=0;j<i;j++){double det=lines[i][0]*lines[j][1]-lines[i][1]*lines[j][0];if(abs(det)<1e-12)continue;double x=(rhs[i]*lines[j][1]-lines[i][1]*rhs[j])/det,y=(lines[i][0]*rhs[j]-rhs[i]*lines[j][0])/det;bool ok=true;for(int k=0;k<(int)lines.size();k++)ok&=lines[k][0]*x+lines[k][1]*y<=rhs[k]+1e-8;if(ok)best=max(best,c[0]*x+c[1]*y);}
  vd x;double got=LPSolver(a,b,c).solve(x);if(isinf(best))CHECK(isinf(got)&&got<0);else CHECK(abs(got-best)<1e-6);
 }
 vd x;CHECK(isinf(LPSolver({{-1}}, {0}, {1}).solve(x)));
 done("algebra");
}
