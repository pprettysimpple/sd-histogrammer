#include "client.h"

#include <cassert>
#include <sstream>
#include <iostream>

//#include <curlpp/Options.hpp>

TFakeClient::TFakeClient()
{
}

std::string TFakeClient::Get(const std::string& path) {
    std::string target;
    target += "/method/newsfeed.search?";
    target += "q=%23cat&";
    target += "start_time=24&";
    target += "end_time=42&";
    target += "count=200&";
    target += "access_token=<token>&";
    target += "v=5.131";
    assert(path == target);
    return R"({"response": {
        "count": 3,
        "items": [
            {"date": 25, "text": "#cats is my love"},
            {"date": 27, "text": "I love #cats"},
            {"date": 41, "text": "I AM #cat"}
        ]
    }})";
}
