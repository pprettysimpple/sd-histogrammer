#include "client.h"

#include <sstream>

#include <curlpp/Options.hpp>

THttpClient::THttpClient(std::string host)
    : Cleanup()
    , Host(std::move(host))
{
}

std::string THttpClient::Get(const std::string& path) {
    std::ostringstream os;
    os << curlpp::options::Url(Host + path);
    return os.str();
}
