set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# tests source files here
add_executable(fvaflowcontrollerlibtests
  test_fvadevicecontext.cpp
  test_FVAFlowController.cpp
)

# add includes
target_include_directories(fvaflowcontrollerlibtests PRIVATE ../../FVACommonLib)

# Link GoogleTest tests and other libs
target_link_libraries(fvaflowcontrollerlibtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib fvaflowcontrollerlib)

# Add the tests to CTest's test runner
add_test(NAME fvaflowcontrollerlibtests COMMAND fvaflowcontrollerlibtests)

