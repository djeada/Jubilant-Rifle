cmake_minimum_required(VERSION 3.7)

# Find the SDL2, SDL2_image, and SDL2_ttf packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)  # This line should set SDL2_TTF_LIBRARIES and SDL2_TTF_INCLUDE_DIRS

# Recursively find all test_*.cpp files in the current source directory and subdirectories
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/test*.cpp")
list(APPEND TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
# Create test executable
add_executable(runUnitTests ${TEST_SOURCES})

# Set properties for C++ standard, if your tests require a specific standard
set_target_properties(runUnitTests PROPERTIES CXX_STANDARD 11)

# Link Google Test and the SDL libraries to the test executable
# Update these variables to the correct ones
target_link_libraries(runUnitTests main_lib ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} gtest gtest_main)

# Include the test subdirectory to your include directories
# Also include the SDL2, SDL2_image, and SDL2_ttf include directories
# Update these variables to the correct ones
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
                    ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Add the tests to be run using gtest_discover_tests which automatically discovers tests
gtest_discover_tests(runUnitTests)
