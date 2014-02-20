#include <iostream>

#include "GraphDatabase.hpp"
#include "GraphStatistics.hpp"
#include "GraphFileOperations.hpp"

#include <boost/graph/iteration_macros.hpp>

using namespace std;
using namespace floorplan;

void saveGraphToMatlab(std::string filenamePath, const floorplanGraph& Graph);

int main(){
    GraphDatabase D;
    D.loadGraphs("/home/gandalf/nbergst/Dropbox/MITData/maps.csail.mit.edu/corpus", "MITquest");
    D.Init();

    GraphStatistics stat(&D);
    stat.printGraphDatabaseStatistics();
    stat.generateAreaDist();
    stat.generateDegreeDistribution();

    stat.saveDistToFile("areadist.txt", stat.getAreaDist());
    stat.saveDistToFile("degreedist.txt", stat.getDegreeDistribution());

    stat.createFileAttributesList();
    stat.createFilePairwiseCounts();

    std::vector<floorplanGraph> graphs =  D.getGraphs();
    
    for( int i=0; i<graphs.size(); ++i ) {
      std::string name;
      char n[256];
      sprintf(n,"%05d\0",i);
      name += "matlab/floorplan_";
      name += n;
      name += ".txt";

      cout << "Saving " << name << std::endl;
      
      //saveGraphToMatlab(std::string filenamePath, const floorplanGraph& Graph);
      GraphFileOperations::saveGraphToMatlab(name, graphs[i]);

    }
    
    return 0;
};
