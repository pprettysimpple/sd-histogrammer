#include <lib/connection/vk-api/connector.h>
#include <lib/http/client/client.h>
#include <lib/utils/pretty.h>

#include <iostream>

int main() {
    std::unique_ptr<IClient> client(new THttpClient("https://api.vk.com"));
    IConnectorPtr conn(new TVKConnector(*client, LoadSettings("conf/auth-settings")));

    try {
        auto result = conn->GetHist("кот", 3);
        std::cout
            << PrettyPrint(result)
            << std::endl;
    } catch (const std::runtime_error& error) {
        std::cout
            << "Error: "
            << error.what()
            << std::endl;
    }
}