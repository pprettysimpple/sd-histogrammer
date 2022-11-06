#pragma once

#include <map>
#include <vector>

#include <onion/onion.hpp>
#include <onion/url.hpp>

#include <nlohmann/json.hpp>

struct TFakeServer {
public:
    explicit TFakeServer(int mode, nlohmann::json&& data);

    void Listen();
    void Stop();

private:
    nlohmann::json ServeStats();

    onion_connection_status_e ServeNewsfeedSearch(Onion::Request& request, Onion::Response& response);
    nlohmann::json ServeNewsfeedSearchImpl(uint64_t start, uint64_t end, uint64_t count, const std::string& query);

private:
    // Base server
    Onion::Onion Server;
    Onion::Url Root;

    // Data
    nlohmann::json Data;

    // Metrics
    size_t ServedAdminRequestsCount;
    size_t ServedSearchRequestsCount;
};

nlohmann::json LoadData(const std::string& path);
