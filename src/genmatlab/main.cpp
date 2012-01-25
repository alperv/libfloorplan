#include "GraphDatabase.hpp"
#include "GraphStatistics.hpp"

#include <iostream>
#include <boost/graph/iteration_macros.hpp>
using namespace std;

void saveGraphToMatlab(std::string filenamePath, const floorplanGraph& Graph);

int main(){
    GraphDatabase D;
    D.loadGraphs("/home/gandalf/nbergst/Dropbox/MITData/maps.csail.mit.edu/corpus");
   // D.Load("small_corpus.dat");


    GraphStatistics stat(&D);
    cout << stat.getNumberOfGraphs() << endl;
    cout << stat.getNumberOfVertices() << endl;
    stat.generateAreaDist();
    stat.generateDegreeDist();

    stat.saveDistToFile("areadist.txt", stat.getAreaDist());
    stat.saveDistToFile("degreedist.txt", stat.getDegreeDist());


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
      saveGraphToMatlab(name, graphs[i]);

    }
    
    return 0;
};
