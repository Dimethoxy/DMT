#==============================================================================
# Neutrino Test Folder CMakeLists.txt file
#==============================================================================

# Set the minimum version of CMake required to build the project
cmake_minimum_required(VERSION 3.22)

# Set the project name and version
project(Neutrino VERSION ${PROJECT_VERSION})

enable_testing()

add_executable(${PROJECT_NAME}_test
    AudioProcessorTest.cpp
)

target_include_directories(${PROJECT_NAME}_test
    PRIVATE
        ${GOOGLETEST_SOURCE_DIR}/googletest/include
        ${CMAKE_CURRENT_FOLDER}/../Source
        ${JUCE_SOURCE_DIR}/modules
)

target_link_libraries(${PROJECT_NAME}_test
    PRIVATE
        ${PROJECT_NAME}
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)