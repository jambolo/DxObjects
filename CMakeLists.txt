cmake_minimum_required (VERSION 3.4)
project(DxObjects CXX)

option(BUILD_SHARED_LIBS "Build libraries as DLLs" FALSE)

set(${PROJECT_NAME}_DOXYGEN_OUTPUT_DIRECTORY "" CACHE PATH "Doxygen output directory (empty to disable)")

if(${PROJECT_NAME}_DOXYGEN_OUTPUT_DIRECTORY)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_OUTPUT_DIRECTORY ${${PROJECT_NAME}_DOXYGEN_OUTPUT_DIRECTORY})
        configure_file(Doxyfile.in ${PROJECT_BINARY_DIR}/Doxyfile)
        add_custom_target(doxygen ALL ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
            SOURCES ${PROJECT_BINARY_DIR}/Doxyfile
    )
    endif(DOXYGEN_FOUND)
endif()

#set(${PROJECT_NAME}_VERSION_MAJOR 0)
#set(${PROJECT_NAME}_VERSION_MINOR 1)
#configure_file("${PROJECT_SOURCE_DIR}/Version.h.in" "${PROJECT_BINARY_DIR}/Version.h")

set(PUBLIC_INCLUDE_PATHS
    include
)

set(SOURCES
    include/DxObjects/Axes.h
    include/DxObjects/LensFlare.h
    include/DxObjects/SkyBox.h
    include/DxObjects/TerrainCamera.h
    
    Axes.cpp
    LensFlare.cpp
    SkyBox.cpp
    TerrainCamera.cpp
)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PUBLIC_INCLUDE_PATHS})
target_link_libraries(${PROJECT_NAME} PRIVATE Misc Wx DxUtility Dxx Math)