cmake_minimum_required(VERSION 3.6)


project(atlasBasedSegmentation)

set(CMAKE_CXX_STANDARD 11)

find_package(ITK REQUIRED)

include(${ITK_USE_FILE} atlasBasedSegmentation.h)
add_executable(atlasBasedSegmentation atlasBasedSegmentation.cpp atlasBasedSegmentation.h)
add_executable(splitter splitter.cpp)
target_link_libraries(atlasBasedSegmentation ${ITK_LIBRARIES})
target_link_libraries(splitter ${ITK_LIBRARIES})


include(${ITK_USE_FILE} interpolateInterleaves.h)
add_executable(interpolateInterleaves interpolateInterleaves.cpp)
target_link_libraries(interpolateInterleaves ${ITK_LIBRARIES})


include(${ITK_USE_FILE} affine3dRegistration.h)
add_executable(affine3dRegistration affine3dRegistration.cpp)
target_link_libraries(affine3dRegistration ${ITK_LIBRARIES})

include(${ITK_USE_FILE})
add_executable(scratch scratch.cpp)
target_link_libraries(scratch ${ITK_LIBRARIES})

include(${ITK_USE_FILE} convertor.h)
add_executable(convertor convertor.cpp)
target_link_libraries(convertor ${ITK_LIBRARIES})

include(${ITK_USE_FILE} applyMask.h)
add_executable(applyMask applyMask.cpp)
target_link_libraries(applyMask ${ITK_LIBRARIES})

include(${ITK_USE_FILE} transform.h)
add_executable(transform transform.cpp)
target_link_libraries(transform ${ITK_LIBRARIES})

include(${ITK_USE_FILE} deformableRegistration.h)
add_executable(deformableRegistration deformableRegistration.cpp)
target_link_libraries(deformableRegistration ${ITK_LIBRARIES})