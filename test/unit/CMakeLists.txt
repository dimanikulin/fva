# tests source files here
add_executable(libtests
  test_fvacltcontext.cpp
  test_fvadevice.cpp
  test_fvaconstants.cpp
  test_fvafile.cpp
  test_fvafolder.cpp
  test_fvaevent.cpp
  test_fvaItem.cpp
  test_fvaperson.cpp
  test_cmdlinebaseexecutor.cpp
  test_cmdlinebasetask.cpp
  test_fvabasedictionaryitem.cpp
  test_fvafmtcontext.cpp
  test_fvacommoncsv.cpp
  test_fvacommonexif.cpp
  test_fvacommonlib.cpp
  test_fvaconfiguration.cpp
  test_fvacsvfile.cpp
  test_fvadescriptionfile.cpp
  test_fvariffparser.cpp
  test_CLTAutoChecks1.cpp
  test_CLTAutoChecks2.cpp
  test_CLTAutoChecks3.cpp
  test_CLTCheckDateTime.cpp
  test_CLTCheckDeviceName.cpp
  test_CLTCheckFileFormat.cpp
  test_CLTCheckLocation.cpp
  test_CLTCreateDirStructByDevNames.cpp
  test_CLTCreateDirStructByFileNames.cpp
  test_CLTCSVFvaFiles.cpp
  test_CLTCSVGetTagsForFvaFiles.cpp
  test_CLTFixDuplicatedFileNames.cpp
  test_CLTMoveAloneFiles.cpp
)

# Link GoogleTest to tests
target_link_libraries(libtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main gmock_main fvalib fvadataprocessorlib)

# Add the tests to CTest's test runner
add_test(NAME libtests COMMAND libtests)

install(TARGETS libtests DESTINATION bin)
