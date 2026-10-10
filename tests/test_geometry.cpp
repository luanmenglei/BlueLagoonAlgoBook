#include "Test.h"
#include "../content/geometry/Point.h"
#include "../content/geometry/OnSegment.h"
#include "../content/geometry/sideOf.h"
#include "../content/geometry/lineIntersection.h"
#include "../content/geometry/SegmentIntersection.h"
#include "../content/geometry/LineProjectionReflection.h"
#include "../content/geometry/lineDistance.h"
#include "../content/geometry/SegmentDistance.h"
#include "../content/geometry/InsidePolygon.h"
#include "../content/geometry/PolygonArea.h"
#include "../content/geometry/ConvexHull.h"
#include "../content/geometry/HullDiameter.h"
#include "../content/geometry/ClosestPair.h"
#include "../content/geometry/PointInsideHull.h"
#include "../content/geometry/LineHullIntersection.h"
#include "../content/geometry/CircleIntersection.h"
#include "../content/geometry/CircleLine.h"
#include "../content/geometry/CircleTangents.h"
#include "../content/geometry/circumcircle.h"
#include "../content/geometry/MinimumEnclosingCircle.h"
#include "../content/geometry/CirclePolygonIntersection.h"
#include "../content/geometry/3dHull.h"
#include "../content/geometry/ConvexTangent.h"
#include "../content/geometry/Minkowski.h"
#include "../content/geometry/HalfPlane.h"
double area(vector<DP> p){if(p.empty())return 0;return abs(polygonArea2(p))/2;}
vector<DP> clip(vector<DP> p,const HalfPlane &h){vector<DP>q;if(p.empty())return q;for(int i=0;i<(int)p.size();i++){DP a=p[i],b=p[(i+1)%p.size()];double x=h.v.cross(a-h.p),y=h.v.cross(b-h.p);if(x>=-1e-10)q.push_back(a);if((x>1e-10 && y<-1e-10)||(x<-1e-10 && y>1e-10))q.push_back(a+(b-a)*(x/(x-y)));}return q;}
int main(){
 for(int it=0;it<1000;it++){
  vector<IP>p;for(int i=0,n=ri(0,25);i<n;i++)p.emplace_back(ri(-30,30),ri(-30,30));auto h=convexHull(p);
  if(!h.empty())for(IP x:p)CHECK(inHull(h,x,false));
  if(!h.empty()){auto d=hullDiameter(h);i64 want=0;for(IP a:p)for(IP b:p)want=max(want,(a-b).dist2());CHECK((d[0]-d[1]).dist2()==want);}
  if(p.size()>=2){auto [a,b]=closest(p);i64 want=LLONG_MAX;for(int i=0;i<(int)p.size();i++)for(int j=0;j<i;j++)want=min(want,(p[i]-p[j]).dist2());CHECK((a-b).dist2()==want);}
  if(h.size()>=3)for(int j=0;j<20;j++){IP x(ri(-50,50),ri(-50,50));CHECK(inHull(h,x,false)==inPolygon(h,x,false));CHECK(inHull(h,x,true)==inPolygon(h,x,true));}
  if(h.size()>=3){IP x(100,ri(-100,100));auto c=h;c.push_back(h[0]);for(int up:{-1,1}){int at=convex_tangent(c,x,up);CHECK(at>=0 && at<(int)h.size());bool pos=true,neg=true;for(IP v:h)pos&=x.cross(h[at],v)>=0,neg&=x.cross(h[at],v)<=0;CHECK(pos||neg);}
   IP dir(ri(-10,10),ri(-10,10));if(dir==IP())dir=IP(1,0);int at=extrVertex(h,dir);i64 best=LLONG_MIN;for(IP v:h)best=max(best,v.dot(dir));CHECK(h[at].dot(dir)==best);
   IP a(ri(-50,50),ri(-50,50)),b(ri(-50,50),ri(-50,50));if(a!=b){auto edges=lineHull(a,b,h);bool pos=false,neg=false,zero=false;for(IP v:h){i64 z=a.cross(b,v);pos|=z>0;neg|=z<0;zero|=z==0;}CHECK((edges[0]!=-1)==((pos&&neg)||zero));}
  }
  vector<IP>q;for(int i=0,n=ri(0,20);i<n;i++)q.emplace_back(ri(-20,20),ri(-20,20));q=convexHull(q);vector<IP>all;for(IP a:h)for(IP b:q)all.push_back(a+b);CHECK(convexHull(minkowski(h,q))==convexHull(all));
 }
 for(int it=0;it<500;it++){
  DP a(ri(-10,10),ri(-10,10)),b(ri(-10,10),ri(-10,10)),p(ri(-10,10),ri(-10,10));if(a!=b){DP q=lineProj(a,b,p),r=lineProj(a,b,p,true);CHECK(abs((b-a).cross(q-a))<1e-7);CHECK(abs((q-p).dot(b-a))<1e-7);CHECK((r+p-q*2).dist()<1e-7);CHECK(abs(abs(lineDist(a,b,p))-(p-q).dist())<1e-7);}
  double want;if(a==b)want=(a-p).dist();else{double t=max(0.,min(1.,(p-a).dot(b-a)/(b-a).dist2()));want=(p-(a+(b-a)*t)).dist();}CHECK(abs(segDist(a,b,p)-want)<1e-7);
 }
 {
  auto x=lineInter(DP(0,0),DP(2,2),DP(0,2),DP(2,0));CHECK(x.first==1 && (x.second-DP(1,1)).dist()<1e-9);
  CHECK(lineInter(DP(0,0),DP(1,0),DP(0,1),DP(1,1)).first==0);
  CHECK(lineInter(DP(0,0),DP(1,0),DP(2,0),DP(3,0)).first==-1);
  CHECK(segInter(IP(0,0),IP(2,0),IP(1,0),IP(3,0)).size()==2);
  CHECK(segInter(DP(0,0),DP(2,2),DP(0,2),DP(2,0)).size()==1);
  CHECK(onSegment(IP(0,0),IP(2,0),IP(1,0)));CHECK(sideOf(IP(0,0),IP(1,0),IP(0,1))==1);
 }
 for(int it=0;it<1000;it++){
  vector<HalfPlane>a;int n=ri(0,15);for(int i=0;i<n;i++){DP p(ri(-20,20),ri(-20,20)),q(ri(-20,20),ri(-20,20));if(p!=q)a.emplace_back(p,q);}
  vector<DP>br={DP(-50,-50),DP(50,-50),DP(50,50),DP(-50,50)};for(auto h:a)br=clip(br,h);
  auto got=halfplane(a,50);double want=area(br),val=area(got);CHECK(isfinite(val));CHECK(abs(val-want)<1e-5*max(1.,want));
 }
 for(int it=0;it<500;it++){
  DP a(ri(-10,10),ri(-10,10)),b(ri(-10,10),ri(-10,10));if(a==b)b=b+DP(1,0);double r=ri(0,10),s=ri(0,10),dist=(a-b).dist();pair<DP,DP>out;
  bool expected=dist<=r+s+1e-10 && dist>=abs(r-s)-1e-10;bool ok=circleInter(a,b,r,s,&out);CHECK(ok==expected);if(ok)for(DP p:{out.first,out.second})CHECK(abs((p-a).dist()-r)<1e-6 && abs((p-b).dist()-s)<1e-6);
  DP x(ri(-10,10),ri(-10,10)),y(ri(-10,10),ri(-10,10));if(x==y)y=y+DP(1,0);auto ps=circleLine(a,r,x,y);for(DP p:ps)CHECK(abs((p-a).dist()-r)<1e-6 && abs(lineDist(x,y,p))<1e-6);
  for(int sign:{1,-1}){auto ts=tangents(a,r,b,s*sign);for(auto [p,q]:ts){CHECK(abs((p-a).dist()-r)<1e-6 && abs((q-b).dist()-s)<1e-6);if(p!=q)CHECK(abs((p-a).dot(q-p))<1e-5 && abs((q-b).dot(q-p))<1e-5);}}
 }
 for(int it=0;it<150;it++){
  vector<DP>ps;for(int i=0,n=ri(1,9);i<n;i++)ps.emplace_back(ri(-10,10),ri(-10,10));double best=INFINITY;
  auto candidate=[&](DP c,double r){for(DP p:ps)if((p-c).dist()>r+1e-7)return;best=min(best,r);};
  for(DP p:ps)candidate(p,0);for(int i=0;i<(int)ps.size();i++)for(int j=0;j<i;j++)candidate((ps[i]+ps[j])/2,(ps[i]-ps[j]).dist()/2);
  for(int i=0;i<(int)ps.size();i++)for(int j=0;j<i;j++)for(int k=0;k<j;k++)if(abs(ps[i].cross(ps[j],ps[k]))>1e-9)candidate(ccCenter(ps[i],ps[j],ps[k]),ccRadius(ps[i],ps[j],ps[k]));
  auto [c,r]=mec(ps);CHECK(abs(best-r)<1e-6);for(DP p:ps)CHECK((p-c).dist()<=r+1e-6);
 }
 const double pi=acos(-1.);vector<DP>box={DP(-10,-10),DP(10,-10),DP(10,10),DP(-10,10)};CHECK(abs(circlePoly(DP(),1,box)-pi)<1e-9);
 vector<DP>small={DP(-.5,-.5),DP(.5,-.5),DP(.5,.5),DP(-.5,.5)};CHECK(abs(circlePoly(DP(),10,small)-1)<1e-9);CHECK(abs(circlePoly(DP(100,100),1,box))<1e-9);
 for(int it=0;it<60;it++){
  vector<P3>a;int n=ri(4,12);while((int)a.size()<n){P3 p(ri(-50,50),ri(-50,50),ri(-50,50));bool ok=true;for(int i=0;i<(int)a.size();i++)if(a[i]==p)ok=false;for(int i=0;i<(int)a.size();i++)for(int j=0;j<i;j++)for(int k=0;k<j;k++)if(abs((a[j]-a[i]).cross(a[k]-a[i]).dot(p-a[i]))<1e-8)ok=false;if(ok)a.push_back(p);}
  auto fs=hull3d(a);map<pair<int,int>,int>edges;set<int>verts;
  for(auto f:fs){P3 normal=(a[f.b]-a[f.a]).cross(a[f.c]-a[f.a]);CHECK(normal.dist()>0);for(P3 p:a)CHECK(normal.dot(p-a[f.a])<=1e-7);for(auto [u,v]:{pair{f.a,f.b},pair{f.b,f.c},pair{f.c,f.a}})edges[minmax(u,v)]++,verts.insert(u),verts.insert(v);}
  for(auto [e,c]:edges)CHECK(c==2);CHECK((int)verts.size()-(int)edges.size()+(int)fs.size()==2);
 }
 done("geometry");
}
