set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Add your tests source files here
add_executable(fvalibtests
  test_CmdLineBaseExecutor.cpp
  test_CmdLineBaseTask.cpp
  test_FVABaseDictionaryItem.cpp
  test_fvacltcontext.cpp
  test_fvacommoncsv.cpp
  test_fvacommonexif.cpp
  test_fvacommonlib.cpp
  test_fvacommonui.cpp
  test_fvaconfiguration.cpp
  test_fvaconstants.cpp
  test_fvacsvfile.cpp
  test_FVADescriptionFile.cpp
  test_fvadevice.cpp
  test_FVAEvent.cpp
  test_fvafile.cpp
  test_fvafmtcontext.cpp
  test_fvafolder.cpp
  test_fvaItem.cpp
  test_fvaperson.cpp
)

# Link GoogleTest to your tests
target_link_libraries(fvalibtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib)

# Add the tests to CTest's test runner
add_test(NAME fvalibtests COMMAND fvalibtests)

