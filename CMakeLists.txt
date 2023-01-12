cmake_minimum_required(VERSION 3.16)
project(TSP_algo)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(${PROJECT_SOURCE_DIR}/src)

add_executable(TSP_algo main.cpp)
target_link_libraries(TSP_algo Library)

target_include_directories(TSP_algo PUBLIC ${PROJECT_SOURCE_DIR}/include)