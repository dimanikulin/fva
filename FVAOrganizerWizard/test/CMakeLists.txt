set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# tests source files here
add_executable(fvaorganizerwizardtests
  test_FVAOrganizerDevicePage.cpp
  test_FVAOrganizerDonePage.cpp
  test_FVAOrganizerEventCfgPage.cpp
  test_FVAOrganizerEventInfoPage.cpp
  test_FVAOrganizerInputDirPage.cpp
  test_FVAOrganizerOrientPage.cpp
  test_FVAOrganizerOutputDirPage.cpp
  test_FVAOrganizerPlacePage.cpp
)

# add includes
target_include_directories( fvaorganizerwizardtests PRIVATE ../../FVACommonLib )

# Link GoogleTest tests and other libs
target_link_libraries(fvaorganizerwizardtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib)

# Add the tests to CTest's test runner
add_test(NAME fvaorganizerwizardtests COMMAND fvaorganizerwizardtests)

