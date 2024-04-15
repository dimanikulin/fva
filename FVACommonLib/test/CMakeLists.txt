set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Add your tests source files here
add_executable(fvalibtests
  test_FVABaseDictionaryItem.cpp
  test_fvacltcontext.cpp
  test_fvacommoncsv.cpp
  test_CmdLineBaseTask.cpp
)

# Link GoogleTest to your tests
target_link_libraries(fvalibtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib)

# Add the tests to CTest's test runner
add_test(NAME fvalibtests COMMAND fvalibtests)

