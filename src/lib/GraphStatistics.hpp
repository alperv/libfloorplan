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
  * Path length distribution for all labels
  * it's distribution with 100 intervals, each interval is 5 square meter,
  * meaning first interval is for rooms with 0-5 msq, second interval 5-10 etc.
  */
    DistVectorByLabel _areaDistByLabel;

    std::vector<std::string> _labels;

   static inline double polygonArea(double *X, double *Y, int points) {

      double  area=0. ;
      int     i, j=points-1  ;

      for (i=0; i<points; i++) {
        area+=(X[j]+X[i])*(Y[j]-Y[i]); j=i; }

      return area*.5;
    }
public:
    GraphStatistics();
    GraphStatistics(GraphDatabase* database);
    void saveDistToFile(std::string filename, DistVectorByLabel dist);
    void generateDegreeDist();
    void generateClusterCoeffDist();
    void generateAreaDist();

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

    const DistVectorByLabel& getDegreeDist() { return _degreeDistByLabel;}
    const DistVectorByLabel& getAreaDist(){ return _areaDistByLabel;}



};

#endif // GRAPHSTATISTICS_H
