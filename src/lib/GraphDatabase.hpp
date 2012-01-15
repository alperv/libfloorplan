/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "FloorPlanGraph.hpp"

// include headers that implement an archive
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <fstream>

#ifndef GRAPHDATABASE_HPP
#define GRAPHDATABASE_HPP

class GraphStatistics;

class GraphDatabase
{
private:

    /**
      * List of all graphs in this dataset
      */
    std::vector<floorplanGraph> _graphs;

    /**
      * Graph properties
      */
    std::vector<graphProperties> _graphProperties;
public:
    GraphDatabase();

    /**
      * Load dataset from a directory
      */
    void loadGraphs(std::string sDir, std::string rootNodeName = "MITquest", int iLimit = -1, bool append = false);

    const std::vector<floorplanGraph>& getGraphs(){ return _graphs; }
    const std::vector<graphProperties>& getGraphProperty(){ return _graphProperties; }

    /**
     * Serializes the class to a specified Archive.
     * This function uses boost serialization to serialize the class and all its data to disk.
     * @param ar The archive to which we will save the class to.
    */
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version);

    /**
     * Saves all the loaded graph in this graph database to disk.
    */
    void Save(std::string sFilename);

    /**
     * Loads all the loaded graphs in this graph database to disk-
    */
    void Load(std::string sFilename);

};



#endif // GRAPHDATABASE_HPP
