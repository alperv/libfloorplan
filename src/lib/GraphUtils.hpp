/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "FloorPlanGraph.hpp"
#include <boost/graph/iteration_macros.hpp>

#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

using namespace std;

namespace floorplan {
class GraphUtils {
public:
    static pair<bool, Vertex> doesVertexExists(string id, floorplanGraph& g){
        BGL_FORALL_VERTICES(v, g, floorplanGraph){
            if(g[v].vertex_id.compare(id) == 0){
                return make_pair(true, v);
            }
        }

        return make_pair(false, Vertex());
    }

    /*
  * Remove vertex from a graph and connect its neighbors to each other with a normal edge
  * E.g. if a graph with nodes A,B and C are connected as A--B--C and we want to remove B
  * the new graph looks like A--C
  */
    static void removeVertex(Vertex vertexToRemove, const floorplanGraph& oldGraph, floorplanGraph& outGraph){

        map<Vertex, Vertex> oldtoOutGraph;

        BGL_FORALL_VERTICES(v, oldGraph, floorplanGraph){
            if (v != vertexToRemove){
                Vertex currentVertex = add_vertex(outGraph);
                outGraph[currentVertex] =  oldGraph[v];
              //  cout << "Adding vertex " << oldGraph[v].vertex_id << " " << v << endl;
                oldtoOutGraph[v] = currentVertex;
            }
            else{
              //  cout << "Removing vertex " << oldGraph[v].vertex_id << " " <<  v << endl;
            }
        }

        BGL_FORALL_EDGES(e, oldGraph, floorplanGraph){
            Vertex sourceVertex = boost::source(e, oldGraph);
            Vertex targetVertex = boost::target(e, oldGraph);
            if (vertexToRemove != targetVertex && vertexToRemove != sourceVertex && sourceVertex != targetVertex){
            //    cout << "Adding edge " << oldGraph[sourceVertex].vertex_id << " " << oldGraph[targetVertex].vertex_id << endl;
            //    cout << sourceVertex << " " << targetVertex << endl;
                add_edge(oldtoOutGraph[sourceVertex], oldtoOutGraph[targetVertex], outGraph);
            }
        }


        vector<Vertex> removedVertexNeighbours;

        BGL_FORALL_OUTEDGES(vertexToRemove, e, oldGraph, floorplanGraph) {
            Vertex targetVertex = boost::target(e, oldGraph);
            //    cout << "Adding neighbor " << oldGraph[targetVertex].vertex_id << " " << targetVertex << endl;
            if (vertexToRemove != targetVertex){
                removedVertexNeighbours.push_back(targetVertex);
            }
        }

        //cout << oldtoOutGraph.size() << " neighbors: " << removedVertexNeighbours.size() << endl;

        for (unsigned int i=0; i < removedVertexNeighbours.size(); i++){
            // Connect all the neighbors of vertexToRemove to each other
            Vertex currentNeighbor = removedVertexNeighbours[i];

            for (unsigned int j=0; j < removedVertexNeighbours.size(); j++){
                if (i != j){
                    Vertex secondNeighbor = removedVertexNeighbours[j];
                    if (!edge(oldtoOutGraph[currentNeighbor], oldtoOutGraph[secondNeighbor], outGraph).second
                            && currentNeighbor != secondNeighbor){
                        add_edge(oldtoOutGraph[currentNeighbor], oldtoOutGraph[secondNeighbor], outGraph);
                    }
                }
            }
        }
    }

    static void removeCategory(std::string categoryToRemove, const floorplanGraph& oldGraph, floorplanGraph& outGraph){
        vector<string> verticesToRemove;
        BGL_FORALL_VERTICES(v, oldGraph, floorplanGraph){
            if (oldGraph[v].category.compare(categoryToRemove) == 0){
                verticesToRemove.push_back(oldGraph[v].vertex_id);
            }
        }

        outGraph = oldGraph;
        for (unsigned int i=0; i < verticesToRemove.size(); i++){
            floorplanGraph tmpGraph;
            removeVertex(doesVertexExists(verticesToRemove[i], outGraph).second, outGraph, tmpGraph);
            outGraph = tmpGraph;
        }

    }

};
}
#endif // GRAPHUTILS_H
