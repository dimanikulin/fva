find_package(Qt5 COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets REQUIRED)

add_executable(fvaplayer fvaplayer.cpp fvaplayer.h main.cpp playercontrols.cpp playercontrols.h videowidget.h)

target_link_libraries(fvaplayer Qt5::Widgets Qt5::Core Qt5::Gui)

install(TARGETS fvaplayer)
