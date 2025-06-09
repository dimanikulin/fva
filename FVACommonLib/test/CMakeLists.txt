# tests source files here
add_executable(fvalibtests
  test_fvacommonlib.cpp
  test_fvaconfiguration.cpp
  test_fvacsvfile.cpp
  test_FVADescriptionFile.cpp
  test_fvariffparser.cpp
)

# Link GoogleTest to tests
target_link_libraries(fvalibtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib)

# Add the tests to CTest's test runner
add_test(NAME fvalibtests COMMAND fvalibtests)

