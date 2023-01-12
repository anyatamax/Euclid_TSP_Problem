include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(Library SHARED ${PROJECT_SOURCE_DIR}/src/Graph.cpp 
            ${PROJECT_SOURCE_DIR}/src/Heap.cpp 
            ${PROJECT_SOURCE_DIR}/src/MinimumCostPerfectMatching/BinaryHeap.cpp
            ${PROJECT_SOURCE_DIR}/src/MinimumCostPerfectMatching/Graph.cpp
            ${PROJECT_SOURCE_DIR}/src/MinimumCostPerfectMatching/Matching.cpp)