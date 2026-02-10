set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well in the current folder 
add_executable(fvaorganizer main.cpp ${HEADER_FILES})

target_link_libraries(fvaorganizer Qt5::Widgets Qt5::Core Qt5::Gui fvalib fvadataprocessorlib)

install(TARGETS fvaorganizer)
                                                                                                                    	