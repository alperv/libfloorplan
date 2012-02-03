/**
  LibFloorplan
  Author: Alper Aydemir. Jan, 2012.
*/


#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/labeled_graph.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/clustering_coefficient.hpp>
#include <boost/graph/exterior_property.hpp>

#include <boost/property_map/property_map.hpp>

#include <boost/graph/property_maps/container_property_map.hpp>
#include <boost/graph/clustering_coefficient.hpp>
#include <boost/graph/exterior_property.hpp>

#ifndef FLOORPLANGRAPH_H
#define FLOORPLANGRAPH_H

/**
  * A point in 2D space.
  */
class Point2D{
public:
    Point2D(double x_, double y_): x(x_), y(y_) {}
    Point2D() { x = -1; y = -1; }
    double x;
    double y;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {

        ar & x;
        ar & y;
    }
};


/**
  * Each segment of a space's layout polygon
  */
class LineSegment{
public:
    LineSegment(){portalToRoom="";}
    LineSegment(Point2D startPos_, Point2D endPos_): startPos(startPos_), endPos(endPos_) { portalToRoom="";}
    Point2D startPos;
    Point2D endPos;
    std::string type;
    std::string portalToRoom;

    template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar & startPos;
            ar & endPos;
            ar & type;
            ar & portalToRoom;
        }
};

/**
  * A space is a segmented part of a floorplan. Can be a room such as an office or an area such as printer area.
  */
class Space {
public:
    std::string category;
    std::string vertex_id;
    std::vector<LineSegment> roomLayout;
    double maxx,maxy,minx,miny;
    Point2D centroid;

    template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {

            ar & category;
            ar & vertex_id;
            ar & roomLayout;
            ar & centroid;
        }
};

/**
  * An edge between two spaces corresponds to traversability between the two.
  */

enum EdgeClass {
    HORIZONTAL,
    VERTICAL
};
enum EdgeType {
    EXPLICIT_EDGE,
    IMPLICIT_EDGE
};

class spaceEdge {
public:
    std::string edge_id;
    EdgeClass edge_class;
    EdgeType edge_type;

    template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {

            ar & edge_id;
            ar & edge_class;
            ar & edge_type;
        }
};

/**
  * Various properties of a single floorplan.
  */
class graphProperties{
public:
    graphProperties(){ maxx = -1; maxy=-1; minx=-1; miny=-1;}

    std::string floorname;
    std::string filepath;
    double maxx,maxy,minx,miny;
    Point2D centroid;

    template<class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {

            ar & floorname;
            ar & filepath;
            ar & maxx;
            ar & maxy;
            ar & minx;
            ar & minx;
            ar & centroid;
        }
};

/**
  * FloorplanGraph description
  */
//typedef boost::labeled_graph<boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, Space, spaceEdge, graphProperties>, std::string> floorplanGraph;
typedef boost::adjacency_list<boost::listS, boost::listS, boost::undirectedS, Space, spaceEdge, graphProperties> floorplanGraph;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property< boost::edge_weight_t, int> > simpleGraph;

/**
  * Vertex and edge descriptors
  */
typedef boost::graph_traits<floorplanGraph>::vertex_descriptor Vertex;
typedef boost::graph_traits<floorplanGraph>::edge_descriptor Edge;
typedef boost::graph_traits<simpleGraph>::vertex_descriptor simpleVertex;
typedef boost::graph_traits<simpleGraph>::edge_descriptor simpleEdge;

/// The clustering property, container, and map define the containment
/// and abstract accessor for the clustering coefficients of vertices.
typedef boost::exterior_vertex_property<simpleGraph, float> ClusteringProperty;
typedef ClusteringProperty::container_type ClusteringContainer;
typedef ClusteringProperty::map_type ClusteringMap;

#endif // FLOORPLANGRAPH_H
