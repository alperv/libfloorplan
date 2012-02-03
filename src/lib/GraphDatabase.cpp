/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "GraphDatabase.hpp"
#include "GraphFileOperations.hpp"
#include "GraphUtils.hpp"

#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/graph/adj_list_serialize.hpp>

using namespace std;
namespace floorplan {

//----------------------------------------------------------------
GraphDatabase::GraphDatabase()
{
}


//----------------------------------------------------------------
void GraphDatabase::loadGraphs(string sDir, string rootNodeName, int iLimit, bool append){
    if (!append)
        _graphs = vector<floorplanGraph>();
    _graphProperties = GraphFileOperations::loadAllGraphsInFolder(sDir, _graphs, iLimit, rootNodeName);
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

//----------------------------------------------------------------
int GraphDatabase::replaceCategory(std::string oldCategory, std::string newCategory){
    cout << "Replacing category: " << oldCategory << "for: " << newCategory << endl;
    int nodesAffected = 0;
    for (unsigned int i=0; i < _graphs.size(); i++){
        BGL_FORALL_VERTICES(v, _graphs[i], floorplanGraph){
            if (_graphs[i][v].category.compare(oldCategory) == 0){
                _graphs[i][v].category = newCategory;
                nodesAffected++;
            }
        }
    }
    return nodesAffected;
}

//----------------------------------------------------------------
int  GraphDatabase::mergeCentralNodes(int degreeThreshold, std::string newCategory){
    int nodesAffected = 0;
    for (unsigned int i=0; i < _graphs.size(); i++){
        BGL_FORALL_VERTICES(v, _graphs[i], floorplanGraph){
            if (in_degree(v, _graphs[i]) >= degreeThreshold){
                _graphs[i][v].category = newCategory;
                nodesAffected++;
            }
        }
    }
    return nodesAffected;
}

//----------------------------------------------------------------
int  GraphDatabase::removeIsolatedVertices(){

    for (unsigned int i=0; i < _graphs.size(); i++){
        vector<string> verticesToRemove;
        BGL_FORALL_VERTICES(v, _graphs[i], floorplanGraph){
            if (in_degree(v, _graphs[i]) == 0){
                verticesToRemove.push_back(_graphs[i][v].vertex_id);
            }
        }

        for (unsigned int j=0; j < verticesToRemove.size(); j++){
            floorplanGraph tmpGraph;
            GraphUtils::removeVertex(GraphUtils::doesVertexExists(verticesToRemove[j], _graphs[i]).second, _graphs[i], tmpGraph);
            _graphs[i] = tmpGraph;
        }
    }
}

//----------------------------------------------------------------
int  GraphDatabase::removeGraphsSmallerThan(int sizeThreshold){
    int graphsRemoved = 0;
    vector<floorplanGraph>::iterator it = _graphs.begin();
    int k = 0;
    for (; it != _graphs.end(); ++it){
        if(num_vertices(*it) < sizeThreshold){
            _graphs.erase(it);
            _graphProperties.erase(_graphProperties.begin()+k);
            graphsRemoved++;
            k++;
        }
    }
    return graphsRemoved;
}

//----------------------------------------------------------------
void GraphDatabase::removeCategoriesBasedonFrequency(int freqThreshold){
    map<string, int> categoryCount;
    vector<string> labels;

    /* Fill up the labels and categoryCount container */
    for (unsigned int i = 0; i < _graphs.size(); i++){
        BGL_FORALL_VERTICES(v, _graphs[i], floorplanGraph){
            vector<string>::iterator it = find(labels.begin(), labels.end(), _graphs[i][v].category);
            if (it == labels.end()){
                  labels.push_back(_graphs[i][v].category);
                  categoryCount[_graphs[i][v].category] = 0;
            }
        }
    }

    // count all the categories in the database
    for (unsigned int i=0; i < _graphs.size(); i++){
        BGL_FORALL_VERTICES(v, _graphs[i], floorplanGraph){
                categoryCount[_graphs[i][v].category]++;
            }
        }
    // Determine categories to remove
    vector<string> categoriesToRemove;
    map<string, int>::iterator it;
    for (it = categoryCount.begin(); it != categoryCount.end(); ++it){
        if (it->second < freqThreshold)
            categoriesToRemove.push_back(it->first);
    }

    // Remove those categories from each graph in the database
    for (unsigned int i =0; i < categoriesToRemove.size(); i++){
        cout << "Removing category " << categoriesToRemove[i] << endl;
        for (unsigned int j = 0; j < _graphs.size(); j++){
            floorplanGraph tmpGraph = _graphs[j];
            GraphUtils::removeCategory(categoriesToRemove[i],tmpGraph, _graphs[j]);
        }
    }

}

//----------------------------------------------------------------
void GraphDatabase::Init(){
    cout << "Preparing graph database" << endl;
    replaceCategory("LAB SV", "RS LAB");
    replaceCategory("RES LO", "RS LAB");

    replaceCategory("F LAV", "BATH");
    replaceCategory("M LAV", "BATH");
    replaceCategory("P LAV", "BATH");

    replaceCategory("OFF SV", "OFF");
    replaceCategory("FOODSV", "FOOD");

   // D.RemoveIsolatedVertices();
  //  D.RemoveDisconnectedGraphs();
    mergeCentralNodes(3, "CORR");
    removeIsolatedVertices();
    removeGraphsSmallerThan(5);
    removeCategoriesBasedonFrequency(500);
}

}
