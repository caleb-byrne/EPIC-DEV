# Unit tests
add_executable(auth_client_tests
    auth_client_tests.cpp
)

target_link_libraries(auth_client_tests
    eos_auth_lib
    gtest_main
    gmock
)

include(GoogleTest)
gtest_discover_tests(auth_client_tests)