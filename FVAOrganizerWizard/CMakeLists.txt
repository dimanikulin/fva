set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

set(HEADER_FILES 
    ../FVACommonLib/fvacommoncsv.h
    ../FVACommonLib/fvacommonui.h
    ../FVACommonLib/fvacommonlib.h
    ../FVACommonLib/FVAconstants.h
    ../FVACommonLib/fvaconfiguration.h
    ../FVACommonLib/fvalogger.inl
    ../FVADataProcessor/FVAFlowController.h
    ../FVAFlowController/FVAFlowController.h
    )

add_executable(fvaorganizerwizard 

    FVAOrganizerDevicePage.cpp
    FVAOrganizerDevicePage.h
    FVAOrganizerDonePage.cpp
    FVAOrganizerDonePage.h
    FVAOrganizerEventCfgPage.cpp
    FVAOrganizerEventCfgPage.h
    FVAOrganizerInputDirPage.cpp
    FVAOrganizerInputDirPage.h
    FVAOrganizerOrientPage.cpp
    FVAOrganizerOrientPage.h
    FVAOrganizerOutputDirPage.cpp
    FVAOrganizerOutputDirPage.h
    FVAOrganizerStartPage.cpp
    FVAOrganizerStartPage.h
    fvaorganizerwizard.cpp
    fvaorganizerwizard.h
    fvaorganizerwizard.qrc
    fvaorganizerwizard.ui
    main.cpp
    
    ${HEADER_FILES}
)

