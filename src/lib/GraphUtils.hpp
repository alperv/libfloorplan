/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "FloorPlanGraph.hpp"
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <boost/graph/property_maps/container_property_map.hpp>
#include <boost/graph/clustering_coefficient.hpp>
#include <boost/graph/exterior_property.hpp>

#include <boost/graph/graphviz.hpp>
#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

using namespace std;

namespace floorplan {
class GraphUtils {
public:
    //----------------------------------------------------------------
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
    //----------------------------------------------------------------
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

    //----------------------------------------------------------------
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

    //----------------------------------------------------------------
    static std::pair<simpleGraph, std::map<string,int> > makesimpleGraph (const floorplanGraph& G){
        simpleGraph ret;
        map<string, int> fpGraphToSimpleGraph;
        BGL_FORALL_VERTICES(v, G, floorplanGraph){
            simpleVertex currentVertex = add_vertex(ret);
            fpGraphToSimpleGraph[G[v].vertex_id] = currentVertex;
        }
        BGL_FORALL_EDGES(e, G, floorplanGraph){
            Vertex sourceVertex = source(e, G);
            Vertex targetVertex = target(e,G);
            string sourceID =G[sourceVertex].vertex_id;
            string targetID = G[targetVertex].vertex_id;
            simpleVertex simpleSource = fpGraphToSimpleGraph[sourceID];
            simpleVertex simpleTarget = fpGraphToSimpleGraph[targetID];
            if (!edge(simpleSource,simpleTarget,ret).second)
                add_edge(simpleSource,simpleTarget, ret);
        }
        assert(boost::num_vertices(G) == boost::num_vertices(ret));
        assert(boost::num_edges(G) == boost::num_edges(ret));
        return make_pair(ret, fpGraphToSimpleGraph);
    }

    //----------------------------------------------------------------
    static double getAverageGraphPathLength(floorplanGraph& G)
    {
        simpleGraph simpleG = makesimpleGraph(G).first;
        return getAverageGraphPathLength(simpleG);
    }

    //----------------------------------------------------------------
    static double getAverageGraphPathLength(simpleGraph& simpleG)
    {
        boost::graph_traits < simpleGraph >::edge_iterator e, e_end;
        boost::property_map < simpleGraph, boost::edge_weight_t >::type w = boost::get(boost::edge_weight, simpleG);
        for (boost::tie(e, e_end) = edges(simpleG); e != e_end; ++e)
            w[*e] = 1;

        int numberofVertices = num_vertices(simpleG);
        // Output matrix.
        std::vector<std::vector<int> > D(numberofVertices,std::vector<int>(numberofVertices));
        boost::johnson_all_pairs_shortest_paths(simpleG,D);
        double retval = 0;
        for (std::vector<int>::size_type i = 0; i < D.size(); ++i) {
            for (std::vector<int>::size_type j = 0; j < D[i].size(); ++j) {
                if (D[i][j] < 100)
                    retval += D[i][j];
            }
        }
        if (numberofVertices < 2) {
            return 0;
        } else {
            return retval / (static_cast<double>(numberofVertices) * (numberofVertices - 1));
        }
    }

    //----------------------------------------------------------------
    static double getAverageMetricPathLength(floorplanGraph& G){

        BGL_FORALL_VERTICES(v, G, floorplanGraph){

        }

    }

    //----------------------------------------------------------------
    static std::pair<ClusteringMap, float> calculateClusterCoeff(const simpleGraph& G)
    {
        ClusteringContainer coefs(num_vertices(G));
        ClusteringMap cm(coefs, G);
        float cc = all_clustering_coefficients(G, cm);
        return make_pair(cm, cc);
    }

    //----------------------------------------------------------------
    static std::pair<ClusteringMap, float> calculateClusterCoeff(const floorplanGraph& G)
    {
        return calculateClusterCoeff(makesimpleGraph(G).first);
    }

};
}
#endif // GRAPHUTILS_H
