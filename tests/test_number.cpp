#include "Test.h"
#include "../content/number-theory/Modular.h"
#include "../content/number-theory/Sieve.h"
#include "../content/number-theory/Factor.h"
#include "../content/number-theory/DiscreteLog.h"
#include "../content/number-theory/PrimitiveRoot.h"
#include "../content/number-theory/Binomial.h"
#include "../content/number-theory/FloorSum.h"
#include "../content/number-theory/DuSieve.h"
#include "../content/number-theory/Min25.h"
bool trial(u64 n){if(n<2)return false;for(u64 p=2;p<=n/p;p++)if(n%p==0)return false;return true;}
int main(){
 for(int n=0;n<10000;n++)CHECK(is_prime(n)==trial(n));
 for(u64 n:{1ULL,2ULL,360ULL,1000000007ULL*1000000009ULL,4294967291ULL*4294967291ULL,18446744073709551615ULL}){vector<u64>a;factor(n,a);u128 z=1;for(u64 p:a){CHECK(is_prime(p));z*=p;}CHECK(z==n);}
 for(int it=0;it<3000;it++){int m=ri(1,30),n=ri(1,30),a=ri(-50,50),b=ri(-50,50),l=lcm(m,n),ans=-1;for(int x=0;x<l;x++)if((x-a)%m==0 && (x-b)%n==0){ans=x;break;}auto z=crt(a,m,b,n);CHECK(bool(z)==(ans!=-1));if(z)CHECK(z->first==ans && z->second==l);int inv=inverse(a,m);if(gcd(a,m)==1)CHECK(inv>=0 && (i128(a)*inv%m+m)%m==1%m);else CHECK(inv==-1);}
 CHECK(crt(LLONG_MAX-2,LLONG_MAX-1,0,1)->first==LLONG_MAX-2);
 CHECK(bsgs(0,0,2)==optional<u64>(1));CHECK(bsgs(2,0,4)==optional<u64>(2));
 for(int m=1;m<=60;m++)for(int a=0;a<m;a++){
  vector<int>ans(m,-1);u64 v=1%m;int k=0;while(ans[v]==-1)ans[v]=k++,v=mul_mod(v,a,m);
  for(int b=0;b<m;b++){auto z=bsgs(a,b,m);CHECK(bool(z)==(ans[b]!=-1));if(z)CHECK(*z==(u64)ans[b]);}
 }
 for(int p=2;p<500;p++)if(trial(p)){
  u64 g=primitive_root(p);set<u64>s;u64 x=1;for(int k=0;k<p-1;k++)s.insert(x),x=x*g%p;CHECK(s.size()==(size_t)p-1);
  vector<bool>sq(p);for(int x=0;x<p;x++)sq[x*x%p]=true;
  for(int a=0;a<p;a++){auto z=mod_sqrt(a,p);CHECK(bool(z)==sq[a]);if(z)CHECK(mul_mod(*z,*z,p)==(u64)a);}
 }
 for(int mod=1;mod<=100;mod++){
  vector<vector<int>>c(45,vector<int>(45));c[0][0]=1%mod;
  for(int n=1;n<45;n++){c[n][0]=1%mod;for(int k=1;k<=n;k++)c[n][k]=(c[n-1][k]+c[n-1][k-1])%mod;}
  for(int it=0;it<50;it++){int n=ri(0,44),k=ri(0,n+1);CHECK(binomial_mod(n,k,mod)==(k>n?0:c[n][k]));}
 }
 for(int it=0;it<10000;it++){int n=ri(0,100),m=ri(1,50),a=ri(-100,100),b=ri(-100,100);i128 ans=0;for(int i=0;i<n;i++)ans+=(i64)floor((double)(a*i+b)/m);CHECK(floor_sum(n,m,a,b)==ans);}
 Sieve s(10000);for(int n=1;n<=2000;n++){int phi=0;for(int k=1;k<=n;k++)phi+=gcd(k,n)==1;CHECK(phi==s.phi[n]);int x=n,mu=1;while(x>1){int p=s.lp[x],e=0;while(x%p==0)x/=p,e++;mu=e>1?0:-mu;}CHECK(s.mu[n]==mu);}
 DuSieve d(100);i128 ph=0;i64 mu=0;for(int n=1;n<=10000;n++){ph+=s.phi[n];mu+=s.mu[n];if(n%13==0)CHECK(d.sum_phi(n)==ph && d.sum_mu(n)==mu);}
 vector<i64>sigma(10001),tau(10001);for(int i=1;i<=10000;i++)for(int j=i;j<=10000;j+=i)sigma[j]+=i,tau[j]++;
 for(int it=0;it<200;it++){
  int n=ri(1,10000);Min25 m(n);i64 count=0; i128 sum=0,sp=0,st=0,ss=0;for(int x=1;x<=n;x++){if(trial(x))count++,sum+=x;sp+=s.phi[x];st+=tau[x];ss+=sigma[x];}CHECK(m.cnt[0]==count && m.sum[0]==sum);
  auto phi=[](i64 p,int e)->i128 {i128 x=1;for(int i=1;i<e;i++)x*=p;return x*(p-1);};
  auto divs=[](i64,int e)->i128{return e+1;};auto sig=[](i64 p,int e)->i128{i128 x=1,z=1;for(int i=0;i<e;i++)x*=p,z+=x;return z;};
  CHECK(m.multiplicative_sum(1,-1,phi)==sp);CHECK(m.multiplicative_sum(0,2,divs)==st);CHECK(m.multiplicative_sum(1,1,sig)==ss);
 }
 done("number theory");
}
