/**
 * @file sHull.cpp
 * @date 2013-04-26
 * @author Zach Maybury
 * @brief FIXME: Documentation needed from Zach.
 * @details Headers for functions related to morphing.
 */
#include <iostream>
#include <hash_set.h>
#include <set>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <math.h>


#include "sHull.hpp"

/*
  fast and simple convex hull finder.
  kind of a circular vesion of Steven Fortune's sweepline algorithm
but without the complicated bits.

  and combined with link insertion and flipping will give you Delaunay triangles..

S-hull, Copyright (c) 2010
Dr David SInclair
Cambridge, UK

email david@s-hull.org
S-hull is free software and may be obtained from www.s-hull.org.
It may be freely copied, modified,
and redistributed under the following conditions which might loosely be termed a contribtors beerware license:
1. All copyright notices must remain intact in all files.
2. A copy of this text file must be distributed along with any copies
   of S-hull that you redistribute; this includes copies that you have
   modified, or copies of programs or other software products that
   include S-hull where distributed as source.

3. If you modify S-hull, you must include a notice giving the
   name of the person performing the modification, the date of
   modification, and the reason for such modification.

4. If you are distributing a binary or compiled version of s-hull it
	    is not necessary to include any acknowledgement or reference
	    to s-hull.
5. There is no warranty or other guarantee of fitness for S-hull, it is
   provided solely "as is".  Bug reports or fixes may be sent to
   bugs@s-hull.org; the authors may or may not act on them as
   they desire.
6. By copying or compliing the code for S-hull you explicitly indemnify
the copyright holder against any liability he may incur as a result of you
copying the code.

7. If you meet any of the contributors to the code you used from s-hull.org
	    in a pub or a bar, and you think the source code they contributed to is worth it,
	    you can buy them a beer.

	    If your principles run against beer a bacon-double-cheeseburger would do just as nicely
	    or you could email david@s-hull.org and arrange to make a donation of 10 of your local currancy units
	    to support s-hull.org.


 */





void circle_cent2(float r1,float c1, float r2,float c2, float r3,float c3,
float &r,float &c, float &ro2){
  /*
   *  function to return the center of a circle and its radius
   * degenerate case should never be passed to this routine!!!!!!!!!!!!!
   * but will return r0 = -1 if it is.
   */

   float v1 = 2*(r2-r1), v2 = 2*(c2-c1), v3 = r2*r2 - r1*r1 + c2*c2 - c1*c1;
   float v4 = 2*(r3-r1),
   v5 = 2*(c3-c1),
   v6 = r3*r3 - r1*r1 + c3*c3 - c1*c1,

   v7 =  v2*v4 - v1*v5;
   if( v7 == 0 ){
      r=0;
      c=0;
      ro2 = -1;
      return;
   }

   c = (v4*v3 - v1*v6)/v7;
   if( v1 != 0 )
      r = (v3 - c*v2)/v1;
   else
      r = (v6 - c*v5)/v4;

   ro2 = ( (r-r1)*(r-r1) + (c-c1)*(c-c1) );

   return;
}


void write_Shx(std::vector<Shx> &pts, char * fname){
   std::ofstream out(fname, ios::out);

   int nr = pts.size();
   out << nr << " 2" << endl;

   for (int r = 0; r < nr; r++){
     out << pts[r].r << ' ' << pts[r].c <<  endl;
   }
   out.close();

   return;
};



/*
 write out triangle ids to be compatible with matlab/octave array numbering.

 */
void write_Triads(std::vector<Triad> &ts, char * fname){
   std::ofstream out(fname, ios::out);

   int nr = ts.size();
   out << nr << " 7" << endl;

   for (int r = 0; r < nr; r++){
     out << ts[r].a+1 << ' ' << ts[r].b+1 <<' ' << ts[r].c+1 <<' '
	 << ts[r].ab+1 <<' ' << ts[r].ac+1 <<' ' << ts[r].bc+1 << " " << ts[r].ro <<  endl;
   }
   out.close();

   return;
};




void T_flip2( std::vector<Shx> &pts, std::vector<Triad> &triads, int *slump, int numt, int start){

  float R,C,r0,c0, r1,c1, r2,c2, r3,c3, rot, rot2, dR, dC;
  int pa,pb,pc, pd, ab, bc, ac, D, L1, L2, L3, L4, T2;
  std::vector<int> aliens;
  Triad tx, tx2;
  std::set<int> ids;

  for( int t=start; t<numt; t++){

    //write_Triads(triads, "tflip0.mat");

    Triad &tri = triads[t];
    // test all 3 neighbours of tri
    pa = slump[tri.a];
    pb = slump[tri.b];
    pc = slump[tri.c];

    int flipped = 0;


    if( tri.bc >= 0 ){
      T2 = tri.bc;
      Triad &t2 = triads[T2];
      // find relative orientation (shared limb).
      if( t2.ab == t ){
	D = t2.c;
	pd = slump[t2.c];

	if( tri.a == t2.a){
	  L3 = t2.ac;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ac;
	}
      }
      else if(  t2.ac == t ){
	D = t2.b;
	pd = slump[t2.b];

	if( tri.a == t2.a){
	  L3 = t2.ab;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ab;
	}
      }
      else if(  t2.bc == t ){
	D = t2.a;
	pd = slump[t2.a];

	if( tri.b == t2.b){
	  L3 = t2.ab;
	  L4 = t2.ac;
	}
	else{
	  L3 = t2.ac;
	  L4 = t2.ab;
	}
      }
      else{
	cerr << "fuck up at line 12 dude!" << endl;
      }

      r3 = pts[pd].r;
      c3 = pts[pd].c;
      dR = tri.R-r3;
      dC = tri.C-c3;


      if( dR*dR+dC*dC < tri.ro ){  // not valid in the Delaunay sense.
	L1 = tri.ab;
	L2 = tri.ac;

	circle_cent2(pts[pa].r, pts[pa].c, pts[pb].r, pts[pb].c, r3, c3, R, C , rot);

	tx.a = tri.a;
	tx.b = tri.b;
	tx.c = D;
	tx.ro = rot;
	tx.R = R;
	tx.C = C;

	tx.ab = L1;
	tx.ac = T2;
	tx.bc = L3;


	// triangle 2;
	tx2.a = tri.a;
	tx2.b = tri.c;
	tx2.c = D;
	circle_cent2(pts[pa].r, pts[pa].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);
	tx2.ro = rot;
	tx2.R = R;
	tx2.C = C;

	tx2.ab = L2;
	tx2.ac = t;
	tx2.bc = L4;


	ids.insert(t);
	ids.insert(T2);

	t2 = tx2;
	tri = tx;
	flipped = 1;

	// change knock on triangle labels.
	if( L3 >= 0 ){
	  Triad &t3 = triads[L3];
	  if( t3.ab == T2 ) t3.ab = t;
	  else if( t3.bc == T2 ) t3.bc = t;
	  else if( t3.ac == T2 ) t3.ac = t;
	}

	if(L2 >= 0 ){
	  Triad &t4 = triads[L2];
	  if( t4.ab == t ) t4.ab = T2;
	  else if( t4.bc == t ) t4.bc = T2;
	  else if( t4.ac == t ) t4.ac = T2;
	}

      }
    }

    //write_Triads(triads, "tflip1.mat");
    int df = 9;
    df = 8;
  }






  return;
}



/*  version in which the ids of the triangles associated with the sides of the hull are tracked.


 */

void s_hull_del_ray2( std::vector<Shx> &pts, std::vector<Triad> &triads)
{

  int nump = pts.size();


  float r = pts[0].r;
  float c = pts[0].c;
  for( int k=0; k<nump; k++){
    float dr = pts[k].r-r;
    float dc = pts[k].c-c;

    pts[k].ro = dr*dr + dc*dc;

  }

  sort( pts.begin(), pts.end() );


  float r1 = pts[0].r;
  float c1 = pts[0].c;

  float r2 = pts[1].r;
  float c2 = pts[1].c;
  int mid = -1;
  float romin2 = 100000000.0, ro2, R,C;

  int k=2;
  while (k<nump){

    circle_cent2(r1,c1,r2,c2,  pts[k].r,  pts[k].c, r,c,ro2);
    if( ro2 < romin2 && ro2 > 0 ){
      mid = k;
      romin2 = ro2;
      R = r;
      C = c;

    }
    else if( romin2 *4 < pts[k].ro )
      k=nump;

    k++;
  }

  //std::cerr << "earwig noodles " << pts[1].id << " " << pts[mid].id << " " << romin2 << endl;

  Shx pt0 = pts[0];
  Shx pt1 = pts[1];
  Shx pt2 = pts[mid];

  pts.erase(pts.begin() + mid);  // necessary for round off reasons:((((((
  pts.erase(pts.begin() );
  pts.erase(pts.begin() );

  for( int k=0; k<nump-3; k++){
    float dr = pts[k].r-R;
    float dc = pts[k].c-C;

    pts[k].ro = dr*dr + dc*dc;

  }

  sort( pts.begin(), pts.end() );

  pts.insert(pts.begin(), pt2);
  pts.insert(pts.begin(), pt1);
  pts.insert(pts.begin(), pt0);

  int slump [nump];

  for( int k=0; k<nump; k++){
    slump[ pts[k].id] = k;
  }

  std::vector<Shx> hull;


  r = (pts[0].r + pts[1].r + pts[2].r )/3.0;
  c = (pts[0].c + pts[1].c + pts[2].c )/3.0;

  float dr0 = pts[0].r - r,  dc0 = pts[0].c - c;
  float tr01 =  pts[1].r - pts[0].r, tc01 =  pts[1].c - pts[0].c;

  float df = -tr01* dc0 + tc01*dr0;
  if( df < 0 ){   // [ 0 1 2 ]
    pt0.tr = pt1.r-pt0.r;
    pt0.tc = pt1.c-pt0.c;
    pt0.trid = 0;
    hull.push_back( pt0 );

    pt1.tr = pt2.r-pt1.r;
    pt1.tc = pt2.c-pt1.c;
    pt1.trid = 0;
    hull.push_back( pt1 );

    pt2.tr = pt0.r-pt2.r;
    pt2.tc = pt0.c-pt2.c;
    pt2.trid = 0;
    hull.push_back( pt2 );


    Triad tri(pt0.id,pt1.id,pt2.id);
    tri.ro = romin2;
    tri.R = R;
    tri.C = C;

    triads.push_back(tri);

  }
  else{          // [ 0 2 1 ] as anti-clockwise turning is the work of the devil....
    pt0.tr = pt2.r-pt0.r;
    pt0.tc = pt2.c-pt0.c;
    pt0.trid = 0;
    hull.push_back( pt0 );

    pt2.tr = pt1.r-pt2.r;
    pt2.tc = pt1.c-pt2.c;
    pt2.trid = 0;
    hull.push_back( pt2 );

    pt1.tr = pt0.r-pt1.r;
    pt1.tc = pt0.c-pt1.c;
    pt1.trid = 0;
    hull.push_back( pt1 );

    Triad tri(pt0.id,pt2.id,pt1.id);
    tri.ro = romin2;
    tri.R = R;
    tri.C = C;
    triads.push_back(tri);
  }

  // add new points into hull (removing obscured ones from the chain)
  // and creating triangles....
  // that will need to be flipped.

  float dr, dc, rx,cx;
  Shx  ptx;
  int numt;

  for( int k=3; k<nump; k++){
    rx = pts[k].r;    cx = pts[k].c;
    ptx.r = rx;
    ptx.c = cx;
    ptx.id = pts[k].id;

    int numh = hull.size(), numh_old = numh;
    dr = rx- hull[0].r;    dc = cx- hull[0].c;  // outwards pointing from hull[0] to pt.

    if(0){
    cerr << "numt = " << triads.size() << endl;
    cerr << "ids = [ " ;
    for(int g=0; g<numh; g++){
      cerr << hull[g].id+1 << ' ' ;
    }
    cerr <<   hull[0].id+1 << "];" << endl;


    cerr << "trids = [ " ;
    for(int g=0; g<numh; g++){
      cerr << hull[g].trid+1 << ' ' ;
    }
    cerr <<   hull[0].trid+1 << "];" << endl;


    cerr << "h_test = [ " ;
    for(int g=0; g<numh-1; g++){
      float gaf = -(hull[g+1].c-hull[g].c) * hull[g].tr + (hull[g+1].r-hull[g].r)*hull[g].tc;
      cerr << gaf << ' ' ;
    }

    cerr <<   -(hull[0].c-hull[numh-1].c) * hull[numh-1].tr +
               (hull[0].r-hull[numh-1].r)*hull[numh-1].tc << "];" << endl;

    }


    std::vector<int> pidx, tridx;
    int hidx;  // new hull point location within hull.....


    float df = -dc* hull[0].tr + dr*hull[0].tc;    // visibility test vector.
    if( df < 0 ){  // starting with a visible hull facet !!!
      int e1 = 1, e2 = numh;
      hidx = 0;

      // check to see if segment numh is also visible
      df = -dc* hull[numh-1].tr + dr*hull[numh-1].tc;
      //cerr << df << ' ' ;
      if( df < 0 ){    // visible.
	pidx.push_back(hull[numh-1].id);
	tridx.push_back(hull[numh-1].trid);


	for( int h=0; h<numh-1; h++){
	  // if segment h is visible delete h
	  dr = rx- hull[h].r;    dc = cx- hull[h].c;
	  df = -dc* hull[h].tr + dr*hull[h].tc;
	  pidx.push_back(hull[h].id);
	  tridx.push_back(hull[h].trid);
	  if( df < 0 ){
	    hull.erase(hull.begin() + h);
	    h--;
	    numh--;
	  }
	  else{	  // quit on invisibility
	    ptx.tr = hull[h].r - ptx.r;
	    ptx.tc = hull[h].c - ptx.c;

	    hull.insert( hull.begin() , ptx);
	    numh++;
	    break;
	  }
	}
	// look backwards through the hull structure.

	for( int h=numh-2; h>0; h--){
	  // if segment h is visible delete h + 1
	  dr = rx- hull[h].r;    dc = cx- hull[h].c;
	  df = -dc* hull[h].tr + dr*hull[h].tc;

	  if( df < 0 ){  // h is visible
	    pidx.insert(pidx.begin(), hull[h].id);
	    tridx.insert(tridx.begin(), hull[h].trid);
	    hull.erase(hull.begin() + h+1);  // erase end of chain

	  }
	  else{

	    h = hull.size()-1;
	    hull[h].tr = -hull[h].r + ptx.r;   // points at start of chain.
	    hull[h].tc = -hull[h].c + ptx.c;
	    break;
	  }
	}

	df = 9;

      }
      else{
	//	cerr << df << ' ' << endl;
	hidx = 1;  // keep pt hull[0]
	tridx.push_back(hull[0].trid);
	pidx.push_back(hull[0].id);

	for( int h=1; h<numh; h++){
	  // if segment h is visible delete h
	  dr = rx- hull[h].r;    dc = cx- hull[h].c;
	  df = -dc* hull[h].tr + dr*hull[h].tc;
	  pidx.push_back(hull[h].id);
	  tridx.push_back(hull[h].trid);
	  if( df < 0 ){                     // visible
	    hull.erase(hull.begin() + h);
	    h--;
	    numh--;
	  }
	  else{	  // quit on invisibility
	    ptx.tr = hull[h].r - ptx.r;
	    ptx.tc = hull[h].c - ptx.c;

	    hull[h-1].tr = ptx.r - hull[h-1].r;
	    hull[h-1].tc = ptx.c - hull[h-1].c;

	    hull.insert( hull.begin()+h, ptx);
	    break;
	  }
	}
      }

      df = 8;

    }
    else{
      int e1 = -1,  e2 = numh;
      for( int h=1; h<numh; h++){
	dr = rx- hull[h].r;    dc = cx- hull[h].c;
	df = -dc* hull[h].tr + dr*hull[h].tc;
	if( df < 0 ){
	  if( e1 < 0 ) e1 = h;  // fist visible
	}
	else{
	  if( e1 > 0 ){ // first invisible segment.
	    e2 = h;
	    break;
	  }
	}

      }


      // triangle pidx starts at e1 and ends at e2 (inclusive).
      if( e2 < numh ){
	for( int e=e1; e<=e2; e++){
	  pidx.push_back(hull[e].id);
	  tridx.push_back(hull[e].trid);
	}
      }
      else{
	for( int e=e1; e<e2; e++){
	  pidx.push_back(hull[e].id);
	  tridx.push_back(hull[e].trid);   // there are only n-1 triangles from n hull pts.
	}
	pidx.push_back(hull[0].id);
      }


      // erase elements e1+1 : e2-1 inclusive.

      if( e1 < e2-1){
	hull.erase(hull.begin() + e1+1, hull.begin()+ e2);
      }

      // insert ptx at location e1+1.
      if( e2 == numh){
	ptx.tr = hull[0].r - ptx.r;
	ptx.tc = hull[0].c - ptx.c;
      }
      else{
	ptx.tr = hull[e1+1].r - ptx.r;
	ptx.tc = hull[e1+1].c - ptx.c;
      }

      hull[e1].tr = ptx.r - hull[e1].r;
      hull[e1].tc = ptx.c - hull[e1].c;

      hull.insert( hull.begin()+e1+1, ptx);
      hidx = e1+1;

    }


    int a = ptx.id, T0;
    Triad trx( a, 0,0);
    r1 = pts[slump[a]].r;
    c1 = pts[slump[a]].c;

    int npx = pidx.size()-1;
    numt = triads.size();
    T0 = numt;

    if( npx == 1){
       trx.b = pidx[0];
       trx.c = pidx[1];

      // compute circum circle radius squared (only relative size is required for flipping)

      r2 = pts[slump[trx.b]].r;
      c2 = pts[slump[trx.b]].c;

      circle_cent2(r1,c1,r2,c2, pts[slump[trx.c]].r,pts[slump[trx.c]].c , r,c,ro2);
      trx.ro = ro2;
      trx.R = r;
      trx.C = c;

      trx.bc = tridx[0];
      trx.ab = -1;
      trx.ac = -1;

      // index back into the triads.
      Triad &txx = triads[tridx[0]];
      if( ( trx.b == txx.a && trx.c == txx.b) |( trx.b == txx.b && trx.c == txx.a)) {
	txx.ab = numt;
      }
      else if( ( trx.b == txx.a && trx.c == txx.c) |( trx.b == txx.c && trx.c == txx.a)) {
	txx.ac = numt;
      }
      else if( ( trx.b == txx.b && trx.c == txx.c) |( trx.b == txx.c && trx.c == txx.b)) {
	txx.bc = numt;
      }


      hull[hidx].trid = numt;
      if( hidx > 0 )
	hull[hidx-1].trid = numt;
      else{
	numh = hull.size();
	hull[numh-1].trid = numt;
      }
      triads.push_back( trx );
      numt++;
    }

    else{
      trx.ab = -1;
      for(int p=0; p<npx; p++){
	trx.b = pidx[p];
	trx.c = pidx[p+1];

	// compute circum circle radius squared (only relative size is required for flipping)

	r2 = pts[slump[trx.b]].r;
	c2 = pts[slump[trx.b]].c;

	circle_cent2(r1,c1,r2,c2, pts[slump[trx.c]].r,pts[slump[trx.c]].c , r,c,ro2);
	trx.ro = ro2;
	trx.R = r;
	trx.C = c;


	trx.bc = tridx[p];
	if( p > 0 )
	  trx.ab = numt-1;
	trx.ac = numt+1;

	// index back into the triads.
	Triad &txx = triads[tridx[p]];
	if( ( trx.b == txx.a && trx.c == txx.b) |( trx.b == txx.b && trx.c == txx.a)) {
	  txx.ab = numt;
	}
	else if( ( trx.b == txx.a && trx.c == txx.c) |( trx.b == txx.c && trx.c == txx.a)) {
	  txx.ac = numt;
	}
	else if( ( trx.b == txx.b && trx.c == txx.c) |( trx.b == txx.c && trx.c == txx.b)) {
	  txx.bc = numt;
	}

	if( trx.ab == 26777 || trx.ac == 26777 || trx.bc == 26777){
	  int foon = 7;
	}

	triads.push_back( trx );
	numt++;
      }
      triads[numt-1].ac=-1;

      hull[hidx].trid = numt-1;
      if( hidx > 0 )
	hull[hidx-1].trid = T0;
      else{
	numh = hull.size();
	hull[numh-1].trid = T0;
      }


    }


    //    write_Triads(triads, "tris.mat");
    //int dfx = 9;
    //T_flip2( pts, triads, slump, numt, T0);

    //write_Triads(triads, "tris2.mat");

  }

  cerr << "of triangles " << triads.size() << " to be flipped. "<< endl;

  //write_Triads(triads, "tris0.mat");

  std::set<int> ids, ids2;
  T_flip3( pts, triads, slump, numt, 0, ids);


  int nits = ids.size(), nit=1;
  while(  nits > 0 && nit < 20){
    // char nam[128];
    //sprintf(nam, "tris_%d.mat", nit);
    //write_Triads(triads, nam);

    T_flip4( pts, triads, slump, ids);
    nits = ids.size();
    nit ++;

  }
  //write_Triads(triads, "triangles3.mat");

  return;
}

/*
  flip pairs of triangles that are not valid delaunay triangles
  (the test used is the circum circle not containing any point of the other triangle (in bad English))

 */


void T_flip3( std::vector<Shx> &pts, std::vector<Triad> &triads, int *slump, int numt, int start, std::set<int> &ids){

  float R,C,r0,c0, r1,c1, r2,c2, r3,c3, rot, rot2, dR, dC;
  int pa,pb,pc, pd, ab, bc, ac, D, L1, L2, L3, L4, T2;
  std::vector<int> aliens;
  Triad tx, tx2;
  //std::set<int> ids;

  for( int t=start; t<numt; t++){

    //write_Triads(triads, "tflip0.mat");

    Triad &tri = triads[t];
    // test all 3 neighbours of tri

    int flipped = 0;

    if( tri.bc >= 0 ){

      pa = slump[tri.a];
      pb = slump[tri.b];
      pc = slump[tri.c];

      T2 = tri.bc;
      Triad &t2 = triads[T2];
      // find relative orientation (shared limb).
      if( t2.ab == t ){
	D = t2.c;
	pd = slump[t2.c];

	if( tri.b == t2.a){
	  L3 = t2.ac;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ac;
	}
      }
      else if(  t2.ac == t ){
	D = t2.b;
	pd = slump[t2.b];

	if( tri.b == t2.a){
	  L3 = t2.ab;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ab;
	}
      }
      else if(  t2.bc == t ){
	D = t2.a;
	pd = slump[t2.a];

	if( tri.b == t2.b){
	  L3 = t2.ab;
	  L4 = t2.ac;
	}
	else{
	  L3 = t2.ac;
	  L4 = t2.ab;
	}
      }
      else{
	cerr << "fuck up at line 12 dude! " << t << endl;
      }

      r3 = pts[pd].r;
      c3 = pts[pd].c;
      dR = tri.R-r3;
      dC = tri.C-c3;


      if( dR*dR+dC*dC < tri.ro ){  // not valid in the Delaunay sense.
	L1 = tri.ab;
	L2 = tri.ac;
      	if( L1 != L3 && L2 != L4 ){  // need this check for stability.

	circle_cent2(pts[pa].r, pts[pa].c, pts[pb].r, pts[pb].c, r3, c3, R, C , rot);

	tx.a = tri.a;
	tx.b = tri.b;
	tx.c = D;
	tx.ro = rot;
	tx.R = R;
	tx.C = C;

	tx.ab = L1;
	tx.ac = T2;
	tx.bc = L3;


	// triangle 2;
	tx2.a = tri.a;
	tx2.b = tri.c;
	tx2.c = D;
	circle_cent2(pts[pa].r, pts[pa].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);
	tx2.ro = rot;
	tx2.R = R;
	tx2.C = C;

	tx2.ab = L2;
	tx2.ac = t;
	tx2.bc = L4;


	ids.insert(t);
	ids.insert(T2);

	t2 = tx2;
	tri = tx;
	flipped = 1;

	// change knock on triangle labels.
	if( L3 >= 0 ){
	  Triad &t3 = triads[L3];
	  if( t3.ab == T2 ) t3.ab = t;
	  else if( t3.bc == T2 ) t3.bc = t;
	  else if( t3.ac == T2 ) t3.ac = t;
	}

	if(L2 >= 0 ){
	  Triad &t4 = triads[L2];
	  if( t4.ab == t ) t4.ab = T2;
	  else if( t4.bc == t ) t4.bc = T2;
	  else if( t4.ac == t ) t4.ac = T2;
	}
	}
      }
    }


    if(  flipped == 0 && tri.ab >= 0 ){

      pc = slump[tri.c];
      pb = slump[tri.b];
      pa = slump[tri.a];

      T2 = tri.ab;
      Triad &t2 = triads[T2];
      // find relative orientation (shared limb).
      if( t2.ab == t ){
	D = t2.c;
	pd = slump[t2.c];

	if( tri.a == t2.a){
	  L3 = t2.ac;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ac;
	}
      }
      else if(  t2.ac == t ){
	D = t2.b;
	pd = slump[t2.b];

	if( tri.a == t2.a){
	  L3 = t2.ab;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ab;
	}
      }
      else if(  t2.bc == t ){
	D = t2.a;
	pd = slump[t2.a];

	if( tri.a == t2.b){
	  L3 = t2.ab;
	  L4 = t2.ac;
	}
	else{
	  L3 = t2.ac;
	  L4 = t2.ab;
	}
      }
      else{
	cerr << "fuck up at line 13 dude! " << t << endl;
      }

      r3 = pts[pd].r;
      c3 = pts[pd].c;
      dR = tri.R-r3;
      dC = tri.C-c3;


      if( dR*dR+dC*dC < tri.ro ){  // not valid in the Delaunay sense.
	L1 = tri.ac;
	L2 = tri.bc;
      	if( L1 != L3 && L2 != L4 ){  // need this check for stability.

	circle_cent2(pts[pa].r, pts[pa].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);

	tx.a = tri.c;
	tx.b = tri.a;
	tx.c = D;
	tx.ro = rot;
	tx.R = R;
	tx.C = C;

	tx.ab = L1;
	tx.ac = T2;
	tx.bc = L3;


	// triangle 2;
	tx2.a = tri.c;
	tx2.b = tri.b;
	tx2.c = D;
	circle_cent2(pts[pb].r, pts[pb].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);
	tx2.ro = rot;
	tx2.R = R;
	tx2.C = C;

	tx2.ab = L2;
	tx2.ac = t;
	tx2.bc = L4;


	ids.insert(t);
	ids.insert(T2);

	t2 = tx2;
	tri = tx;
	flipped = 1;

	// change knock on triangle labels.
	if( L3 >= 0 ){
	  Triad &t3 = triads[L3];
	  if( t3.ab == T2 ) t3.ab = t;
	  else if( t3.bc == T2 ) t3.bc = t;
	  else if( t3.ac == T2 ) t3.ac = t;
	}

	if(L2 >= 0 ){
	  Triad &t4 = triads[L2];
	  if( t4.ab == t ) t4.ab = T2;
	  else if( t4.bc == t ) t4.bc = T2;
	  else if( t4.ac == t ) t4.ac = T2;
	}

	}

      }
    }


    if( flipped == 0 && tri.ac >= 0 ){

      pc = slump[tri.c];
      pb = slump[tri.b];
      pa = slump[tri.a];

      T2 = tri.ac;
      Triad &t2 = triads[T2];
      // find relative orientation (shared limb).
      if( t2.ab == t ){
	D = t2.c;
	pd = slump[t2.c];

	if( tri.a == t2.a){
	  L3 = t2.ac;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ac;
	}
      }
      else if(  t2.ac == t ){
	D = t2.b;
	pd = slump[t2.b];

	if( tri.a == t2.a){
	  L3 = t2.ab;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ab;
	}
      }
      else if(  t2.bc == t ){
	D = t2.a;
	pd = slump[t2.a];

	if( tri.a == t2.b){
	  L3 = t2.ab;
	  L4 = t2.ac;
	}
	else{
	  L3 = t2.ac;
	  L4 = t2.ab;
	}
      }
      else{
	cerr << "fuck up at line 14 dude! " << t << endl;
      }

      r3 = pts[pd].r;
      c3 = pts[pd].c;
      dR = tri.R-r3;
      dC = tri.C-c3;


      if( dR*dR+dC*dC < tri.ro ){  // not valid in the Delaunay sense.
	L1 = tri.ab;   // .ac shared limb
	L2 = tri.bc;
      	if( L1 != L3 && L2 != L4 ){  // need this check for stability.

	circle_cent2(pts[pa].r, pts[pa].c, pts[pb].r, pts[pb].c, r3, c3, R, C , rot);

	tx.a = tri.b;
	tx.b = tri.a;
	tx.c = D;
	tx.ro = rot;
	tx.R = R;
	tx.C = C;

	tx.ab = L1;
	tx.ac = T2;
	tx.bc = L3;


	// triangle 2;
	tx2.a = tri.b;
	tx2.b = tri.c;
	tx2.c = D;
	circle_cent2(pts[pb].r, pts[pb].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);
	tx2.ro = rot;
	tx2.R = R;
	tx2.C = C;

	tx2.ab = L2;
	tx2.ac = t;
	tx2.bc = L4;

	ids.insert(t);
	ids.insert(T2);

	t2 = tx2;
	tri = tx;

	// change knock on triangle labels.
	if( L3 >= 0 ){
	  Triad &t3 = triads[L3];
	  if( t3.ab == T2 ) t3.ab = t;
	  else if( t3.bc == T2 ) t3.bc = t;
	  else if( t3.ac == T2 ) t3.ac = t;
	}

	if(L2 >= 0 ){
	  Triad &t4 = triads[L2];
	  if( t4.ab == t ) t4.ab = T2;
	  else if( t4.bc == t ) t4.bc = T2;
	  else if( t4.ac == t ) t4.ac = T2;
	}

	}
      }
    }

    //write_Triads(triads, "tflip1.mat");
    int df = 9;
    df = 8;
  }


  //  cerr << " triangles to resolve " << ids.size() << endl;





  return;
}







// same again but with set of triangle ids to be iterated over.


void T_flip4( std::vector<Shx> &pts, std::vector<Triad> &triads, int *slump, std::set<int> &ids){

  float R,C,r0,c0, r1,c1, r2,c2, r3,c3, rot, rot2, dR, dC;
  int pa,pb,pc, pd, ab, bc, ac, D, L1, L2, L3, L4, T2;

  Triad tx, tx2;
  std::set<int> ids2;
  ids2.clear();

  std::set<int> :: const_iterator x=ids.begin();
  while(x != ids.end() ){
    int t = *x;
    x++;


    Triad &tri = triads[t];
    // test all 3 neighbours of tri
    int flipped = 0;



    if( tri.bc >= 0 ){

      pa = slump[tri.a];
      pb = slump[tri.b];
      pc = slump[tri.c];

      T2 = tri.bc;
      Triad &t2 = triads[T2];
      // find relative orientation (shared limb).
      if( t2.ab == t ){
	D = t2.c;
	pd = slump[t2.c];

	if( tri.b == t2.a){
	  L3 = t2.ac;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ac;
	}
      }
      else if(  t2.ac == t ){
	D = t2.b;
	pd = slump[t2.b];

	if( tri.b == t2.a){
	  L3 = t2.ab;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ab;
	}
      }
      else if(  t2.bc == t ){
	D = t2.a;
	pd = slump[t2.a];

	if( tri.b == t2.b){
	  L3 = t2.ab;
	  L4 = t2.ac;
	}
	else{
	  L3 = t2.ac;
	  L4 = t2.ab;
	}
      }
      else{
	cerr << "fuck up at line 15 dude! t: " << t << "  T2: " <<  T2<<  endl;
	//	tri.prnt();
	//t2.prnt();

      }

      r3 = pts[pd].r;
      c3 = pts[pd].c;
      dR = tri.R-r3;
      dC = tri.C-c3;


      if( dR*dR+dC*dC < tri.ro ){  // not valid in the Delaunay sense.
	L1 = tri.ab;
	L2 = tri.ac;

	if( L1 != L3 && L2 != L4 ){  // need this check for stability.

	circle_cent2(pts[pa].r, pts[pa].c, pts[pb].r, pts[pb].c, r3, c3, R, C , rot);

	tx.a = tri.a;
	tx.b = tri.b;
	tx.c = D;
	tx.ro = rot;
	tx.R = R;
	tx.C = C;

	tx.ab = L1;
	tx.ac = T2;
	tx.bc = L3;


	// triangle 2;
	tx2.a = tri.a;
	tx2.b = tri.c;
	tx2.c = D;
	circle_cent2(pts[pa].r, pts[pa].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);
	tx2.ro = rot;
	tx2.R = R;
	tx2.C = C;

	tx2.ab = L2;
	tx2.ac = t;
	tx2.bc = L4;

	ids2.insert(t);
	ids2.insert(T2);

	t2 = tx2;
	tri = tx;
	flipped = 1;

	// change knock on triangle labels.
	if( L3 >= 0 ){
	  Triad &t3 = triads[L3];
	  if( t3.ab == T2 ) t3.ab = t;
	  else if( t3.bc == T2 ) t3.bc = t;
	  else if( t3.ac == T2 ) t3.ac = t;
	}

	if(L2 >= 0 ){
	  Triad &t4 = triads[L2];
	  if( t4.ab == t ) t4.ab = T2;
	  else if( t4.bc == t ) t4.bc = T2;
	  else if( t4.ac == t ) t4.ac = T2;
	}
	//if( triads[26777].ab == 27004 && triads[26777].bc == 27004)
	//  cerr << " feck 1 " << endl;
	}
      }
    }


    if( flipped == 0 && tri.ab >= 0 ){

      pc = slump[tri.c];
      pb = slump[tri.b];
      pa = slump[tri.a];

      T2 = tri.ab;
      Triad &t2 = triads[T2];
      // find relative orientation (shared limb).
      if( t2.ab == t ){
	D = t2.c;
	pd = slump[t2.c];

	if( tri.a == t2.a){
	  L3 = t2.ac;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ac;
	}
      }
      else if(  t2.ac == t ){
	D = t2.b;
	pd = slump[t2.b];

	if( tri.a == t2.a){
	  L3 = t2.ab;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ab;
	}
      }
      else if(  t2.bc == t ){
	D = t2.a;
	pd = slump[t2.a];

	if( tri.a == t2.b){
	  L3 = t2.ab;
	  L4 = t2.ac;
	}
	else{
	  L3 = t2.ac;
	  L4 = t2.ab;
	}
      }
      else{
	cerr << "fuck up at line 16 dude! " << t <<  endl;
      }

      r3 = pts[pd].r;
      c3 = pts[pd].c;
      dR = tri.R-r3;
      dC = tri.C-c3;


      if( dR*dR+dC*dC < tri.ro ){  // not valid in the Delaunay sense.
	L1 = tri.ac;
	L2 = tri.bc;
      	if( L1 != L3 && L2 != L4 ){  // need this check for stability.
	circle_cent2(pts[pa].r, pts[pa].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);

	tx.a = tri.c;
	tx.b = tri.a;
	tx.c = D;
	tx.ro = rot;
	tx.R = R;
	tx.C = C;

	tx.ab = L1;
	tx.ac = T2;
	tx.bc = L3;


	// triangle 2;
	tx2.a = tri.c;
	tx2.b = tri.b;
	tx2.c = D;
	circle_cent2(pts[pb].r, pts[pb].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);
	tx2.ro = rot;
	tx2.R = R;
	tx2.C = C;

	tx2.ab = L2;
	tx2.ac = t;
	tx2.bc = L4;

	if( (tx.ab == 26777 && tx.ac == 26777 && tx.bc == 26777) ||
	    (tx2.ab == 26777 && tx2.ac == 26777 && tx2.bc == 26777) ){
	  int foon = 8;
	}


	ids2.insert(t);
	ids2.insert(T2);

	t2 = tx2;
	tri = tx;
	flipped = 1;

	// change knock on triangle labels.
	if( L3 >= 0 ){
	  Triad &t3 = triads[L3];
	  if( t3.ab == T2 ) t3.ab = t;
	  else if( t3.bc == T2 ) t3.bc = t;
	  else if( t3.ac == T2 ) t3.ac = t;
	}

	if(L2 >= 0 ){
	  Triad &t4 = triads[L2];
	  if( t4.ab == t ) t4.ab = T2;
	  else if( t4.bc == t ) t4.bc = T2;
	  else if( t4.ac == t ) t4.ac = T2;
	}
	///if( triads[26777].ab == 27004 && triads[26777].bc == 27004)
	//  cerr << " feck 2 " << endl;
	}
      }
    }


    if( flipped == 0 && tri.ac >= 0 ){

      pc = slump[tri.c];
      pb = slump[tri.b];
      pa = slump[tri.a];

      T2 = tri.ac;
      Triad &t2 = triads[T2];
      // find relative orientation (shared limb).
      if( t2.ab == t ){
	D = t2.c;
	pd = slump[t2.c];

	if( tri.a == t2.a){
	  L3 = t2.ac;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ac;
	}
      }
      else if(  t2.ac == t ){
	D = t2.b;
	pd = slump[t2.b];

	if( tri.a == t2.a){
	  L3 = t2.ab;
	  L4 = t2.bc;
	}
	else{
	  L3 = t2.bc;
	  L4 = t2.ab;
	}
      }
      else if(  t2.bc == t ){
	D = t2.a;
	pd = slump[t2.a];

	if( tri.a == t2.b){
	  L3 = t2.ab;
	  L4 = t2.ac;
	}
	else{
	  L3 = t2.ac;
	  L4 = t2.ab;
	}
      }
      else{
	cerr << "fuck up at line 17 dude! " << t << endl;
      }

      r3 = pts[pd].r;
      c3 = pts[pd].c;
      dR = tri.R-r3;
      dC = tri.C-c3;

      float scubit = dR*dR+dC*dC - tri.ro;

      if( scubit < 0.0 ){  // not valid in the Delaunay sense.
	L1 = tri.ab;   // .ac shared limb
	L2 = tri.bc;
      	if( L1 != L3 && L2 != L4 ){  // need this check for stability.

	if( -scubit*1000 > tri.ro )
	  circle_cent2(pts[pa].r, pts[pa].c, pts[pb].r, pts[pb].c, r3, c3, R, C , rot);
	else{
	  circle_cent4(pts[pa].r, pts[pa].c, pts[pb].r, pts[pb].c, r3, c3, R, C , rot);
	  //cerr << "duff " << t << " scubit " << scubit << endl;
	}

	tx.a = tri.b;
	tx.b = tri.a;
	tx.c = D;
	tx.ro = rot;
	tx.R = R;
	tx.C = C;

	tx.ab = L1;
	tx.ac = T2;
	tx.bc = L3;


	// triangle 2;
	tx2.a = tri.b;
	tx2.b = tri.c;
	tx2.c = D;


	if( -scubit*1000 > tri.ro )
	  circle_cent2(pts[pb].r, pts[pb].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);
	else{
	  circle_cent2(pts[pb].r, pts[pb].c, pts[pc].r, pts[pc].c, r3, c3, R, C , rot);
	  //cerr << "duff " << t << " scubit " << scubit << endl;
	}

	tx2.ro = rot;
	tx2.R = R;
	tx2.C = C;

	tx2.ab = L2;
	tx2.ac = t;
	tx2.bc = L4;


	ids2.insert(t);
	ids2.insert(T2);

	t2 = tx2;
	tri = tx;

	// change knock on triangle labels.
	if( L3 >= 0 ){
	  Triad &t3 = triads[L3];
	  if( t3.ab == T2 ) t3.ab = t;
	  else if( t3.bc == T2 ) t3.bc = t;
	  else if( t3.ac == T2 ) t3.ac = t;
	}

	if(L2 >= 0 ){
	  Triad &t4 = triads[L2];
	  if( t4.ab == t ) t4.ab = T2;
	  else if( t4.bc == t ) t4.bc = T2;
	  else if( t4.ac == t ) t4.ac = T2;
	}

	//if( triads[26777].ab == 27004 && triads[26777].bc == 27004)
	//  cerr << " feck 3" << endl;
	}
      }
    }

    //write_Triads(triads, "tflip1.mat");
    int df = 9;
    df = 8;
  }


  //  cerr << " triangles to resolve " << ids2.size() << endl;
  ids.clear();
  ids.insert(ids2.begin(), ids2.end());

  return;
}


void circle_cent4(float r1,float c1, float r2,float c2, float r3,float c3,
		  float &r,float &c, float &ro2){
  /*
   *  function to return the center of a circle and its radius
   * degenerate case should never be passed to this routine!!!!!!!!!!!!!
   * but will return r0 = -1 if it is.
   */

  double rd, cd;
  double v1 = 2*(r2-r1), v2 = 2*(c2-c1), v3 = r2*r2 - r1*r1 + c2*c2 - c1*c1;
  double v4 = 2*(r3-r1),
    v5 = 2*(c3-c1),
    v6 = r3*r3 - r1*r1 + c3*c3 - c1*c1,

    v7 =  v2*v4 - v1*v5;
  if( v7 == 0 ){
    r=0;
    c=0;
    ro2 = -1;
    return;
  }

  cd = (v4*v3 - v1*v6)/v7;
  if( v1 != 0 )
    rd = (v3 - c*v2)/v1;
  else
    rd = (v6 - c*v5)/v4;

  ro2 = (float)  ( (rd-r1)*(rd-r1) + (cd-c1)*(cd-c1) );
  r = (float) rd;
  c = (float) cd;

  return;
}

void makeHull(std::vector<Angel::vec4> points){
	std::vector<Shx> pts, hull;
	Shx pt;


	for(int v=0; v<points.size(); v++){
		pt.id = v;
		if(!pointAlreadyExists(points[v],pts)){
		pt.r = points[v].x;
		pt.c = points[v].y;

		pts.push_back(pt);
		}
	}

	std::vector<Triad> triads;

	s_hull_del_ray2( pts, triads);

	std::cout << "Delaunay Triangles: " << triads.size() << std::endl;
}

bool pointAlreadyExists(Angel::vec4 point ,std::vector<Shx> points){
	for(int i=0; i<points.size(); i++){
		if((point.x == points[i].r)&&(point.y == points[i].c)){
			return true;
		}
	}
	return false;
}
