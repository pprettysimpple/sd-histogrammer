#include "connector.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <chrono>

namespace {
    struct TQueryBuilder {
        std::string Build() {
            return "/method/" + Method + "?"
                + Params
                + "access_token=" + AccessToken + "&"
                + "v=" + Version;
        }

        TQueryBuilder& AddParam(std::string paramName, std::string paramValue) {
            Params += std::move(paramName) + "=" + std::move(paramValue) + "&";
            return *this;
        }

        std::string Method;
        std::string Params;
        std::string AccessToken;
        std::string Version = "5.131";
    };

    std::string BuildQuery(std::string hashtag, uint64_t fromTs, uint64_t toTs, const TVKAuthSettings& settings) {
        TQueryBuilder builder;
        builder.AccessToken = settings.PublicToken;
        builder.Method = "newsfeed.search";

        builder.AddParam("q", "%23" + std::move(hashtag));
        builder.AddParam("start_time", std::to_string(fromTs));
        builder.AddParam("end_time", std::to_string(toTs));
        builder.AddParam("count", std::to_string(200)); // this is maximum :(

        return builder.Build();
    }

    bool CheckResponseValid(const nlohmann::json& resp) {
        auto valuesPtr = "/response/items"_json_pointer;
        if (!resp.contains(valuesPtr)) {
            return false;
        }
        if (!resp[valuesPtr].is_array()) {
            return false;
        }
        return true;
    }

    uint64_t BuildResult(const nlohmann::json& resp) {
        using nlohmann::json;
        const json& posts = resp.at("/response/items"_json_pointer);

        return posts.size();
    }
}

TVKConnector::TVKConnector(IClient& client, TVKAuthSettings settings)
    : Client(client)
    , Settings(std::move(settings))
{
}

uint64_t TVKConnector::GetHistImpl(const char* hashtag, uint64_t fromTs, uint64_t toTs) {
    auto query = BuildQuery(hashtag, fromTs, toTs, Settings);

    try {
        auto responseText = Client.Get(query);
        auto responseJson = nlohmann::json::parse(responseText);

        if (!CheckResponseValid(responseJson)) {
            throw std::runtime_error("Invalid response");
        }

        return BuildResult(responseJson);
    } catch(...) {
        throw;
    }
}

TVKAuthSettings LoadSettings(const std::string& directorySettingsPath) {
    TVKAuthSettings settings;
    std::fstream fstreamClientId(directorySettingsPath + "/.app_id", std::ios::in);
    std::fstream fstreamPublicToken(directorySettingsPath + "/.public_token", std::ios::in);

    fstreamClientId     >> settings.ClientId;
    fstreamPublicToken  >> settings.PublicToken;

    return settings;
}
