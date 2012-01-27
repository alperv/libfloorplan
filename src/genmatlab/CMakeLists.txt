include_directories(../lib)
ADD_EXECUTABLE(genmatlab main.cpp)


TARGET_LINK_LIBRARIES(genmatlab floorplan)
install(TARGETS genmatlab DESTINATION ${OUTPUT})
