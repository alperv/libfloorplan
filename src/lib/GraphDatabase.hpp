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


namespace floorplan {
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
    const std::vector<graphProperties>& getGraphProperties(){ return _graphProperties; }


    /**
      * Remove those categories that occur less than @freqThreshold times in the database
      *
      */

    void removeCategoriesBasedonFrequency(int freqThreshold);

    /**
      *  Swap a category with another category label, typically this is done
      * if two category labels indicate semantically the same type of space but have
      * different labels for one reason or another (e.g. swap male bathroom to simply bathroom)
      * @return: number of spaces affected by this.
      */
   int replaceCategory(std::string oldCategory, std::string newCategory);

   /**
     * Remove graphs based on size, graphs smaller than @sizeThreshold will be removed
     * @return: number of graphs removed
     */
   int removeGraphsSmallerThan(int sizeThreshold);

   /**
     * Remove vertices that do not have any edges
     */
   int removeIsolatedVertices();


   /**
     * Loop through all the spaces in the database and if the number of their neighbors
     * is greater or equal than @degreeThreshold, then set its category to newCategory.
     * This is useful when say, a corridor-like place is called many names, a "circulation area"
     * a "hallway" etc. but in terms of observable features they appear the same.
     * @return: number of spaces affected by this.
     */
   int mergeCentralNodes(int degreeThreshold, std::string newCategory);


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

    /**
      * Filter and prepare graphs
      */
    void Init();


};

}

#endif // GRAPHDATABASE_HPP
