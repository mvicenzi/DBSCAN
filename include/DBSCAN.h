#ifndef DBSCAN_h
#define DBSCAN_h

#include "StructDefinitions.h"
#include <vector>
#include <string>

/// \class DBSCAN
/// \brief Class implementing the DBSCAN algorithm
/// DBSCAN requires three parameters: the mininum number of neighbors to start a cluster (density threshold),
/// the maximum distance to scan for neighbors (radius) and the choice of a notion of distance between points.

class DBSCAN{

public:

  ///Constructor
  DBSCAN(int MinPts, double Epsilon,  DistanceOption DistOpt);
  ///Destructor
  ~DBSCAN();
  ///Run DBSCAN on input database
  void Compute(std::vector<Point> &input_database);

private:

  ///Scan database for neighbors of a given point
  std::vector<Point> RangeScan(std::vector<Point> db, Point p);
  ///Compute the distance between two points
  double ComputeDistance(Point p1, Point p2);

  int _MinPts; ///< density threshold
  double _Epsilon; ///< radius
  DistanceOption _DistOpt; ///< distance option
  std::vector<Cluster> m_Clusters; ///< output clusters

};
#endif
