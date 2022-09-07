cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(HaiLing_Optical_Simulation)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

find_package(Geant4 REQUIRED)
find_package(yaml-cpp)
include(${Geant4_USE_FILE})

file(GLOB SOURCE_FILE ${PROJECT_SOURCE_DIR}/src/*.cc)

add_executable(OpticalSimulation 
               ${SOURCE_FILE})

target_include_directories(OpticalSimulation PRIVATE
                           ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(OpticalSimulation PRIVATE
                      ${Geant4_LIBRARIES}
                      yaml-cpp)

install(TARGETS OpticalSimulation RUNTIME
        DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${CMAKE_SOURCE_DIR}/config/config.yaml
        DESTINATION ${CMAKE_INSTALL_PREFIX})