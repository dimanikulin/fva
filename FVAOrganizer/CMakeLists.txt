set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES ../FVADataProcessor/FVADataProcessor.h ../FVACommonLib/fvacommonlib.h ../FVACommonLib/fvalogger.inl)

add_executable(fvaorganizer main.cpp ${HEADER_FILES})

target_link_libraries(fvaorganizer Qt5::Widgets Qt5::Core Qt5::Gui fvalib fvadataprocessorlib)

target_include_directories(fvaorganizer PRIVATE ../FVACommonLib ../FVADataProcessor)

install(TARGETS fvaorganizer)
