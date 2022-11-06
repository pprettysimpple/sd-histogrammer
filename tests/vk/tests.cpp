#include <conf/config.h>

#include <lib/connection/mock/connector.h>
#include <lib/connection/vk-api/connector.h>
#include <lib/http/client/client.h>
#include <lib/http/mock/client.h>

#include <tests/vk/fake-server/server.h>

#include <gtest/gtest.h>


namespace {
    using TResult = std::vector<uint64_t>;

    class TFakeServerTest : public testing::Test {
    protected:
        TFakeServerTest() {
            Server = std::make_shared<TFakeServer>(O_POLL | O_DETACH_LISTEN,
                                                   LoadData("tests/vk/data/fake_server_basic_items.json"));
        }

        virtual ~TFakeServerTest() override {}

        // Override this to define how to set up the environment.
        virtual void SetUp() override {
            Server->Listen();
        }

        // Override this to define how to tear down the environment.
        virtual void TearDown() override {
            Server->Stop();
        }

    private:
        std::shared_ptr<TFakeServer> Server;
    };
}

TEST_F(TFakeServerTest, SimpleQuery) {
    std::unique_ptr<IClient> client(new THttpClient(FAKE_SERVER_CONN_STRING));
    IConnectorPtr conn(new TVKConnector(*client, TVKAuthSettings{
        .ClientId = "<does-not-matter>",
        .PublicToken = FAKE_SERVER_ACCESS_TOKEN,
    }));

    {
        auto result = conn->GetHistImpl("cat", 1667667601, 1667673852 + 1);
        ASSERT_EQ(result, 3);
    }

    {
        auto result = conn->GetHistImpl("cat", 1667667601, 1667673852);
        ASSERT_EQ(result, 2);
    }

    {
        auto result = conn->GetHistImpl("cat", 10, 20);
        ASSERT_EQ(result, 0);
    }

    {
        auto result = conn->GetHistImpl("cats", 1667667601, 1667673852 + 1);
        ASSERT_EQ(result, 3);
    }

    {
        auto result = conn->GetHistImpl("cats", 1667667601, 1667673852);
        ASSERT_EQ(result, 2);
    }

    {
        auto result = conn->GetHistImpl("not-found", 1667667601, 1667673852 + 1);
        ASSERT_EQ(result, 0);
    }
}

TEST_F(TFakeServerTest, WrongToken) {
    std::unique_ptr<IClient> client(new THttpClient(FAKE_SERVER_CONN_STRING));
    IConnectorPtr conn(new TVKConnector(*client, TVKAuthSettings{
            .ClientId = "<does-not-matter>",
            .PublicToken = "WRONG_TOKEN",
    }));

    ASSERT_THROW(auto result = conn->GetHistImpl("cat", 1667667601, 1667673852 + 1), nlohmann::json::parse_error);
}

TEST(FakeClientTest, SimpleQuery) {
    std::unique_ptr<IClient> client(new TFakeClient());
    IConnectorPtr conn(new TVKConnector(*client, TVKAuthSettings{
            .ClientId = "<does-not-matter>",
            .PublicToken = "<token>",
    }));

    auto result = conn->GetHistImpl("cat", 24, 42);
    ASSERT_EQ(result, 3);
}

TEST(FakeConnectorTest, SimpleQuery) {
    IConnectorPtr conn(new TFakeConnector());

    auto result = conn->GetHist("cat", 4);
    ASSERT_EQ(result, TResult({
        3, 6, 9, 12
    }));
}
