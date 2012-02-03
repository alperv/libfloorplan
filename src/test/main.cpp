#include <iostream>

#include "GraphDatabase.hpp"
#include "GraphStatistics.hpp"
#include "GraphFileOperations.hpp"

using namespace std;
using namespace floorplan;

int main(){
    GraphDatabase D;
   // D.loadGraphs("corpus/");
   // D.Init();
   // D.Save("corpus_processed.dat");

    D.Load("corpus_processed.dat");
    GraphStatistics stat(&D);
    stat.printGraphDatabaseStatistics();
    stat.createFileAttributesList();
    stat.createFilePairwiseCounts();
    return 1;

  //  stat.generateAreaDist();
    stat.generateDegreeDistByLabel();
    stat.generateCategoryCountDist();
    stat.generateAverageGraphPathLengthDist();
    stat.generateDegreeDistribution();
   // stat.calculateAverageClusterCoeffient();
    //stat.saveDistToFile("raw_areadist.txt", stat.getAreaDist());
   // stat.saveDistToFile("tmp/raw_degreedist.txt", stat.getDegreeDist());
   // stat.saveDistToFile("tmp/raw_categorycountdist.txt", stat.getCategoryCountDist());
   // stat.saveDistToFile("tmp/raw_averagedegree.txt", stat.getAveragePathLength());
   // stat.saveDistToFile("raw_degreedistribution.txt",stat.getDegreeDistribution());
    cout << "cluster coeff " << stat.getAverageClusterCoefficient() << endl;
    return 0;
}
