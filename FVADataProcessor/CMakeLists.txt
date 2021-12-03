set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_library( fvadataprocessor
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
   CLTFixDuplicatedFileNames.cpp
   CLTFixDuplicatedFileNames.h
   CLTGetFvaDirType.cpp
   CLTGetFvaDirType.h
   CLTMerge1DayEventDir.cpp
   CLTMerge1DayEventDir.h
   CLTMerge1EventDir.cpp
   CLTMerge1EventDir.h
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
)

target_link_libraries(fvadataprocessor Qt5::Widgets Qt5::Core Qt5::Gui)

install(TARGETS fvadataprocessor)