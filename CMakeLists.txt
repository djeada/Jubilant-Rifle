cmake_minimum_required(VERSION 3.7)
project(Jubilant-Riffle)

# Extend module path so it picks up our custom FindSDL2.cmake, etc.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/scripts")

# Locate SDL2 + image/ttf/mixer:
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Include the SDL2 headers and your project's include directory.
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    include
)

# Gather source and header files.
file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

# Create a static library and an executable.
add_library(main_lib STATIC ${SOURCES} ${HEADERS})
add_executable(main ${SOURCES})

# Set the C standard and compile/link options.
set_target_properties(main PROPERTIES C_STANDARD 99)
target_compile_options(main PRIVATE -Wall -Wextra -Werror -Wpedantic -fsanitize=address)
target_link_options(main PRIVATE -fsanitize=address)

# Link against main_lib, SDL2 (and related libraries), and the math library.
target_link_libraries(
    main
    main_lib
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    m
)

# Setup GoogleTest via FetchContent.
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.10.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_compile_definitions(CMAKE_POLICY_DEFAULT_CMP0048=NEW)
FetchContent_MakeAvailable(googletest)

set_target_properties(gtest PROPERTIES COMPILE_FLAGS "-Wno-error=maybe-uninitialized")
set_target_properties(gtest_main PROPERTIES COMPILE_FLAGS "-Wno-error=maybe-uninitialized")

# Function to copy resources from source to target directories.
function(move_resources source_dir target_dir)
    foreach(path ${source_dir})
        file(GLOB_RECURSE files ${path}/*)
        foreach(file ${files})
            file(COPY ${file} DESTINATION ${target_dir})
        endforeach()
    endforeach()
endfunction()

# Copy various resources to the binary directory.
move_resources(${CMAKE_CURRENT_SOURCE_DIR}/resources/textures ${CMAKE_BINARY_DIR}/resources/textures)
move_resources(${CMAKE_CURRENT_SOURCE_DIR}/resources/maps     ${CMAKE_BINARY_DIR}/resources/maps)
move_resources(${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts    ${CMAKE_BINARY_DIR}/resources/fonts)
move_resources(${CMAKE_CURRENT_SOURCE_DIR}/resources/audio    ${CMAKE_BINARY_DIR}/resources/audio)

# Enable testing if a C++ compiler is available.
include(CheckLanguage)
check_language(CXX)
if(CMAKE_CXX_COMPILER)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()
