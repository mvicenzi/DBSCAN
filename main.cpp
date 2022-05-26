#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include "StructDefinitions.h"
#include "DBSCAN.h"

////////////////////////////////
/// Default values: filename is always required from user
/// DBSCAN parameters are optional, default values are used if not specified by user

std::string filename = "";
int MinPts = 4;
double Epsilon = 2;
DistanceOption DistOpt = DistanceOption::kEuclid;

///////////////////////////////////
/// This function explains usage

void usage(){
    std::cout << "Usage: ./dbscan -f inputfile [-m minpts] [-e epsilon] [-d distopt]" << std::endl;
    std::cout << "           -f input file name in the required format" << std::endl;
    std::cout << "           -m minimum number of points for clustering (see DBSCAN reference)" << std::endl;
    std::cout << "           -e maximum radius for neighbors search (see DBSCAN reference)" << std::endl;
    std::cout << "           -d distance metric: Euclid (default), Manhattan, Chebyshev" << std::endl;
}

///////////////////////////////////
/// This function parses the user input
int GetCommandLineArgs(int argc, char **argv){

    int fflag=0; int opt;
    std::map<std::string,DistanceOption> distMap = {
        { "euclid", kEuclid }, { "Euclid", kEuclid },
        { "manhattan", kManhattan }, { "Manhattan", kManhattan },
        { "chebyshev", kChebyshev }, { "Chebyshev", kChebyshev },
    };      

    while( (opt = getopt(argc, argv, "m:e:d:f:")) != -1){
        switch(opt){
            case 'f':
                if(optarg) filename = optarg;
                fflag = 1;
                break;
            case 'm':
                if(optarg) MinPts = std::atoi(optarg);
                break;
            case 'e':
                if(optarg) Epsilon = std::atof(optarg);
                break;
            case 'd':
                if(optarg) DistOpt = distMap[optarg];
                break;
            default: 
                usage();               
        }
    }

    if(!fflag)
        return 0;
    else
        return 1;
}

///////////////////////////////////
/// Convert distance-metric name
std::string ToString(DistanceOption opt){
    std::map<DistanceOption,std::string> Map = {
        { kEuclid, "Euclid" }, { kManhattan, "Manhattan" }, { kChebyshev, "Chebyshev" }};
    return Map[opt];
}

///////////////////////////////////
/// This function reads the input database of points
/// It assumes an input-file format

void GetInputFromFile(std::string filename, std::vector<Point> &input_database){

  std::ifstream ifile;
  ifile.open(filename.c_str());
  if(!ifile)
    std::cout << "ERROR: Cannot find/open input file" << std::endl;

  //The input file contains the points n-coordinates
  //pnt1_x1 pnt1_x2 .... pnt1_xn
  //pnt2_x1 pnt2_x2 .... pnt2_xn
  //....
  int point_ID = 0;
  std::string line;

  //extract a line
  while( std::getline(ifile,line)){
    Point buffer;
    //set point coordinates
    std::istringstream sline(line);
    std::string number;
    while (std::getline(sline, number, ' ')){
        buffer.x.push_back(std::stod(number));
    }
    //set point unique ID
    buffer.id = point_ID;
    //set point label as UNDEFINED
    buffer.label = UNDEFINED;

    input_database.push_back(buffer);
    point_ID++;
    }

  std::cout << "There are " << input_database.size() << " input points." << std::endl;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv){

    //get args from command line
    int redflag = GetCommandLineArgs(argc, argv);
    if(!redflag){
        std::cout << "ERROR: missing REQUIRED parameter(s)!" << std::endl;
        usage();
        return 1;
    }

    std::cout << "*************************" << std::endl;
    std::cout << "Input file: " << filename << std::endl;
    std::cout << "DBSCAN parameters: " << std::endl;
    std::cout << "      MinPts: " << MinPts << std::endl;
    std::cout << "     Epsilon: " << Epsilon << std::endl;
    std::cout << "     DistOpt: " << ToString(DistOpt) << std::endl;
    std::cout << "*************************" << std::endl;
    
    std::vector<Point> input_database;

    GetInputFromFile(filename, input_database);
    if(input_database.size() == 0)  return 1;
    
    DBSCAN ClusterMaker(input_database);
    ClusterMaker.Compute(MinPts, Epsilon, DistOpt);
	
    //print output
    ClusterMaker.PrintClusters();

  return 0;
}
