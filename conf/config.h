#pragma once

#include <string>

inline constexpr auto FAKE_SERVER_HOST = "http://localhost";
inline constexpr auto FAKE_SERVER_PORT = 8080;

inline const auto VK_AUTH_SETTINGS_PATH = "conf/auth-settings";
inline const auto FAKE_SERVER_CONN_STRING = std::string(FAKE_SERVER_HOST) + ":" + std::to_string(FAKE_SERVER_PORT);

inline const auto FAKE_SERVER_ACCESS_TOKEN = "CAFE_BABE";
