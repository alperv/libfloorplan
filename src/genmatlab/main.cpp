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
    D.loadGraphs("/home/gandalf/nbergst/Dropbox/MITData/maps.csail.mit.edu/corpus");
   // D.Load("small_corpus.dat");


    D.replaceCategory("LAB SV", "RS LAB");
    D.replaceCategory("RS LO", "RS LAB");

    D.replaceCategory("F LAV", "BATH");
    D.replaceCategory("M LAV", "BATH");
    D.replaceCategory("P LAV", "BATH");

    D.replaceCategory("OFF SV", "OFF");

   // D.RemoveIsolatedVertices();
  //  D.RemoveDisconnectedGraphs();
    D.removeCategoriesBasedonFrequency(500);

    // GraphFileOperations::saveGraphToPNG("first.png", D.getGraphs()[0]);

    floorplanGraph tmpgraph;
    GraphUtils::removeCategory("LAB SV",D.getGraphs()[0],tmpgraph);
    GraphFileOperations::saveGraphToPNG("second.png", tmpgraph);


   // D.Load("small_corpus.dat");

    cout << __FILE__ << ": " << __LINE__ << endl;
    GraphStatistics stat(&D);
    cout << stat.getNumberOfGraphs() << endl;
    cout << stat.getNumberOfVertices() << endl;
    cout << stat.getNumberofCategories() << endl;

    stat.generateAreaDist();
    stat.generateDegreeDistribution();

    stat.saveDistToFile("areadist.txt", stat.getAreaDist());
    stat.saveDistToFile("degreedist.txt", stat.getDegreeDistribution());


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
