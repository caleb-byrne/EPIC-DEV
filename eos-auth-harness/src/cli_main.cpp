#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "auth_client.h"

using namespace eos_test;

void print_usage()
{
    std::cout << "EOS Auth Tool - CLI\n\n";
    std::cout << "Usage:\n";
    std::cout << "  eos_auth_tool login <host> <credential_name>\n";
    std::cout << "  eos_auth_tool logout\n";
    std::cout << "  eos_auth_tool status\n";
    std::cout << "  eos_auth_tool refresh\n";
    std::cout << "\nExample:\n";
    std::cout << "  eos_auth_tool login localhost:6547 testuser1\n";
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        print_usage();
        return 1;
    }

    std::string command = argv[1];

    AuthClient client;

    // Initialize with your Epic credentials
    if (!client.Initialize("your_product_id", "your_sandbox_id", "your_deployment_id"))
    {
        std::cerr << "Failed to initialize EOS client" << std::endl;
        return 1;
    }

    if (command == "login")
    {
        if (argc != 4)
        {
            std::cerr << "Usage: eos_auth_tool login <host> <credential_name>" << std::endl;
            return 1;
        }

        std::string host = argv[2];
        std::string credential = argv[3];

        std::cout << "Attempting login with Dev Auth..." << std::endl;

        bool login_complete = false;
        bool login_success = false;

        client.LoginDevAuth(host, credential, [&](bool success, const std::string &error)
                            {
            login_complete = true;
            login_success = success;
            
            if (success) {
                std::cout << "✓ Login successful!" << std::endl;
            } else {
                std::cerr << "✗ Login failed: " << error << std::endl;
            } });

        // Wait for callback (with timeout)
        int wait_count = 0;
        while (!login_complete && wait_count < 100)
        {
            client.Tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            wait_count++;
        }

        if (!login_complete)
        {
            std::cerr << "Login timeout" << std::endl;
            return 1;
        }

        return login_success ? 0 : 1;
    }
    else if (command == "status")
    {
        if (client.IsLoggedIn())
        {
            std::cout << "Status: Logged in" << std::endl;
            TokenInfo token = client.GetTokenInfo();
            std::cout << "Token valid: " << (token.is_valid ? "yes" : "no") << std::endl;
        }
        else
        {
            std::cout << "Status: Not logged in" << std::endl;
        }
    }
    else if (command == "logout")
    {
        bool logout_complete = false;

        client.Logout([&](bool success, const std::string &error)
                      {
            logout_complete = true;
            if (success) {
                std::cout << "✓ Logged out successfully" << std::endl;
            } else {
                std::cerr << "✗ Logout failed: " << error << std::endl;
            } });

        int wait_count = 0;
        while (!logout_complete && wait_count < 100)
        {
            client.Tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            wait_count++;
        }
    }
    else if (command == "refresh")
    {
        if (client.RefreshToken())
        {
            std::cout << "✓ Token refreshed" << std::endl;
        }
        else
        {
            std::cerr << "✗ Token refresh failed" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Unknown command: " << command << std::endl;
        print_usage();
        return 1;
    }

    client.Shutdown();
    return 0;
}