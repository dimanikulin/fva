set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES 
    ../FVACommonLib/FVADescriptionFile.h 
    ../FVACommonLib/fvaconfiguration.h 
    ../FVACommonLib/fvacommonui.h
    ../FVACommonLib/fvalogger.inl
    ../FVACommonLib/fvacommonlib.h)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_executable(fvadescriptioneditor 
    fvadescriptioneditor.cpp
    fvadescriptioneditor.h
    fvadescriptioneditor.qrc
    fvadescriptioneditor.ui
    fvapeoplelistdlg.cpp
    fvapeoplelistdlg.h
    fvapeoplelistdlg.ui
    main.cpp

    ${HEADER_FILES}
)
target_link_libraries(fvadescriptioneditor Qt5::Widgets Qt5::Core Qt5::Gui fvalib)

target_include_directories(fvadescriptioneditor PRIVATE ../FVACommonLib)

install(TARGETS fvadescriptioneditor)

