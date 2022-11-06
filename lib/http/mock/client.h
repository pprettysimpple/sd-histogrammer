#pragma once

#include "lib/http/iface.h"

#include <curlpp/cURLpp.hpp>

#include <string>

struct TFakeClient : IClient {
    TFakeClient();
    std::string Get(const std::string& path) override;

    virtual ~TFakeClient() override = default;
};
