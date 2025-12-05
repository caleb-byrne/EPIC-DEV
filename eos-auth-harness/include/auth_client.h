#ifndef EOS_AUTH_CLIENT_H
#define EOS_AUTH_CLIENT_H

#include <string>
#include <memory>
#include <functional>
#include "eos_sdk.h"
#include "eos_auth.h"
#include "eos_platform.h"

namespace eos_test {

// Token information structure
struct TokenInfo {
    std::string access_token;
    std::string refresh_token;
    int64_t expires_at;  // Unix timestamp
    bool is_valid;
};

// Auth result callback
using AuthCallback = std::function<void(bool success, const std::string& error)>;

class AuthClient {
public:
    AuthClient();
    ~AuthClient();

    // Initialize EOS Platform
    bool Initialize(const std::string& product_id, 
                   const std::string& sandbox_id,
                   const std::string& deployment_id);
    
    // Shutdown EOS Platform
    void Shutdown();

    // Login using Dev Auth Tool
    void LoginDevAuth(const std::string& host,
                     const std::string& credential_name,
                     AuthCallback callback);

    // Refresh access token
    bool RefreshToken();

    // Revoke/logout
    void Logout(AuthCallback callback);

    // Get current token info
    TokenInfo GetTokenInfo() const;

    // Check if currently logged in
    bool IsLoggedIn() const;

    // Tick the platform (call regularly in main loop)
    void Tick();

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};

} // namespace eos_test

#endif