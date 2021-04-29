set (CNPY_SOURCES cnpy.cpp)

set(CNPY_HEADERS cnpy.hpp)

add_library(cnpy STATIC ${CNPY_SOURCES})

# target dependencies on other libraries (also the header onlys)
target_include_directories(cnpy INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    message(STATUS "ZLIB found")
else()
    message(FATAL_ERROR "ZLIB not found")
endif()

target_link_libraries(cnpy PUBLIC ZLIB::ZLIB)
