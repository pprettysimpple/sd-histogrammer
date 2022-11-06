#include "server.h"

#include <chrono>
#include <fstream>

#include <conf/config.h>

#include <onion/request.hpp>
#include <onion/response.hpp>


TFakeServer::TFakeServer(int mode, nlohmann::json&& data)
: Server(mode)
, Root(&Server)
, Data(std::move(data))
, ServedAdminRequestsCount(0)
, ServedSearchRequestsCount(0)
{

    // admin handlers
    Root.add("admin/stop", [this](Onion::Request &req, Onion::Response &res) {
        this->Stop();
        res << "ok";
        ServedAdminRequestsCount++;
        return OCS_PROCESSED;
    });

    Root.add("admin/stats", [this](Onion::Request &req, Onion::Response &res) {
        res << to_string(ServeStats());
        ServedAdminRequestsCount++;
        return OCS_PROCESSED;
    });

    Root.add("admin/flush-stats", [this](Onion::Request &req, Onion::Response &res) {
        res << to_string(ServeStats());
        ServedAdminRequestsCount = 0;
        ServedSearchRequestsCount = 0;
        return OCS_PROCESSED;
    });

    // user handlers
    Root.add("method/newsfeed.search", [this](Onion::Request &req, Onion::Response &res) {
        return this->ServeNewsfeedSearch(req, res);
    });
}

void TFakeServer::Listen() {
    Server.listen();
}

void TFakeServer::Stop() {
    Server.listenStop();
}

nlohmann::json TFakeServer::ServeStats() {
    auto stats = nlohmann::json::object_t();
    stats["ServedAdminRequestsCount"] = ServedAdminRequestsCount;
    stats["ServedSearchRequestsCount"] = ServedSearchRequestsCount;
    return stats;
}

onion_connection_status_e TFakeServer::ServeNewsfeedSearch(Onion::Request& request, Onion::Response& response) {
    auto reqJSON = nlohmann::json::parse(request.query().toJSON());

    if (reqJSON["access_token"] != FAKE_SERVER_ACCESS_TOKEN) {
        response.setCode(HTTP_BAD_REQUEST);
        response << "Wrong access token\n";
        return OCS_FORBIDDEN;
    }

    if (reqJSON["v"] != "5.131") {
        response.setCode(HTTP_BAD_REQUEST);
        response << "Unsupported version of API\n";
        return OCS_NOT_IMPLEMENTED;
    }

    if (!reqJSON.contains("q")) {
        response.setCode(HTTP_BAD_REQUEST);
        response << "Search string is required\n";
        return OCS_NOT_PROCESSED;
    }

    uint64_t startTime = 0;
    uint64_t endTime = std::numeric_limits<uint64_t>::max();
    uint64_t count = std::numeric_limits<uint64_t>::max();

    if (reqJSON.contains("start_time")) {
        std::string valStr;
        reqJSON["start_time"].get_to(valStr);
        startTime = std::atol(valStr.c_str());
    }

    if (reqJSON.contains("end_time")) {
        std::string valStr;
        reqJSON["end_time"].get_to(valStr);
        endTime = std::atol(valStr.c_str());
    }

    if (reqJSON.contains("count")) {
        std::string valStr;
        reqJSON["count"].get_to(valStr);
        count = std::atol(valStr.c_str());
    }

    auto items = ServeNewsfeedSearchImpl(startTime, endTime, count, reqJSON["q"].get<std::string>());

    auto respJSON = nlohmann::json::object();
    respJSON["response"] = nlohmann::json::object();
    respJSON["response"]["count"] = items.size();
    respJSON["response"]["items"] = items;

    response.setHeader("Content-Type", "application/json");
    response << to_string(respJSON);
    return OCS_PROCESSED;
}

nlohmann::json TFakeServer::ServeNewsfeedSearchImpl(uint64_t start, uint64_t end, uint64_t count, const std::string& query) {
    const nlohmann::json& items = Data.at("/response/items"_json_pointer);
    auto result = nlohmann::json::array();

    for (const auto& item : items) {
        auto time = item.at("date").get<uint64_t>();
        auto txt = item.at("text").get<std::string>();
        if (time >= start
            && time < end
            && txt.find(query) != txt.npos) {
            result.push_back(item);
        }
    }

    return result;
}

nlohmann::json LoadData(const std::string& path) {
    std::fstream fs(path, std::ios::in);
    auto data = std::string(
            std::istreambuf_iterator<char>(fs),
            std::istreambuf_iterator<char>());

    return nlohmann::json::parse(data);
}
