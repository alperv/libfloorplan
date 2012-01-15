#include "GraphDatabase.hpp"
#include "GraphStatistics.hpp"

#include <iostream>
#include <boost/graph/iteration_macros.hpp>
using namespace std;
int main(){
    GraphDatabase D;
    D.loadGraphs("small_corpus/");
   // D.Load("small_corpus.dat");


    GraphStatistics stat(&D);
    cout << stat.getNumberOfGraphs() << endl;
    cout << stat.getNumberOfVertices() << endl;
    stat.generateAreaDist();
    stat.generateDegreeDist();
    stat.saveDistToFile("areadist.txt", stat.getAreaDist());
    stat.saveDistToFile("degreedist.txt", stat.getDegreeDist());

    return 0;
};
