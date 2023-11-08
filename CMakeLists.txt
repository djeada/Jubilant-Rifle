cmake_minimum_required(VERSION 3.7)
project(Jubilant-Riffle)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/scripts")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR})
# Include the directory where the header files are
include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(main ${SOURCES} ${HEADERS})  

add_compile_options(-Wall -Wextra -Werror -Wpedantic -std=c99 -fsanitize=address)
add_link_options(-fsanitize=address)

# Link against the math library along with the SDL libraries
target_link_libraries(main ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES}
                      ${SDL2_TTF_LIBRARIES} m)

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
