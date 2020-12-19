#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "StructDefinitions.h"
#include "DBSCAN.h"

///////////////////////////////////
/// This function reads the input databse of points
/// It assumes the input is stored in a file

void GetInputFromFile(std::string filename, std::vector<Point> &input_database){

  std::ifstream ifile;
  ifile.open(filename.c_str());
  if(!ifile)
    std::cout << "Cannot find/open input file" << std::endl;

  //The input file contains the points n-coordinates
  //line 1: pnt1_x1 pnt1_x2 .... pnt1_xn
  //line 2: pnt2_x1 pnt2_x2 .... pnt2_xn
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


int main(int argc, char **argv){

    if(argc != 2){
      std::cout << "No input file selected!" << std::endl;
      return 1;
    }

    std::string filename(argv[1]);
    std::vector<Point> input_database;

    //TODO: check this function!!!
    GetInputFromFile(filename, input_database);

    //TODO: apply DBSCAN to input_databse

    //TODO: check/extract DBSCAN results



  return 0;
}
