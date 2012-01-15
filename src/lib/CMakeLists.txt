

include_directories(/usr/include/boost)
find_package(Boost COMPONENTS graph serialization REQUIRED)


ADD_LIBRARY(floorplan STATIC
GraphStatistics.cpp
GraphDatabase.cpp
GraphFileOperations.hpp
GraphUtils.hpp
FloorPlanGraph.hpp)
SET_TARGET_PROPERTIES(floorplan PROPERTIES LINKER_LANGUAGE C)
TARGET_LINK_LIBRARIES( floorplan ${Boost_GRAPH_LIBRARY}  ${Boost_SERIALIZATION_LIBRARY})

install(TARGETS floorplan DESTINATION ${OUTPUT})

