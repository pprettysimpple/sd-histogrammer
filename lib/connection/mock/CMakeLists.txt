include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_library(FakeConnector
        connector.cpp
        )

target_link_libraries(FakeConnector nlohmann_json::nlohmann_json)
