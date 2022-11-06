#pragma once

#include "lib/http/iface.h"

#include <curlpp/cURLpp.hpp>

#include <string>

struct THttpClient : IClient {
    explicit THttpClient(std::string Host);

    std::string Get(const std::string& path) override;

    virtual ~THttpClient() override = default;

private:
    cURLpp::Cleanup Cleanup;
    std::string Host;
};
