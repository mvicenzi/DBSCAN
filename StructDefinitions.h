#ifndef StructDefinitions_h
#define StructDefinitions_h

#include <vector>

////////////////////////////////////////////////////////
// This file header defines some useful structures

#define UNDEFINED -1
#define NOISE 0

/// \struct Point
/// \brief A point in d-dimensional space
/// The point is described by its set of coordinates, a label (undefined, noise, cluster number)
/// and an id (unique identifier)
struct Point{

    std::vector<double> x;  ///< spatial coordinates
    int label;  ///< flag for point status (undefined, noise, cluster label)
    int id; ///< unique identifier

};

/// \struct Cluster
/// \brief A cluster of d-dimensional points
struct Cluster{
    std::vector<Point> points; ///< points in cluster
    int id; ///< unique identifier
};

#endif
