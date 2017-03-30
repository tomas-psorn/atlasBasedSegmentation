cmake_minimum_required(VERSION 3.6)


project(atlasBasedSegmentation)

set(CMAKE_CXX_STANDARD 11)

find_package(ITK REQUIRED)

include(${ITK_USE_FILE} atlasBasedSegmentation.h)


add_executable(atlasBasedSegmentation atlasBasedSegmentation.cpp affine2D.cpp labelInverseTransform.cpp)
add_executable(splitter splitter.cpp)


target_link_libraries(atlasBasedSegmentation ${ITK_LIBRARIES})
target_link_libraries(splitter ${ITK_LIBRARIES})
