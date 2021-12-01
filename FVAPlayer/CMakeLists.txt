#add_executable(fvaplayer fvaplayer.cpp fvaplayer.h main.cpp playercontrols.cpp playercontrols.h videowidget.h)
add_executable(fvaplayer main.cpp)

find_package(
  QT NAMES Qt5
  COMPONENTS Core Gui Widgets Multimedia main MultimediaWidgets
  REQUIRED) 

install(TARGETS fvaplayer)
