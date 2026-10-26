#include "Test.h"
#include "../content/strings/PrefixZ.h"
#include "../content/strings/Hash.h"
#include "../content/strings/Manacher.h"
#include "../content/strings/AhoCorasick.h"
#include "../content/strings/SuffixLCP.h"
#include "../content/strings/SuffixArray.h"
#include "../content/strings/SAM.h"
#include "../content/strings/Eertree.h"
#include "../content/strings/Duval.h"
#include "../content/strings/BitsetLCS.h"
string random_string(int n,int alphabet=3){string s;while(n--)s+=char('a'+ri(0,alphabet-1));return s;}
int occurrences(const string &s,const string &t){int ans=0;for(int i=0;i+(int)t.size()<=(int)s.size();i++)ans+=s.compare(i,t.size(),t)==0;return ans;}
int main(){
 for(int it=0;it<1500;it++) {
  string s=random_string(ri(0,40)),t=random_string(ri(1,10));int n=s.size();
  auto pi=prefix_function(s),z=z_function(s);for(int i=0;i<n;i++){int p=0;for(int k=1;k<=i;k++)if(s.substr(0,k)==s.substr(i-k+1,k))p=k;CHECK(pi[i]==p);int k=0;while(i+k<n && s[k]==s[i+k])k++;if(i)CHECK(z[i]==k);}
  vector<int>match;for(int i=0;i+(int)t.size()<=n;i++)if(s.compare(i,t.size(),t)==0)match.push_back(i);CHECK(kmp(s,t)==match);
  auto [o,e]=manacher(s);for(int i=0;i<n;i++){int k=1;while(i-k>=0 && i+k<n && s[i-k]==s[i+k])k++;CHECK(o[i]==k);k=0;while(i-k-1>=0 && i+k<n && s[i-k-1]==s[i+k])k++;CHECK(e[i]==k);}
  StringHash h(s);for(int j=0;j<20 && n;j++){int len=ri(0,n),a=ri(0,n-len),b=ri(0,n-len);CHECK((h.get(a,a+len)==h.get(b,b+len))==(s.substr(a,len)==s.substr(b,len)));}
  SuffixLCP sa(s);vector<int>order(n+1);iota(order.begin(),order.end(),0);sort(order.begin(),order.end(),[&](int a,int b){return s.substr(a)<s.substr(b);});CHECK(sa.sa.sa==order);
  for(int j=0;j<30;j++){int a=ri(0,n),b=ri(0,n),k=0;while(a+k<n && b+k<n && s[a+k]==s[b+k])k++;CHECK(sa.lcp(a,b)==k);}
  auto fac=duval(s);string prev;for(auto [l,r]:fac){string x=s.substr(l,r-l);for(int k=1;k<(int)x.size();k++)CHECK(x<x.substr(k)+x.substr(0,k));if(!prev.empty())CHECK(prev>=x);prev=x;}
  if(n){string want=s;for(int j=1;j<n;j++)want=min(want,s.substr(j)+s.substr(0,j));int k=min_rotation(s);CHECK(s.substr(k)+s.substr(0,k)==want);}else CHECK(min_rotation(s)==0);
 }
 for(int it=0;it<300;it++){
  string s;int n=ri(0,40);for(int i=0;i<n;i++)s+=char(ri(1,255));SuffixArray a(s);vector<int>v(n+1);iota(v.begin(),v.end(),0);sort(v.begin(),v.end(),[&](int x,int y){return s.substr(x)<s.substr(y);});CHECK(a.sa==v);
 }
 for(int it=0;it<700;it++) {
  string s=random_string(ri(0,250)),t=random_string(ri(0,250));vector<vector<int>>d(s.size()+1,vector<int>(t.size()+1));for(int i=1;i<=(int)s.size();i++)for(int j=1;j<=(int)t.size();j++)d[i][j]=s[i-1]==t[j-1]?d[i-1][j-1]+1:max(d[i-1][j],d[i][j-1]);CHECK(bitset_lcs(s,t)==d.back().back());
 }
 for(int it=0;it<500;it++){
  AC a;vector<string>pat;vector<int>id;for(int j=0;j<10;j++){pat.push_back(random_string(ri(1,8)));id.push_back(a.add(pat.back()));}a.build();for(int k=0;k<3;k++){string s=random_string(ri(0,70));auto cnt=a.count(s);for(int j=0;j<10;j++)CHECK(cnt[id[j]]==occurrences(s,pat[j]));}
 }
 for(int it=0;it<400;it++){
  string s=random_string(ri(0,35));SAM a;Eertree e;map<int,string>pal;
  for(int i=0;i<(int)s.size();i++){a.add(s[i]);int id=e.add(s[i]);pal[id]=s.substr(i-e.tr[id].len+1,e.tr[id].len);}
  a.count();e.count();set<string>all,pals;for(int l=0;l<(int)s.size();l++)for(int r=l+1;r<=(int)s.size();r++){string t=s.substr(l,r-l);all.insert(t);string v=t;reverse(v.begin(),v.end());if(t==v)pals.insert(t);}
  i64 distinct=0;for(int u=1;u<(int)a.tr.size();u++)distinct+=a.tr[u].len-a.tr[a.tr[u].link].len;CHECK(distinct==(i64)all.size());
  for(const string &t:all){int u=0;for(char c:t)u=a.tr[u].ch[c-'a'];CHECK(u!=-1);CHECK(a.tr[u].cnt==occurrences(s,t));}
  CHECK(e.tr.size()==pals.size()+2);for(auto [id,t]:pal)CHECK(e.tr[id].cnt==occurrences(s,t));CHECK(pal.size()==pals.size());
 }
 done("strings");
}
