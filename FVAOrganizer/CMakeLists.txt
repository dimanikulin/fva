set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES ../FVACommonLib/fvacommonlib.h ../FVACommonLib/fvalogger.inl ../FVADataProcessor/FVADataProcessor.h)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_executable(fvaorganizer main.cpp ${HEADER_FILES})

target_link_libraries(fvaorganizer Qt5::Widgets Qt5::Core Qt5::Gui fvalib fvadataprocessorlib)

target_include_directories(fvaorganizer PRIVATE ../FVACommonLib ../FVADataProcessor)

install(TARGETS fvaorganizer)
                                                                                                                    	