ExternalProject_Add(libonion
        GIT_REPOSITORY  https://github.com/davidmoreno/onion
        GIT_TAG         master
        CMAKE_ARGS      -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
        )

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)


add_library(FakeServer STATIC
        server.cpp
        )

target_link_libraries(FakeServer onion onioncpp nlohmann_json::nlohmann_json)
add_dependencies(FakeServer libonion nlohmann_json::nlohmann_json)


add_executable(FakeServerExecutable
        main.cpp
        )
target_link_libraries(FakeServerExecutable FakeServer)
add_dependencies(FakeServerExecutable FakeServer)
