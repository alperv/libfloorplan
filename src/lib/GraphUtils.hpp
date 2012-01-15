/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/

#include "FloorPlanGraph.hpp"
#include <boost/graph/iteration_macros.hpp>

#ifndef GRAPHUTILS_H
#define GRAPHUTILS_H

using namespace std;

pair<bool, Vertex> doesVertexExists(string id, floorplanGraph& g){
    BGL_FORALL_VERTICES(v, g, floorplanGraph){
        if(g[v].vertex_id.compare(id) == 0){
            return make_pair(true, v);
        }
    }

    return make_pair(false, Vertex());
}

#endif // GRAPHUTILS_H
