cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(HaiLing_Optical_Simulation)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED on) 
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Geant4 REQUIRED)
find_package(yaml-cpp)
include(${Geant4_USE_FILE})

file(GLOB SOURCE_FILE ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(PointSourceShell 
               ${PROJECT_SOURCE_DIR}/PointSourceShellMain.cc
               ${SOURCE_FILE})

target_include_directories(PointSourceShell PRIVATE
                           ${PROJECT_SOURCE_DIR}/include
                           ${PROJECT_SOURCE_DIR}/config)

target_link_libraries(PointSourceShell PRIVATE
                      ${Geant4_LIBRARIES}
                      stdc++fs
                      yaml-cpp)