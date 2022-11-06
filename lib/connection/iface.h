#pragma once

#include <cstdint>
#include <vector>
#include <string_view>
#include <future>
#include <optional>

struct IConnector {
    IConnector() = default;

    IConnector(const IConnector&) = delete;
    IConnector& operator=(const IConnector&) = delete;

    virtual std::vector<uint64_t> GetHist(const char* hashtag, uint64_t hours) {
        std::vector<uint64_t> result;
        result.resize(hours);

        using namespace std::chrono;
        auto now = system_clock::now();
        for (uint64_t i = 0; i < hours; i++) {
            uint64_t fromTs;
            uint64_t toTs;

            auto from = (now - std::chrono::hours(i + 1)).time_since_epoch();
            fromTs = duration_cast<seconds>(from).count();
            auto to = (now - std::chrono::hours(i)).time_since_epoch();
            toTs = duration_cast<seconds>(to).count();

            result[i] = GetHistImpl(hashtag, fromTs, toTs);
        }

        return result;
    }

    virtual uint64_t GetHistImpl(const char* hashtag, uint64_t from, uint64_t to) = 0;

    virtual ~IConnector() = default;
};

using IConnectorPtr = std::shared_ptr<IConnector>;