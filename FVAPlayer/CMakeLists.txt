#add_executable(fvaplayer fvaplayer.cpp fvaplayer.h main.cpp playercontrols.cpp playercontrols.h videowidget.h)

#find_package(
#  QT NAMES Qt5
#  COMPONENTS Core Gui Widgets Multimedia main MultimediaWidgets
#  REQUIRED) 
  
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(fvaplayer main.cpp)

install(TARGETS fvaplayer)
