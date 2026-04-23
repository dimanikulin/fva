set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(
  Qt5
  COMPONENTS Core Gui Widgets
  REQUIRED)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well
# in the current folder
add_library(
  fvacommonuilib
  fvafilter.h
  fvacommonui.cpp
  fvacommonui.h
  )

target_include_directories(fvacommonuilib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(fvacommonuilib Qt5::Widgets Qt5::Core Qt5::Gui)

install(TARGETS fvacommonuilib)
