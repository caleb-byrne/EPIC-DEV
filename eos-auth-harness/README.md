# EOS Auth Harness

A C++ testing library and CLI tool for Epic Online Services authentication workflows. Built for SDET portfolio demonstration.

## Features

- ✅ EOS Auth SDK wrapper with modern C++ (C++17)
- ✅ Dev Auth Tool integration
- ✅ Token lifecycle management (login, refresh, revoke)
- ✅ Comprehensive unit tests (Google Test)
- ✅ Integration tests with real EOS backend
- ✅ CLI tool for manual testing
- ✅ CMake build system

## Prerequisites

- **CMake** 3.15 or higher
- **C++17** compatible compiler (clang, gcc, MSVC)
- **EOS SDK** (download from Epic Developer Portal)
- **Dev Auth Tool** (for integration testing)

## Quick Start

### 1. Setup Environment

# Clone repository
git clone https://github.com/yourusername/eos-auth-harness.git
cd eos-auth-harness

# Set EOS SDK path
export EOS_SDK_PATH=/path/to/EOS_SDK

# Download and run Dev Auth Tool
./EOS_DevAuthTool

# Add test credential: testuser1 / testpass1

### 2. Build

mkdir build && cd build
cmake ..
make -j4

### 3. Run Tests

# Unit tests
./bin/auth_client_tests

# Integration tests (requires Dev Auth Tool running)
INTEGRATION_TEST=1 ./bin/integration_tests

### 4. Use CLI Tool

# Login
./bin/eos_auth_tool login localhost:6547 testuser1

# Check status
./bin/eos_auth_tool status

# Logout
./bin/eos_auth_tool logout
```

## Architecture
```
┌─────────────────┐
│   CLI Tool      │
│  (cli_main.cpp) │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  AuthClient     │  ← Core library
│  (auth_client)  │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│   EOS SDK       │
│  (Platform/Auth)│
└─────────────────┘

## Testing Strategy

### Unit Tests
- Mock EOS SDK interfaces
- Test token lifecycle logic
- Validate error handling
- Coverage target: 80%+

### Integration Tests
- Real Dev Auth Tool interaction
- End-to-end auth flows
- Token expiry scenarios
- Network failure handling

## Project Structure
eos-auth-harness/
├── include/
│   └── auth_client.h       # Public API
├── src/
│   ├── auth_client.cpp     # Implementation
│   └── cli_main.cpp        # CLI tool
├── tests/
│   ├── auth_client_tests.cpp
│   └── integration_tests.cpp
├── CMakeLists.txt
└── README.md


## Key Learnings

This project demonstrates:

1. **EOS SDK Integration**: Practical experience with Epic's online services
2. **Modern C++ Patterns**: RAII, smart pointers, PIMPL idiom
3. **Async Programming**: Callbacks, event loops (EOS_Platform_Tick)
4. **Test Automation**: Unit + integration testing strategies
5. **Build Systems**: CMake for cross-platform C++ projects
