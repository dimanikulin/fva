set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(fvaconfigurator fvaconfigurator.qrc fvaconfigurator.ui fvaconfigurator.cpp fvaconfigurator.h main.cpp)

target_link_libraries(fvaconfigurator Qt5::Widgets Qt5::Core Qt5::Gui fvalib)

install(TARGETS fvaconfigurator)
