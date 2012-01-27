#include <iostream>

#include "GraphDatabase.hpp"
#include "GraphStatistics.hpp"
#include "GraphFileOperations.hpp"

using namespace std;
using namespace floorplan;

int main(){
    GraphDatabase D;
    D.loadGraphs("corpus/");
    //cout << "Replaced " << D.replaceCategory("RS LAB", "OFF") << " categories" << endl;

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
    stat.generateDegreeDist();
    stat.saveDistToFile("areadist.txt", stat.getAreaDist());
    stat.saveDistToFile("degreedist.txt", stat.getDegreeDist());

    return 0;
};
