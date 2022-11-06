# CURLPP
include(FindPkgConfig)
pkg_check_modules(CURLPP REQUIRED curlpp)

include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)

add_library(HttpClient
        client.cpp
        )

target_link_libraries(HttpClient curlpp curl)
