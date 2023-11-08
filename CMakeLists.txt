cmake_minimum_required(VERSION 3.7)
project(Jubilant-Riffle)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

add_library(main_lib STATIC ${SOURCES} ${HEADERS})
add_executable(main ${SOURCES})

set_target_properties(main PROPERTIES C_STANDARD 99)
target_compile_options(main PRIVATE -Wall -Wextra -Werror -Wpedantic -fsanitize=address)
target_link_options(main PRIVATE -fsanitize=address)

# Link against the math library along with the SDL libraries for main_lib
target_link_libraries(main main_lib ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} m)

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

function(move_resources source_dir target_dir)
    foreach(path ${source_dir})
        file(GLOB_RECURSE files ${path}/*)
        foreach(file ${files})
            file(COPY ${file} DESTINATION ${target_dir})
        endforeach()
    endforeach()
endfunction()

move_resources(${CMAKE_CURRENT_SOURCE_DIR}/resources/textures ${CMAKE_BINARY_DIR}/resources/textures)
move_resources(${CMAKE_CURRENT_SOURCE_DIR}/resources/maps ${CMAKE_BINARY_DIR}/resources/maps)
move_resources(${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts ${CMAKE_BINARY_DIR}/resources/fonts)

include(CheckLanguage)
check_language(CXX)
if(CMAKE_CXX_COMPILER)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()
