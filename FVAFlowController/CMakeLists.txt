set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES ../FVACommonLib/fvaconstants.h ../FVACommonLib/FVAConfiguration.h)

add_library(fvaflowcontroller fvadevicecontext.h FVAFlowController.cpp FVAFlowController.h ${HEADER_FILES})

target_link_libraries(fvaflowcontroller Qt5::Widgets Qt5::Core Qt5::Gui fvalib)

target_include_directories(fvaflowcontroller PRIVATE ../FVACommonLib)

install(TARGETS fvaflowcontroller)
