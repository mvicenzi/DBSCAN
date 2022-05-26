#include "DBSCAN.h"
#include "StructDefinitions.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

/////////////////////////////////////////
//// Constructor
/////////////////////////////////////////

DBSCAN::DBSCAN(std::vector<Point> db){ m_db = db; }

DBSCAN::~DBSCAN(){}

////////////////////////////////////////
/// Main DBSCAN algorithm
////////////////////////////////////////

///This function implements DBSCAN, based on the pseudocode from this paper:
///ACM Trans. Database Syst. 42 (3): 19:1–19:21. doi:10.1145/3068335
void DBSCAN::Compute(int minpts, double eps,  DistanceOption dist){

  m_minpts = minpts;
  m_eps    = eps;
  m_dist   = dist;
  
  int cluster_counter = 0; //initialize cluster counter

  for(int i=0; i<m_db.size(); i++){ //iterate thrrough all points

    if( m_db[i].label != UNDEFINED) continue; //point already processed in the inner loop, skip
   
    std::vector<Point> neighbors = RangeScan(m_db,m_db[i]);
    //search for neighbors   
 
    //non-core points are initially  noise
    if (neighbors.size() < m_minpts){
      m_db[i].label = NOISE;
      continue;
    }

    cluster_counter++;
    m_db[i].label = cluster_counter; //point belongs to cluster

    //and its neighbors are possible seeds!
    std::vector<Point> seeds; //seeds to expand cluster: all neighbors except the original one
    for(int j=0; j<neighbors.size(); j++)
      if(neighbors[j].id != m_db[i].id) seeds.push_back(neighbors[j]);

    for(int j=0; j<seeds.size(); j++){

      if( seeds[j].label == NOISE) //if noise, change to cluster
		m_db[seeds[j].id].label = cluster_counter;

      if( seeds[j].label != UNDEFINED) continue; //if not undefined, continue

      std::vector<Point> nneighbors = RangeScan(m_db,seeds[j]); //search for neighbors of the neighbor
      m_db[seeds[j].id].label = cluster_counter;

      if(nneighbors.size() < m_minpts) continue; //the seed is border point, but not core point

      //if seed is core point, add its neighbors to the seeds
      for(int r=0; r<nneighbors.size(); r++){
        
	bool isthere = false;
        for(int t=0; t<seeds.size(); t++) //check if it is already inside seed
          	if(nneighbors[r].id == seeds[t].id) isthere = true;

        if(!isthere) seeds.push_back(nneighbors[r]);
      }
    }
  }

  std::cout << "Number of clusters found: " << cluster_counter << std::endl;
  AssignClusters(); 

}


/////////////////////////////////////////////////////
/// Support functions for the main algorithm
////////////////////////////////////////////////////


///This function computes the distance between two points according
///to the chosen metric (kEuclid, kManhattan, kChebyshev)
double DBSCAN::ComputeDistance(Point p1, Point p2){

  double distance = 0;

  //for all the coordinates
  for(int i=0; i<p1.x.size(); i++){
    
    //euclidian distance: sum of squared differences
    if( m_dist == DistanceOption::kEuclid){
      distance += pow(p1.x[i]-p2.x[i],2);
    
    //manhattan distance: sum of the differences (absolute values)
    }else if ( m_dist == DistanceOption::kManhattan){
      distance += abs(p1.x[i]-p2.x[i]);
    
    //chebyshev distance: max among the differences (absolute values)
    }else if ( m_dist == DistanceOption::kChebyshev){
      if(i==0) distance = abs(p1.x[i]-p2.x[i]);
      else if( abs(p1.x[i]-p2.x[i]) > distance) distance = abs(p1.x[i]-p2.x[i]);
    }
  }

  if( m_dist == DistanceOption::kEuclid)
    return sqrt(distance);
  else return distance;
}

///This function returns all neighboring points by selecting all points within
///the scan radius according to the chosen distance metric
std::vector<Point> DBSCAN::RangeScan(std::vector<Point> db, Point p){

  std::vector<Point> neighbors;
  for(int i=0; i<db.size(); i++){
      if (ComputeDistance(db[i],p)<=m_eps)
        neighbors.push_back(db[i]);
  }

  return neighbors;
}

///This functions scans each point, putting it into a cluster structure
///according to the label previously assigned
void DBSCAN::AssignClusters(){

  for(int i=0; i< m_db.size(); i++){
	
    int cluster_index = m_db[i].label;
    if( m_clusters.find(cluster_index) == m_clusters.end()){
	
	Cluster cl;
	cl.id = cluster_index;	
	cl.points.push_back(m_db[i]);

	m_clusters.insert(std::make_pair(cluster_index,cl));
    }else
	m_clusters[cluster_index].points.push_back(m_db[i]);

  }

}  


/////////////////////////////////////////////////////
/// Methods to return DBSCAN output
////////////////////////////////////////////////////

int DBSCAN::GetNClusters(){ return m_clusters.size();}
std::map<int, Cluster> DBSCAN::GetClusters(){ return m_clusters; }
Cluster DBSCAN::GetCluster(int index){ return m_clusters.at(index); }
 
void DBSCAN::PrintClusters(){

	for (auto it = m_clusters.begin(); it != m_clusters.end(); it++){
		
		std::cout << "----------> Cluster " << it->first << std::endl;
		std::cout << "Tot points: " << it->second.points.size() << std::endl;

		for( int j = 0; j<it->second.points.size(); j++ ){
			std::cout << "id " << it->second.points[j].id << ": ";
			
			for(int k=0; k<it->second.points[j].x.size(); k++) std::cout << it->second.points[j].x[k] << " ";
			std::cout << std::endl;
		}
	}
}
