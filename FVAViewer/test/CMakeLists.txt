set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# tests source files here
add_executable(fvaviewertests
  test_fvaviewer.cpp
)

# add includes
target_include_directories(fvaviewertests PRIVATE 
../../FVACommonLib
../../FVAFlowController)

# Link GoogleTest tests and other libs
target_link_libraries(fvaviewertests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib)

# Add the tests to CTest's test runner
add_test(NAME fvaviewertests COMMAND fvaviewertests)

