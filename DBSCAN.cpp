#include "DBSCAN.h"
#include "StructDefinitions.h"
#include <vector>
#include <cmath>
#include <iostream>

/////////////////////////////////////////
//// Constructor
/////////////////////////////////////////

DBSCAN::DBSCAN(int MinPts, double Epsilon,  DistanceOption DistOpt){

  _MinPts = MinPts;
  _Epsilon = Epsilon;
  _DistOpt = DistOpt;

}

DBSCAN::~DBSCAN(){
  //nothing to destroy here :)
}

////////////////////////////////////////
/// Main DBSCAN algorithm
////////////////////////////////////////

///This function implements DBSCAN, based on the pseudocode from
///this paper: ACM Trans. Database Syst. 42 (3): 19:1–19:21. doi:10.1145/3068335
void DBSCAN::Compute(std::vector<Point> input_database){

  int cluster_counter = 0;

  //iterate over every point
  for(int i=0; i<input_database.size(); i++){

    Point p = input_database[i];
    //skip processed points
    if( p.label != UNDEFINED) continue;
    //find initial neighbors
    std::vector<Point> neighbors = RangeScan(input_database,p);
    //non-core points are noise
    if (neighbors.size() < _MinPts){
      p.label = NOISE;
      continue;
    }
    //start new cluster
    cluster_counter++;
    p.label = cluster_counter;

    std::vector<Point> seed_set;
    for(int j=0; j<neighbors.size(); j++)
      if(neighbors[j].id != p.id) seed_set.push_back(neighbors[j]);

    //Expand neighborhood
    for(int j=0; j<seed_set.size(); j++){
      Point q = seed_set[j];
      if( q.label == NOISE) q.label = cluster_counter;
      if( q.label != UNDEFINED) continue;

      //Get neighbors of the new cluster point
      std::vector<Point> nneighbors = RangeScan(input_database,q);
      q.label = cluster_counter;
      //Core-point check
      if(nneighbors.size() < _MinPts) continue;

      for(int r=0; r<nneighbors.size(); r++){
        bool isAlreadyInSeed = false;
        for(int t=0; t<seed_set.size(); t++)
          if(nneighbors[r].id == seed_set[t].id) isAlreadyInSeed = true;
        if(!isAlreadyInSeed) seed_set.push_back(nneighbors[r]);
      }
    }
  }

std::cout << "Number of clusters found: " << cluster_counter << std::endl;

}


/////////////////////////////////////////////////////
/// Private support function for the main algorithm
////////////////////////////////////////////////////


///This function computes the distance between two points according
///to the chosen metric (kEuclid, kManhattan, kChebyshev)
double DBSCAN::ComputeDistance(Point p1, Point p2){

  double distance = 0;

  //for all the d coordinates
  for(int i=0; i<p1.x.size(); i++){
    //euclidian distance: sum of squared differences
    if(_DistOpt == DistanceOption::kEuclid)
      distance += pow(p1.x[i]-p2.x[i],2);
    //manhattan distance: sum of the differences (absolute values)
    else if (_DistOpt == DistanceOption::kManhattan)
      distance += abs(p1.x[i]-p2.x[i]);
    //chebyshev distance: max among the differences (absolute values)
    else if (_DistOpt == DistanceOption::kChebyshev){
      if(i==0) distance = abs(p1.x[i]-p2.x[i]);
      else if( abs(p1.x[i]-p2.x[i]) > distance) distance = abs(p1.x[i]-p2.x[i]);
    }
  }

  if(_DistOpt == DistanceOption::kEuclid)
    return sqrt(distance);
  else return distance;
}

///This function returns all neighboring points by selecting all points within
///the scan radius according to the chosen distance metric
std::vector<Point> DBSCAN::RangeScan(std::vector<Point> db, Point p){

  std::vector<Point> neighbors;
  for(int i=0; i<db.size(); i++)
      if (ComputeDistance(db[i],p)<=_Epsilon)
        neighbors.push_back(db[i]);

  return neighbors;
}
