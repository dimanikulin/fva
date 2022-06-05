set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_library(fvalib  
  fvadevice.h
  fvafile.h
  fvafolder.h
  fvaItem.h
  fvaperson.h
  fvaItem.cpp
  FVABaseDictionaryItem.h
  FVAEvent.h

  CmdLineBaseExecutor.h
  CmdLineBaseTask.h
  fvacltcontext.h
  fvacommoncsv.h
  fvacommonexif.h
  fvaconfiguration.h
  fvacsvfile.h
  fvacommonlib.h
  fvacommonui.h
  fvaconstants.h
  FVADescriptionFile.h
  fvaexitcodes.h
  fvafmtcontext.h
  fvariffparser.h
  fvalogger.inl

  CmdLineBaseExecutor.cpp
  CmdLineBaseTask.cpp
  fvacommoncsv.cpp
  fvacommonexif.cpp
  fvaconfiguration.cpp
  fvacsvfile.cpp
  fvacommonlib.cpp
  fvacommonui.cpp
  FVADescriptionFile.cpp
  fvafmtcontext.cpp
  fvariffparser.cpp

  exif.cpp
)

target_link_libraries(fvalib Qt5::Widgets Qt5::Core Qt5::Gui)

install(TARGETS fvalib)
