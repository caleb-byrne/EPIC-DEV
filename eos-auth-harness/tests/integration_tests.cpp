#include <gtest/gtest.h>
#include "auth_client.h"
#include <thread>
#include <chrono>

class IntegrationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Check if Dev Auth Tool is running
        // Set environment flag: INTEGRATION_TEST=1
        if (std::getenv("INTEGRATION_TEST") == nullptr)
        {
            GTEST_SKIP() << "Skipping integration test. Set INTEGRATION_TEST=1 to run.";
        }
    }
};

TEST_F(IntegrationTest, FullAuthFlow)
{
    AuthClient client;

    ASSERT_TRUE(client.Initialize("product_id", "sandbox_id", "deployment_id"));

    bool login_success = false;
    bool callback_called = false;

    client.LoginDevAuth("localhost:6547", "testuser1",
                        [&](bool success, const std::string &error)
                        {
                            callback_called = true;
                            login_success = success;
                        });

    // Wait for callback
    int attempts = 0;
    while (!callback_called && attempts < 50)
    {
        client.Tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        attempts++;
    }

    ASSERT_TRUE(callback_called);
    ASSERT_TRUE(login_success);
    EXPECT_TRUE(client.IsLoggedIn());

    // Test token retrieval
    TokenInfo token = client.GetTokenInfo();
    EXPECT_TRUE(token.is_valid);
    EXPECT_FALSE(token.access_token.empty());
}