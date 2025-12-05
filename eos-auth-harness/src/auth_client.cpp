#include "auth_client.h"
#include <iostream>
#include <ctime>

namespace eos_test
{

    // PIMPL implementation
    class AuthClient::Impl
    {
    public:
        EOS_HPlatform platform_handle = nullptr;
        EOS_HAuth auth_handle = nullptr;
        EOS_EpicAccountId account_id = nullptr;
        TokenInfo current_token;
        bool initialized = false;

        // Static callback for EOS login
        static void EOS_CALL OnLoginComplete(const EOS_Auth_LoginCallbackInfo *data)
        {
            auto *callback = static_cast<AuthCallback *>(data->ClientData);

            if (data->ResultCode == EOS_EResult::EOS_Success)
            {
                std::cout << "Login successful!" << std::endl;
                if (callback)
                {
                    (*callback)(true, "");
                }
            }
            else
            {
                std::string error = "Login failed: " + std::to_string(static_cast<int>(data->ResultCode));
                std::cerr << error << std::endl;
                if (callback)
                {
                    (*callback)(false, error);
                }
            }
        }

        static void EOS_CALL OnLogoutComplete(const EOS_Auth_LogoutCallbackInfo *data)
        {
            auto *callback = static_cast<AuthCallback *>(data->ClientData);

            if (data->ResultCode == EOS_EResult::EOS_Success)
            {
                std::cout << "Logout successful!" << std::endl;
                if (callback)
                {
                    (*callback)(true, "");
                }
            }
            else
            {
                if (callback)
                {
                    (*callback)(false, "Logout failed");
                }
            }
        }
    };

    AuthClient::AuthClient() : pimpl_(std::make_unique<Impl>()) {}

    AuthClient::~AuthClient()
    {
        Shutdown();
    }

    bool AuthClient::Initialize(const std::string &product_id,
                                const std::string &sandbox_id,
                                const std::string &deployment_id)
    {

        // Initialize EOS Platform
        EOS_InitializeOptions init_options = {};
        init_options.ApiVersion = EOS_INITIALIZE_API_LATEST;
        init_options.ProductName = "EOS Auth Harness";
        init_options.ProductVersion = "1.0";

        EOS_EResult result = EOS_Initialize(&init_options);
        if (result != EOS_EResult::EOS_Success)
        {
            std::cerr << "EOS_Initialize failed: " << static_cast<int>(result) << std::endl;
            return false;
        }

        // Create platform
        EOS_Platform_Options platform_options = {};
        platform_options.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
        platform_options.ProductId = product_id.c_str();
        platform_options.SandboxId = sandbox_id.c_str();
        platform_options.DeploymentId = deployment_id.c_str();
        platform_options.ClientCredentials.ClientId = "your_client_id"; // Get from Epic Dev Portal
        platform_options.ClientCredentials.ClientSecret = "your_client_secret";

        pimpl_->platform_handle = EOS_Platform_Create(&platform_options);
        if (!pimpl_->platform_handle)
        {
            std::cerr << "EOS_Platform_Create failed" << std::endl;
            EOS_Shutdown();
            return false;
        }

        pimpl_->auth_handle = EOS_Platform_GetAuthInterface(pimpl_->platform_handle);
        pimpl_->initialized = true;

        std::cout << "EOS Platform initialized successfully" << std::endl;
        return true;
    }

    void AuthClient::Shutdown()
    {
        if (pimpl_->platform_handle)
        {
            EOS_Platform_Release(pimpl_->platform_handle);
            pimpl_->platform_handle = nullptr;
        }

        if (pimpl_->initialized)
        {
            EOS_Shutdown();
            pimpl_->initialized = false;
        }
    }

    void AuthClient::LoginDevAuth(const std::string &host,
                                  const std::string &credential_name,
                                  AuthCallback callback)
    {

        if (!pimpl_->auth_handle)
        {
            callback(false, "Auth handle not initialized");
            return;
        }

        EOS_Auth_Credentials credentials = {};
        credentials.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
        credentials.Type = EOS_ELoginCredentialType::EOS_LCT_Developer;
        credentials.Id = host.c_str();
        credentials.Token = credential_name.c_str();

        EOS_Auth_LoginOptions login_options = {};
        login_options.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
        login_options.Credentials = &credentials;

        // Store callback in heap for async operation
        auto *callback_ptr = new AuthCallback(callback);

        EOS_Auth_Login(pimpl_->auth_handle, &login_options,
                       callback_ptr, Impl::OnLoginComplete);
    }

    void AuthClient::Logout(AuthCallback callback)
    {
        if (!pimpl_->auth_handle || !pimpl_->account_id)
        {
            callback(false, "Not logged in");
            return;
        }

        EOS_Auth_LogoutOptions logout_options = {};
        logout_options.ApiVersion = EOS_AUTH_LOGOUT_API_LATEST;
        logout_options.LocalUserId = pimpl_->account_id;

        auto *callback_ptr = new AuthCallback(callback);

        EOS_Auth_Logout(pimpl_->auth_handle, &logout_options,
                        callback_ptr, Impl::OnLogoutComplete);
    }

    bool AuthClient::IsLoggedIn() const
    {
        return pimpl_->account_id != nullptr;
    }

    TokenInfo AuthClient::GetTokenInfo() const
    {
        return pimpl_->current_token;
    }

    void AuthClient::Tick()
    {
        if (pimpl_->platform_handle)
        {
            EOS_Platform_Tick(pimpl_->platform_handle);
        }
    }

    bool AuthClient::RefreshToken()
    {
        // Implementation for token refresh
        // This would use EOS_Auth_CopyUserAuthToken and related APIs
        return true;
    }

} // namespace eos_test