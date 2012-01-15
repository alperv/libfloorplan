/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "GraphDatabase.hpp"
#include "GraphFileOperations.hpp"


#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/graph/adj_list_serialize.hpp>

using namespace std;


//----------------------------------------------------------------
GraphDatabase::GraphDatabase()
{
}


//----------------------------------------------------------------
void GraphDatabase::loadGraphs(string sDir, string rootNodeName, int iLimit, bool append){
    if (append)
        _graphs = vector<floorplanGraph>();
    _graphProperties = loadAllGraphsInFolder(sDir, _graphs, iLimit, rootNodeName);
}


//----------------------------------------------------------------
template<class Archive>
void GraphDatabase::serialize(Archive &ar, const unsigned int version)
{

    ar & _graphs;
    ar & _graphProperties;
}


//----------------------------------------------------------------
void GraphDatabase::Save(string sFilename)
{
    ofstream ofs(sFilename.c_str(), ios::binary);

    boost::archive::binary_oarchive oa(ofs);
    oa << (*this);
    ofs.close();
}

//----------------------------------------------------------------
void GraphDatabase::Load(string sFilename)
{
    std::ifstream ifs(sFilename.c_str(), ios::binary);


    boost::archive::binary_iarchive oa(ifs);
    oa >> (*this);

    ifs.close();
}


