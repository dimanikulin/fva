# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well
# in the current folder
add_library(fvaflowcontrollerlib fvadevicecontext.h FVAFlowController.cpp FVAFlowController.h
                                 ${HEADER_FILES})

target_link_libraries(fvaflowcontrollerlib fvalib fvadataprocessorlib)

target_include_directories(fvaflowcontrollerlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS fvaflowcontrollerlib)
