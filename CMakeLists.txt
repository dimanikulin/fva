cmake_minimum_required(VERSION 3.0)

option(BUILD_TESTING "Build tests" ON)

if (WIN32)
  set(CMAKE_CXX_STANDARD_LIBRARIES
      "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
  set(CMAKE_EXE_LINKER_FLAGS
      "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")
endif (WIN32)

project(fva)

add_subdirectory(src)

# Enable testing for the project
if (BUILD_TESTING)
  # Add GoogleTest as a subdirectory.
  add_subdirectory(googletest)

  enable_testing()
  add_subdirectory(tests)
endif ()

# Code coverage
if (ENABLE_COVERAGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage")
  message("Code coverage is enabled and provided with GCC.")
endif ()
