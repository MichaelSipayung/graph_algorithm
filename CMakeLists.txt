cmake_minimum_required(VERSION 3.29)
project(graph_algorithm)
set(CMAKE_CXX_STANDARD 23)
add_library(algo_graph)
target_sources(
    algo_graph
    PUBLIC
        FILE_SET CXX_MODULES FILES
        algo_graph.cppm   
)
add_executable(${PROJECT_NAME} algo_graph_test.cc)
target_link_libraries(${PROJECT_NAME} PRIVATE algo_graph)