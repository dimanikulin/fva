set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# tests source files here
add_executable(fvadataprocessorlibtests
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
  test_CLTMoveInputDir2Output.cpp
  test_CLTPrintFSStructure.cpp
  test_CLTRenameFiles.cpp
  test_CLTRenameVideoBySequence.cpp
  test_CLTSetFileAtts.cpp
  test_FVADataProcessor.cpp
)

# Link GoogleTest tests and other libs
target_link_libraries(fvadataprocessorlibtests Qt5::Widgets Qt5::Core Qt5::Gui gtest gtest_main fvalib fvadataprocessorlib)

# add includes
target_include_directories(fvadataprocessorlibtests PRIVATE ../../FVACommonLib)

# Add the tests to CTest's test runner
add_test(NAME fvadataprocessorlibtests COMMAND fvadataprocessorlibtests)
