set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES ../FVACommonLib/CmdLineBaseTask.h 
   ../FVACommonLib/fvafmtcontext.h 
   ../FVACommonLib/fvacommonexif.h
   ../FVACommonLib/fvacommoncsv.h
   ../FVACommonLib/CmdLineBaseExecutor.h
)
# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_library( fvadataprocessorlib
   CLTAutoChecks1.cpp
   CLTAutoChecks1.h
   CLTAutoChecks2.cpp
   CLTAutoChecks2.h
   CLTAutoChecks3.cpp
   CLTAutoChecks3.h
   CLTCheckDateTime.cpp
   CLTCheckDateTime.h
   CLTCheckDeviceName.cpp
   CLTCheckDeviceName.h
   CLTCheckFileFormat.cpp
   CLTCheckFileFormat.h
   CLTCheckLocation.cpp
   CLTCheckLocation.h
   CLTCreateDirStructByDevNames.cpp
   CLTCreateDirStructByDevNames.h
   CLTCreateDirStructByFileNames.cpp
   CLTCreateDirStructByFileNames.h
   CLTCSVFvaFiles.cpp
   CLTCSVFvaFiles.h
   CLTCSVGetTagsForFvaFiles.h
   CLTCSVGetTagsForFvaFiles.cpp
   CLTFixDuplicatedFileNames.cpp
   CLTFixDuplicatedFileNames.h
   CLTMoveInputDir2Output.cpp
   CLTMoveInputDir2Output.h
   CLTMoveAloneFiles.cpp
   CLTMoveAloneFiles.h
   CLTPrintFSStructure.cpp
   CLTPrintFSStructure.h
   CLTRenameFiles.cpp
   CLTRenameFiles.h
   CLTRenameVideoBySequence.cpp
   CLTRenameVideoBySequence.h
   CLTSetFileAtts.cpp
   CLTSetFileAtts.h
   FVADataProcessor.cpp
   FVADataProcessor.h

   ${HEADER_FILES}
)

target_link_libraries(fvadataprocessorlib Qt5::Widgets Qt5::Core Qt5::Gui fvalib)

target_include_directories(fvadataprocessorlib PRIVATE ../FVACommonLib)

install(TARGETS fvadataprocessorlib)
