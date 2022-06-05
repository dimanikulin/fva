set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES 
    ../FVACommonLib/fvacommonui.h 
    ../FVACommonLib/fvacommonlib.h 
    ../FVACommonLib/fvaItem.h
    ../FVACommonLib/fvafolder.h
    ../FVACommonLib/fvafile.h
    ../FVACommonLib/FVADescriptionFile.h
)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_executable(fvaviewer 
    d-filter.ui
    e-filter.ui
    filters.ui
    fvaFilter.h
    fvaviewer.cpp
    fvaviewer.h
    fvaviewer.qrc
    fvaviewer.ui
    geo-filter.ui
    h-filter.ui
    main.cpp
    qpicturelabel.cpp
    qpicturelabel.h
    qpicturelabel.ui
    main.cpp 
   ${HEADER_FILES}
)

target_link_libraries(fvaviewer Qt5::Widgets Qt5::Core Qt5::Gui fvalib)

target_include_directories(fvaviewer PRIVATE ../FVACommonLib)

install(TARGETS fvaviewer)
                                                                                                                    	