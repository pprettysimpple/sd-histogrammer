cmake_minimum_required(VERSION 3.23)
project(Histogrammer
        LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)

include(ExternalProject)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

# GOOGLETEST
ExternalProject_Add(googletest
        GIT_REPOSITORY  https://github.com/google/googletest
        GIT_TAG         main
        CMAKE_ARGS      -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        )

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)


# includes
include_directories(.)

add_subdirectory(lib)
add_subdirectory(tests/vk/fake-server)

# TESTS
add_executable(TestAll
        tests/vk/tests.cpp
        )
add_dependencies(TestAll googletest FakeServer)
target_link_libraries(TestAll
        gtest gtest_main pthread
        VKConnector
        FakeServer
        FakeClient
        FakeConnector)

# examples
add_executable(SimpleQuery
        examples/simple-query.cpp
        )
target_link_libraries(SimpleQuery
        VKConnector
        Utils
        )