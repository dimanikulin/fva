cmake_minimum_required (VERSION 3.0)

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif (WIN32)

if (UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -Wl,--no-whole-archive")
endif (UNIX)

project(fva)

add_subdirectory(FVACommonLib)
add_subdirectory(FVAConfigurator)
add_subdirectory(FVADataProcessor)
add_subdirectory(FVADescriptionEditor)
add_subdirectory(FVADictionaryEditor)
add_subdirectory(FVAFlowController)
add_subdirectory(FVAOrganizer)
add_subdirectory(FVAOrganizerWizard)
add_subdirectory(FVAPlayer)
add_subdirectory(FVAViewer)


