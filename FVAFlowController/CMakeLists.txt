set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES ../FVACommonLib/fvaconstants.h ../FVACommonLib/FVAConfiguration.h ../FVADataProcessor/FVADataProcessor.h)

add_library(fvaflowcontrollerlib fvadevicecontext.h FVAFlowController.cpp FVAFlowController.h ${HEADER_FILES})

target_link_libraries(fvaflowcontrollerlib Qt5::Widgets Qt5::Core Qt5::Gui fvalib fvadataprocessorlib -static-libgcc -static-libstdc++)

target_include_directories(fvaflowcontrollerlib PRIVATE ../FVACommonLib ../FVADataProcessor/)

install(TARGETS fvaflowcontrollerlib)
