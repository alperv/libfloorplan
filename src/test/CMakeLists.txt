include_directories(../lib)
ADD_EXECUTABLE(APITest main.cpp)


TARGET_LINK_LIBRARIES(APITest floorplan)
install(TARGETS APITest DESTINATION ${OUTPUT})
