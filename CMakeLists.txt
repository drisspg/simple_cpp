
cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(simple_cpp LANGUAGES CXX)
# Set the C++ standard for all targets
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable better clangd support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#  simple_cpp source files
file(GLOB_RECURSE SIMPLE_CPP_CPP_SOURCES src/*.cpp)
MESSAGE(STATUS "SIMPLE_CPP_CPP_SOURCES: ${SIMPLE_CPP_CPP_SOURCES}")

add_library(SimpleCppLib STATIC
  ${SIMPLE_CPP_CPP_SOURCES}
)

# Add include directories to the library
target_include_directories(SimpleCppLib PUBLIC src/include)

# Add third_party stuff
# add_subdirectory(third_party/fmt)

# Set the output directory for the binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build the examples
file(GLOB_RECURSE EXAMPLE_SOURCES examples/*.cpp)
foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})

    # Include header utils
    # This is gross but makes clangd work, I am sure is brittle but w.e for now
    target_include_directories(${EXAMPLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    # Third party links
    # target_link_libraries(${EXAMPLE_NAME} PRIVATE fmt::fmt)

    # Link the library to the target
    target_link_libraries(${EXAMPLE_NAME} PRIVATE SimpleCppLib)
endforeach()
