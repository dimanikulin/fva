# tests source files here
add_executable(libtests
  test_fvadevice.cpp
)

# Link GoogleTest to tests
target_link_libraries(libtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib)

# Add the tests to CTest's test runner
add_test(NAME libtests COMMAND libtests)

