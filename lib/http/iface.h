#pragma once

#include <string>
#include <memory>

struct IClient {
    virtual std::string Get(const std::string& path) = 0;

    virtual ~IClient() {}
};

using IClientPtr = std::shared_ptr<IClient>;