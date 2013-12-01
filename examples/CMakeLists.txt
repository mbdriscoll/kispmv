include_directories(${CMAKE_SOURCE_DIR}/include ${MKL_LIBRARIES})
link_directories(${CMAKE_BINARY_DIR}/lib)

add_executable(bfs bfs.cpp)
add_executable(spmv spmv.cpp)
add_executable(csrmm csrmm.cpp)
#add_executable(nbody nbody.cpp)

target_link_libraries(bfs kispmv)
target_link_libraries(spmv kispmv ${MKL_LIBRARIES})
target_link_libraries(csrmm kispmv)
#target_link_libraries(nbody kispmv)
