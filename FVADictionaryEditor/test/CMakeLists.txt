set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# tests source files here
add_executable(fvadictionaryeditortests
  test_fvadictionaryeditor.cpp
)

# Link GoogleTest to tests
target_link_libraries(fvadictionaryeditortests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib)

# Add the tests to CTest's test runner
add_test(NAME fvadictionaryeditortests COMMAND fvadictionaryeditortests)

