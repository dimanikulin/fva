set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets REQUIRED)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_executable(fvaplayer fvaplayer.qrc fvaplayer.ui fvaplayer.cpp fvaplayer.h main.cpp playercontrols.cpp playercontrols.h videowidget.h)

target_link_libraries(fvaplayer Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::MultimediaWidgets)

install(TARGETS fvaplayer)
