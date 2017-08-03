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

include(${ITK_USE_FILE})
add_executable(applyMask3d applyMask3d.cpp)
target_link_libraries(applyMask3d ${ITK_LIBRARIES})

include(${ITK_USE_FILE} transform.h)
add_executable(transform transform.cpp)
target_link_libraries(transform ${ITK_LIBRARIES})

include(${ITK_USE_FILE} subtract2d.h)
add_executable(subtract2d subtract2d.cpp)
target_link_libraries(subtract2d ${ITK_LIBRARIES})

include(${ITK_USE_FILE} subtract3d.h)
add_executable(subtract3d subtract3d.cpp)
target_link_libraries(subtract3d ${ITK_LIBRARIES})

include(${ITK_USE_FILE} evalMaskOverlap.h)
add_executable(evalMaskOverlap evalMaskOverlap.cpp)
target_link_libraries(evalMaskOverlap ${ITK_LIBRARIES})

include(${ITK_USE_FILE} deformableRegistration.h)
add_executable(deformableRegistration deformableRegistration.cpp)
target_link_libraries(deformableRegistration ${ITK_LIBRARIES})

include(${ITK_USE_FILE})
add_executable(deformableRegistration2 DeformableRegistration2.cpp)
target_link_libraries(deformableRegistration2 ${ITK_LIBRARIES})

include(${ITK_USE_FILE})
add_executable(deformableRegistration3 DeformableRegistration3.cpp)
target_link_libraries(deformableRegistration3 ${ITK_LIBRARIES})

include(${ITK_USE_FILE})
add_executable(deformableRegistration4 DeformableRegistration4.cpp)
target_link_libraries(deformableRegistration4 ${ITK_LIBRARIES})

include(${ITK_USE_FILE})
add_executable(deformableRegistration5 DeformableRegistration5.cpp)
target_link_libraries(deformableRegistration5 ${ITK_LIBRARIES})

include(${ITK_USE_FILE})
add_executable(testt test.cpp)
target_link_libraries(testt ${ITK_LIBRARIES})

include(${ITK_USE_FILE})
add_executable(flip3d flip3d.cpp)
target_link_libraries(flip3d ${ITK_LIBRARIES})