set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES 
    ../FVACommonLib/fvacommonlib.h 
    ../FVACommonLib/fvacommoncsv.h
    ../FVACommonLib/fvaconfiguration.h
    ../FVACommonLib/fvacommonlib.h)

add_executable(fvadictionaryeditor 

    fvadictionaryeditor.cpp
    fvadictionaryeditor.h
    fvadictionaryeditor.qrc
    fvadictionaryeditor.ui
    main.cpp

    ${HEADER_FILES}
)
target_link_libraries(fvadictionaryeditor Qt5::Widgets Qt5::Core Qt5::Gui fvalib)

target_include_directories(fvadictionaryeditor PRIVATE ../FVACommonLib)

install(TARGETS fvadictionaryeditor)


