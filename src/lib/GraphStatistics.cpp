/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "GraphStatistics.hpp"
#include "GraphFileOperations.hpp"
#include <boost/graph/iteration_macros.hpp>
#include "GraphUtils.hpp"
#include <math.h>

using namespace std;
using namespace boost;

namespace floorplan {
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
            //cout << _database->getGraphs()[i][v].vertex_id << endl;
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
void GraphStatistics::generateDegreeDistByLabel(){
    for(unsigned int i = 0; i < _labels.size(); i++)
        _degreeDistByLabel[_labels[i]] = vector<double>(100, 0);

    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
        floorplanGraph G = _database->getGraphs()[i];
        BGL_FORALL_VERTICES(v, G, floorplanGraph){
            int degree = in_degree(v, G);
            // cout << G[v].category << " degree: " << degree << endl;
            if (degree < 100)
                _degreeDistByLabel[G[v].category][degree]++;
        }
    }
}

// ---------------------------------------------------------------------
void GraphStatistics::generateCategoryCountDist(){
    for(unsigned int i = 0; i < _labels.size(); i++)
        _categoryCountDistByLabel[_labels[i]] = vector<double>(1, 0);

    /* Fill up the labels and categoryCount container */
    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
        floorplanGraph G = _database->getGraphs()[i];
        BGL_FORALL_VERTICES(v, G, floorplanGraph){
            _categoryCountDistByLabel[G[v].category][0]++;
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
        out << it->first << ", ";
        for (unsigned int i = 0; i < it->second.size(); i++){
            out << it->second[i] << " ";
        }
        out << endl;
    }
    out.close();
}

// ---------------------------------------------------------------------
void GraphStatistics::saveDistToFile(string filename, map<int,double> dist){

    ofstream out(filename.c_str() );
    map<int,double>::iterator it = dist.begin();

    for (; it != dist.end(); ++it){
        out << it->first << ", " << it->second << endl;
    }
    out.close();
}

// ---------------------------------------------------------------------
void GraphStatistics::generateAverageGraphPathLengthDist(){
    map<int, int> graphSizeCount; // how many of graphs size <key> is encountered
    int maxsize = 50;
    for(unsigned int i = 0; i < maxsize; i++){
        _averagePathLengthByGraphSize[i] = 0;
        graphSizeCount[i]=0;
    }

    for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
        floorplanGraph G = _database->getGraphs()[i];
        int graphsize = num_vertices(G);
      //  cout << "Checking graph" << _database->getGraphProperties()[i].filepath << "size: " << graphsize << endl;
        double currentPathLength = GraphUtils::getAverageGraphPathLength(G);
        if (graphsize > 2 && graphsize < maxsize && currentPathLength > 0){
            graphSizeCount[graphsize]++;
           // cout << currentPathLength << endl;
            _averagePathLengthByGraphSize[graphsize] = (_averagePathLengthByGraphSize[graphsize] + currentPathLength);
        }
    }

    for (unsigned int i=0; i<maxsize; i++){
      //  cout << "#"<< graphSizeCount[i] << " of size " << i << endl;
        if (graphSizeCount[i] > 1){
            _averagePathLengthByGraphSize[i] = _averagePathLengthByGraphSize[i]/ graphSizeCount[i];
        }
        else{
            _averagePathLengthByGraphSize[i] = -1;
        }
    }
}

// ---------------------------------------------------------------------
void GraphStatistics::generateDegreeDistribution(){
    int maxsize = 100;
    for(unsigned int i = 0; i < maxsize; i++){
        _degreeDistribution[i] = 0;
    }

     for (unsigned int i = 0; i < _database->getGraphs().size(); i++){
         floorplanGraph G = _database->getGraphs()[i];
         BGL_FORALL_VERTICES(v, G, floorplanGraph){
             _degreeDistribution[getDegree(v,G)]++;
         }
     }
}

//----------------------------------------------------------------
void GraphStatistics::calculateAverageClusterCoeffient()//ExportClusterCoeffDistForCategory(string sFilename, string sCategory)
{
    _averageClusterCoefficient = 0.0;
    int nanvalues = 0;

    for(int i=0; i < _database->getGraphs().size(); i++)
    {
        std::pair<ClusteringMap, double> kRes = GraphUtils::calculateClusterCoeff(_database->getGraphs()[i]);
        if (isnan(kRes.second)){
        nanvalues++;
            continue;
        }
        if( kRes.second > 0.4 && num_vertices(_database->getGraphs()[i]) > 20 )
        {
            cout << "High!" << endl;
            cout << kRes.second << endl;
            stringstream strs;
            strs << i;
            GraphFileOperations::saveGraphToPNG("high_" + strs.str(), _database->getGraphs()[i]);
        }
        else if( kRes.second == 0 && num_vertices(_database->getGraphs()[i]) > 20 )
        {
            cout << "Low!" << endl;
            cout << kRes.second << endl;
            if(kRes.second == 0){
                stringstream strs;
                strs << i;
                GraphFileOperations::saveGraphToPNG("low_" + strs.str(), _database->getGraphs()[i]);

            }

        }

        _averageClusterCoefficient += kRes.second;
        cout << kRes.second << " total " << _averageClusterCoefficient << endl;
    }
    _averageClusterCoefficient = _averageClusterCoefficient /  ( _database->getGraphs().size() - nanvalues);
}

//----------------------------------------------------------------
void  GraphStatistics::createFileAttributesList(){

    for(int i=0; i < _database->getGraphs().size(); i++)
    {    string attFilename = _database->getGraphProperties()[i].filepath + "_attributes.txt";
         ofstream out(attFilename.c_str() );
         BGL_FORALL_VERTICES(v, _database->getGraphs()[i], floorplanGraph ){
             out << _database->getGraphs()[i][v].vertex_id << ","<< in_degree(v, _database->getGraphs()[i]) << "," << getArea(_database->getGraphs()[i][v]) << endl;
         }
         out.close();
    }

}

bool GraphStatistics::isStringPairEqual(std::pair<std::string, std::string> p1, std::pair<std::string, std::string> p2){
    if ( (p1.first.compare(p2.first) == 0 && p1.second.compare(p2.second)) ||
         (p1.first.compare(p2.second) == 0 && p1.second.compare(p2.first)) ){
        return true;
    }
    else{
        return false;
    }
}

void GraphStatistics::printGraphDatabaseStatistics(){
    cout << "--- Graph Database statistics ---" << endl;
    cout << "# of Graphs: " << getNumberOfGraphs() << endl;
    cout << "# of Vertices: " <<getNumberOfVertices() << endl;
    cout << "# of Categories: " << getNumberofCategories() << endl;
    cout <<"# Labels: " << _labels.size() << endl;
    for (unsigned int i=0; i < _labels.size(); i++){
        cout << _labels[i] << endl;
    }
    cout << "--------------------------------" << endl;
}

//----------------------------------------------------------------
void  GraphStatistics::createFilePairwiseCounts(){

    for (unsigned int i=0; i < _labels.size(); i++){
        cout << _labels[i] << endl;
    }

    for(unsigned int i = 0; i < _labels.size(); i++){
        for(unsigned int j = i; j < _labels.size(); j++){
            _pairwiseCount[make_pair(_labels[i],_labels[j])] = 0;
        }
    }
    for(int i=0; i < _database->getGraphs().size(); i++)
    {
        floorplanGraph G = _database->getGraphs()[i];
        BGL_FORALL_EDGES(e, G, floorplanGraph ){
            string sourceVertex = G[source(e,G)].category;
            string targetVertex = G[target(e,G)].category;
            pair<string,string> catPair(sourceVertex, targetVertex);
            CategoryPairToIntMap::iterator it = _pairwiseCount.begin();
            for (;it!=_pairwiseCount.end();++it){
                if (isStringPairEqual(catPair, it->first)){
                    it->second++;
                }
            }
        }
     }

    CategoryPairToIntMap::iterator it = _pairwiseCount.begin();
    for (;it!=_pairwiseCount.end();++it){
        cout << it->first.first << ", " << it->first.second << ": " << it->second << endl;
    }

    ofstream out("pairwise_counts.txt");
    for (unsigned int i =0; i < _labels.size(); i++){
        for (unsigned int j=0; j< _labels.size(); j++){
            pair<string, string> catPair(_labels[i], _labels[j]);
            CategoryPairToIntMap::iterator it = _pairwiseCount.begin();
            for (;it!=_pairwiseCount.end();++it){
                if (isStringPairEqual(catPair, it->first)){
                    out << " " << it->second;
                }
            }
        }
    }
}


}
