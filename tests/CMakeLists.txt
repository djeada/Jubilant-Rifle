cmake_minimum_required(VERSION 3.7)

# Find the SDL2 and SDL2_image packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

# Ensure the test sources are set to .cpp for C++ compilation
set(TEST_SOURCES
  test_map.cpp  # Note the change to .cpp
  # Add more test source files here
)

# Create test executable
add_executable(runUnitTests ${TEST_SOURCES})

# Set properties for C++ standard, if your tests require a specific standard
set_target_properties(runUnitTests PROPERTIES CXX_STANDARD 11)

# Link Google Test and the SDL libraries to the test executable
target_link_libraries(runUnitTests main_lib ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} gtest gtest_main)

# Include the test subdirectory to your include directories
# Also include the SDL2 and SDL2_image include directories
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR}
                    ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

# Add the tests to be run using gtest_discover_tests which automatically discovers tests
gtest_discover_tests(runUnitTests)
