# IMPORTANT: once you add new implementation file here please update IMPLEMENTATIONSTATUS.MD as well
# in the current folder
add_executable(fvaorganizer main.cpp ${HEADER_FILES})

target_link_libraries(fvaorganizer fvalib fvadataprocessorlib)

install(TARGETS fvaorganizer)
