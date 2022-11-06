include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_library(VKConnector
        connector.cpp
        )

target_link_libraries(VKConnector PRIVATE HttpClient nlohmann_json::nlohmann_json)

