set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# tests source files here
add_executable(uitests
  test_fvaconfigurator.cpp
  test_fvadescriptioneditor.cpp
  test_fvapeoplelistdlg.cpp
)

# Link GoogleTest and QT to tests
target_link_libraries(uitests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib fvaconfigurator)

# Add the tests to CTest's test runner
add_test(NAME uitests COMMAND uitests)
