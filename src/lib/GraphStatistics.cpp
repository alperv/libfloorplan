/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "GraphStatistics.hpp"
#include <boost/graph/iteration_macros.hpp>

using namespace std;
using namespace boost;

GraphStatistics::GraphStatistics()
{

}

// ---------------------------------------------------------------------
GraphStatistics::GraphStatistics(GraphDatabase* database)
{
    _database = database;
    _labels.clear();
    _degreeDistByLabel.clear();
    _pathLengthDistByLabel.clear();

    /* Fill up the _labels container */
    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
        BGL_FORALL_VERTICES(v, _database->getGraphs()[i], floorplanGraph){
            vector<string>::iterator it = find(_labels.begin(), _labels.end(), _database->getGraphs()[i][v].category);
            if (it == _labels.end()){
                  _labels.push_back(_database->getGraphs()[i][v].category);
            }
        }
    }
}

// ---------------------------------------------------------------------
int GraphStatistics::getNumberOfVertices(){
    int ret = 0;
    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
        BGL_FORALL_VERTICES(v, _database->getGraphs()[i], floorplanGraph){
            cout << _database->getGraphs()[i][v].vertex_id << endl;
            ret++;
        }
    }
    return ret;
}

// ---------------------------------------------------------------------
int GraphStatistics::getNumberOfEdges(){
    int ret = 0;
    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
        BGL_FORALL_EDGES(e, _database->getGraphs()[i], floorplanGraph){
            ret++;
        }
    }
    return ret;
}


// ---------------------------------------------------------------------
void GraphStatistics::generateDegreeDist(){
    for(unsigned int i = 0; i < _labels.size(); i++)
        _degreeDistByLabel[_labels[i]] = vector<double>(100, 0);

    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
        floorplanGraph G = _database->getGraphs()[i];
        BGL_FORALL_VERTICES(v, G, floorplanGraph){
            int degree = in_degree(v, G);
            cout << G[v].category << " degree: " << degree << endl;
            if (degree < 100)
                _degreeDistByLabel[G[v].category][degree]++;
        }
    }
}


// ---------------------------------------------------------------------
void GraphStatistics::generateClusterCoeffDist(){
    for(unsigned int i = 0; i < _labels.size(); i++)
        _clusterCoeffDistByLabel[_labels[i]] = vector<double>(10, 0);

//    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
//        floorplanGraph G = _database->getGraphs()[i];
//        BGL_FORALL_VERTICES(v, _database->getGraphs()[i], floorplanGraph){
//            double clustercoeff = clustering_coefficient(G[v], G);
//        }
//    }
}


// ---------------------------------------------------------------------
void GraphStatistics::generateAreaDist(){
    for(unsigned int i = 0; i < _labels.size(); i++)
        _areaDistByLabel[_labels[i]] = vector<double>(100, 0);

    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
        floorplanGraph G = _database->getGraphs()[i];
        BGL_FORALL_VERTICES(v, _database->getGraphs()[i], floorplanGraph){
            vector<LineSegment> roomLayout;
            roomLayout = G[v].roomLayout;
            int pointSize = roomLayout.size();
            double* xpoints = new double[pointSize];
            double* ypoints = new double[pointSize];
            for (unsigned int j=0; j< roomLayout.size(); j++){
             //   cout << G[v].vertex_id <<  "," << roomLayout[j].startPos.x << "," << roomLayout[j].startPos.y << endl;
                xpoints[j] = roomLayout[j].startPos.x;
                ypoints[j] = roomLayout[j].startPos.y;
            }
            double roomArea = abs(polygonArea(xpoints, ypoints, pointSize))* 0.09290304; // feet square to meter square
            int interval = roomArea/5;
            if (interval >= 0 && interval < 100){
                _areaDistByLabel[G[v].category][interval]++;
            }
            delete xpoints;
            delete ypoints;
        }
    }
}


// ---------------------------------------------------------------------
double GraphStatistics::getArea(const Space& r){
    vector<LineSegment> roomLayout;
    roomLayout = r.roomLayout;
    int pointSize = roomLayout.size();
    double* xpoints = new double[pointSize];
    double* ypoints = new double[pointSize];
    for (unsigned int j=0; j< roomLayout.size(); j++){
        xpoints[j] = roomLayout[j].startPos.x;
        ypoints[j] = roomLayout[j].startPos.y;
    }
    double roomArea = abs(polygonArea(xpoints, ypoints, pointSize))* 0.09290304; // feet square to meter square
    return roomArea;
}


// ---------------------------------------------------------------------
int GraphStatistics::getDegree(Vertex v, const floorplanGraph& G){
    return in_degree(v, G);
}



// ---------------------------------------------------------------------
void GraphStatistics::saveDistToFile(string filename, DistVectorByLabel dist){
    std::ofstream out(filename.c_str() );
    DistVectorByLabel::iterator it = dist.begin();

    for (; it != dist.end(); ++it){
        out << it->first << " ";
        for (unsigned int i = 0; i < it->second.size(); i++){
            out << it->second[i] << " ";
        }
        out << endl;
    }
    out.close();
}
