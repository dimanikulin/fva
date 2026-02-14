set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_executable(fvaorganizerwizard 

    FVAOrganizerDevicePage.cpp
    FVAOrganizerDevicePage.h
    FVAOrganizerDonePage.cpp
    FVAOrganizerDonePage.h
    FVAOrganizerEventCfgPage.cpp
    FVAOrganizerEventCfgPage.h
    FVAOrganizerEventInfoPage.h
    FVAOrganizerEventInfoPage.cpp
    FVAOrganizerInputDirPage.cpp
    FVAOrganizerInputDirPage.h
    FVAOrganizerOrientPage.cpp
    FVAOrganizerOrientPage.h
    FVAOrganizerOutputDirPage.cpp
    FVAOrganizerOutputDirPage.h
    FVAOrganizerPlacePage.h
    FVAOrganizerPlacePage.cpp
    FVAOrganizerStartPage.cpp
    FVAOrganizerStartPage.h
    fvaorganizerwizard.cpp
    fvaorganizerwizard.h
    fvaorganizerwizard.qrc
    fvaorganizerwizard.ui
    main.cpp
    
    ${HEADER_FILES}
)

target_link_libraries(fvaorganizerwizard Qt5::Widgets Qt5::Core Qt5::Gui fvalib fvadataprocessorlib fvaflowcontrollerlib)

install(TARGETS fvaorganizerwizard)
