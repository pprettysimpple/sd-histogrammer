#pragma once

#include <sstream>

#include <lib/connection/iface.h>
#include <lib/http/iface.h>

struct TVKAuthSettings {
    std::string ClientId;
    std::string PublicToken;
};

class TVKConnector : public IConnector {
public:
    TVKConnector(IClient& client, TVKAuthSettings settings);

    uint64_t GetHistImpl(const char* hashtag, uint64_t fromTs, uint64_t toTs) override;
private:

    IClient& Client;
    TVKAuthSettings Settings;
};

TVKAuthSettings LoadSettings(const std::string& directoryPath);