/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "GraphDatabase.hpp"

// boost includes
#include <boost/graph/clustering_coefficient.hpp>
#include <boost/graph/exterior_property.hpp>

#ifndef GRAPHSTATISTICS_H
#define GRAPHSTATISTICS_H

typedef std::map< std::string, std::vector<double> > DistVectorByLabel;
typedef std::map< std::pair<std::string, std::string>, int> CategoryPairToIntMap;
namespace floorplan {
class GraphStatistics
{
private:
    GraphDatabase* _database;


    /**
  * Degree distribution for all labels
  */
    DistVectorByLabel _degreeDistByLabel;

    /**
  * Path length distribution for all labels
  */
    DistVectorByLabel _pathLengthDistByLabel;

    /**
  * Path length distribution for all labels
  */
    DistVectorByLabel _clusterCoeffDistByLabel;

    /**
  * Category count distribution over all labels
  */
    DistVectorByLabel _categoryCountDistByLabel;

    /**
      * Average path length
      * the int key is size of graph
      * double value is the average path length given for a graph size
      */
    std::map<int,double> _averagePathLengthByGraphSize;

    /**
      * Degree distribution
      * Counts of how many nodes (double value in the map) has N edges (int key)
      */

    std::map<int,double> _degreeDistribution;

    CategoryPairToIntMap _pairwiseCount;
    /**
  * Path length distribution for all labels
  * it's distribution with 100 intervals, each interval is 5 square meter,
  * meaning first interval is for rooms with 0-5 msq, second interval 5-10 etc.
  */
    DistVectorByLabel _areaDistByLabel;

    /**
      * Set of all the room category labels.
      * Filled in the constructor.
      */
    std::vector<std::string> _labels;

    double _averageClusterCoefficient;

   static inline double polygonArea(double *X, double *Y, int points) {

      double  area=0. ;
      int     i, j=points-1  ;

      for (i=0; i<points; i++) {
        area+=(X[j]+X[i])*(Y[j]-Y[i]); j=i; }

      return area*.5;
    }

   bool isStringPairEqual(std::pair<std::string, std::string> p1, std::pair<std::string, std::string> p2);

public:
    GraphStatistics();
    GraphStatistics(GraphDatabase* database);
    void saveDistToFile(std::string filename, DistVectorByLabel dist);
    void saveDistToFile(std::string filename, std::map<int, double> dist);

    void generateDegreeDistByLabel();
    void generateClusterCoeffDist();
    void generateAreaDist();
    void generateCategoryCountDist();
    void generateAverageGraphPathLengthDist();
    void generateDegreeDistribution();
    void calculateAverageClusterCoeffient();

    void createFileAttributesList();
    void createFilePairwiseCounts();

    /**
      * Area of a space
      */
    double getArea(const Space& r);

    /**
      * Number of connections of a space
      */
    int getDegree(Vertex v, const floorplanGraph& G);


    /**
      * Number of Graphs in the whole database
      */
    int getNumberOfGraphs() { return _database->getGraphs().size(); }

    /**
      * Number of all the vertices of all the graphs in the whole database
      */
    int getNumberOfVertices();

    /**
      * Number of all the edges of all the graphs in the whole database
      */
    int getNumberOfEdges();

    int getNumberofCategories() {return _labels.size();}

    void printGraphDatabaseStatistics();

    /**
      * Getters
      */
    const DistVectorByLabel& getDegreeDistByLabel() { return _degreeDistByLabel;}
    const DistVectorByLabel& getAreaDist(){ return _areaDistByLabel;}
    const DistVectorByLabel& getCategoryCountDist(){ return _categoryCountDistByLabel;}
    const std::map<int,double>& getAveragePathLength(){return _averagePathLengthByGraphSize;}
    const std::map<int,double>& getDegreeDistribution(){return _degreeDistribution;}
    double getAverageClusterCoefficient(){return _averageClusterCoefficient;}

};
}

#endif // GRAPHSTATISTICS_H
