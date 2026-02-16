set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_executable(fvadictionaryeditor 

    fvadictionaryeditor.cpp
    fvadictionaryeditor.h
    fvadictionaryeditor.qrc
    fvadictionaryeditor.ui
    main.cpp

    ${HEADER_FILES}
)
target_link_libraries(fvadictionaryeditor Qt5::Widgets Qt5::Core Qt5::Gui fvalib)

install(TARGETS fvadictionaryeditor)


