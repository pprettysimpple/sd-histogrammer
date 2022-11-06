#pragma once

#include <sstream>

#include <lib/connection/iface.h>
#include <lib/http/iface.h>

class TFakeConnector : public IConnector {
public:
    TFakeConnector() noexcept;

    uint64_t GetHistImpl(const char* hashtag, uint64_t fromTs, uint64_t toTs) override;
private:
    uint64_t Counter;
};
