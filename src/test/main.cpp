#include <iostream>

#include "GraphDatabase.hpp"
#include "GraphStatistics.hpp"
#include "GraphFileOperations.hpp"

using namespace std;
using namespace floorplan;

int main(){
 GraphDatabase D;

 // Use "floor" for the KTH dataset
 // and "MITquest" for the MIT dataset
 // Load the KTH dataset
 D.loadGraphs("kth_dataset", "floor");

 // Load the MIT dataset
 D.loadGraphs("maps.csail.mit.edu", "MITquest");

 GraphStatistics stat1(&D);
 stat1.printGraphDatabaseStatistics();
 stat1.generateCategoryCountDist();
 stat1.saveDistToFile("raw_categorycountdist.txt", stat1.getCategoryCountDist());

 D.Init();
 GraphStatistics stat2(&D);
 stat2.printGraphDatabaseStatistics();
    return 1;

//    D.Load("corpus_processed.dat");
    D.Load("corpus_raw.dat");

    GraphStatistics stat(&D);
//    stat.printGraphDatabaseStatistics();
//    stat.createFileAttributesList();
//    stat.createFilePairwiseCounts();
//    return 1;

  //  stat.generateAreaDist();
    stat.generateDegreeDistByLabel();
    stat.generateCategoryCountDist();
    stat.generateAverageGraphPathLengthDist();
    stat.generateDegreeDistribution();
   // stat.calculateAverageClusterCoeffient();
    //stat.saveDistToFile("raw_areadist.txt", stat.getAreaDist());
   // stat.saveDistToFile("tmp/raw_degreedist.txt", stat.getDegreeDist());
    stat.saveDistToFile("raw_MIT_categorycountdist.txt", stat.getCategoryCountDist());
   // stat.saveDistToFile("tmp/raw_averagedegree.txt", stat.getAveragePathLength());
   // stat.saveDistToFile("raw_degreedistribution.txt",stat.getDegreeDistribution());
    cout << "cluster coeff " << stat.getAverageClusterCoefficient() << endl;
    return 0;
}
