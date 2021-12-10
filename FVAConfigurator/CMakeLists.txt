set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES 
   ../FVACommonLib/fvaconstants.h
   ../FVACommonLib/FVAConfiguration.h   
   ../FVACommonLib/fvacommonui.h
 )

add_executable(fvaconfigurator fvaconfigurator.qrc fvaconfigurator.ui fvaconfigurator.cpp fvaconfigurator.h main.cpp ${HEADER_FILES})

target_link_libraries(fvaconfigurator Qt5::Widgets Qt5::Core Qt5::Gui fvalib)

target_include_directories(fvaconfigurator PRIVATE ../FVACommonLib)

install(TARGETS fvaconfigurator)
