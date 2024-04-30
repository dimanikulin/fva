set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(test)

set(HEADER_FILES 
   ../FVACommonLib/fvaconstants.h 
   ../FVACommonLib/fvaconfiguration.h 
   ../FVADataProcessor/FVADataProcessor.h 
   ../FVACommonLib/fvalogger.inl)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_library(fvaflowcontrollerlib fvadevicecontext.h FVAFlowController.cpp FVAFlowController.h ${HEADER_FILES})

target_link_libraries(fvaflowcontrollerlib Qt5::Widgets Qt5::Core Qt5::Gui fvalib fvadataprocessorlib)

target_include_directories(fvaflowcontrollerlib PRIVATE ../FVACommonLib ../FVADataProcessor/)

install(TARGETS fvaflowcontrollerlib)
