set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# tests source files here
add_executable(fvadataprocessorlibtests
  test_CLTAutoChecks1.cpp
  test_CLTAutoChecks2.cpp
)

# Link GoogleTest tests and other libs
target_link_libraries(fvadataprocessorlibtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib)

# add includes
target_include_directories(fvadataprocessorlibtests PRIVATE ../../FVACommonLib)

# Add the tests to CTest's test runner
add_test(NAME fvadataprocessorlibtests COMMAND fvadataprocessorlibtests)

