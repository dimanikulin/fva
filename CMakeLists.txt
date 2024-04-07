cmake_minimum_required (VERSION 3.0)

if (WIN32)
    set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif (WIN32)

project(fva)

# Enable testing for the project
enable_testing()

# Add GoogleTest as a subdirectory. You need to have gtest in your project directory.
add_subdirectory(googletest)

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
	