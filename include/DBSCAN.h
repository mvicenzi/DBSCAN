#ifndef DBSCAN_h
#define DBSCAN_h

#include "StructDefinitions.h"
#include <vector>
#include <string>
#include <map>

/// \class DBSCAN
/// \brief Class implementing the DBSCAN algorithm
/// DBSCAN requires three parameters: the mininum number of neighbors to start a cluster (density threshold),
/// the maximum distance to scan for neighbors (radius) and the choice of a notion of distance between points.

class DBSCAN{

public:

  ///Constructor
  DBSCAN(std::vector<Point> db);
  ///Destructor
  ~DBSCAN();
  
  ///Run DBSCAN on input database
  void Compute(int minpts, double eps,  DistanceOption dist);

  ///Scan for neighbors of a given point
  std::vector<Point> RangeScan(std::vector<Point> db, Point p);
  ///Compute the distance between two points
  double ComputeDistance(Point p1, Point p2);

  ///Assign points to clusters
  void AssignClusters();

  ///Return outputs
  int GetNClusters();
  std::map<int, Cluster> GetClusters();
  Cluster GetCluster(int index);
  void PrintClusters();

private:

  int m_minpts; ///< density threshold
  double m_eps; ///< radius
  DistanceOption m_dist; ///< distance option
  
  std::vector<Point> m_db; ///< input database
  std::map<int,Cluster> m_clusters; ///< output clusters

};
#endif
