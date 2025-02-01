#
# FindSDL2.cmake
#
# A robust FindSDL2 script that:
# 1) Tries pkg-config
# 2) Falls back to manual searching in known paths
# 3) Allows user overrides
#
# Once found, this module sets:
#  - SDL2_FOUND        = TRUE if found, FALSE otherwise
#  - SDL2_INCLUDE_DIRS = include paths for SDL2
#  - SDL2_LIBRARIES    = libraries to link against
#
# Usage:
#   find_package(SDL2 REQUIRED)
#   include_directories(${SDL2_INCLUDE_DIRS})
#   target_link_libraries(yourTarget ${SDL2_LIBRARIES})
#

include(FindPackageHandleStandardArgs)
include(CMakeFindDependencyMacro)

# ----------------------------------------------------------------------------
# 1. Try pkg-config first
# ----------------------------------------------------------------------------
find_package(PkgConfig QUIET)

if(PkgConfig_FOUND)
    pkg_check_modules(PC_SDL2 QUIET sdl2)
endif()

# ----------------------------------------------------------------------------
# 2. Find the include directory (SDL2/SDL.h)
# ----------------------------------------------------------------------------
# If pkg-config worked, use that hint. If not, we’ll guess typical paths.

set(_SDL2_SEARCH_DIRS
    # If pkg-config found a prefix, use it first.
    ${PC_SDL2_INCLUDEDIR}
    ${PC_SDL2_INCLUDE_DIRS}

    # Typical fallback locations on Linux
    /usr/include
    /usr/include/SDL2
    /usr/include/x86_64-linux-gnu
    /usr/include/x86_64-linux-gnu/SDL2
    /usr/local/include
    /usr/local/include/SDL2

    # macOS typical
    /Library/Frameworks/SDL2.framework/Headers

    # Possibly more, as needed…
)

find_path(SDL2_INCLUDE_DIR
    NAMES SDL.h SDL2/SDL.h
    HINTS ${_SDL2_SEARCH_DIRS}
    PATH_SUFFIXES SDL2
)

# ----------------------------------------------------------------------------
# 3. Find the SDL2 library
# ----------------------------------------------------------------------------
# If pkg-config gave us something, we prefer that. Otherwise we try well-known paths.

set(_SDL2_LIB_SEARCH_DIRS
    # If pkg-config found a prefix, use it first.
    ${PC_SDL2_LIBDIR}
    ${PC_SDL2_LIBRARY_DIRS}

    # Typical fallback library paths on Linux
    /usr/lib
    /usr/lib64
    /usr/lib/x86_64-linux-gnu
    /usr/local/lib
    /usr/local/lib64

    # Possibly more, as needed…
)

# On macOS, SDL2 is often distributed as a .framework
# but let's just do standard library search first, then handle frameworks:
find_library(SDL2_LIBRARY
    NAMES SDL2 SDL2-2.0 SDL2main
    HINTS ${_SDL2_LIB_SEARCH_DIRS}
)

# Check for frameworks on macOS (optional; remove if not needed)
find_library(SDL2_FRAMEWORK
    NAMES SDL2
    PATHS /Library/Frameworks /System/Library/Frameworks
    NO_DEFAULT_PATH
)

# If we found a framework, combine it with the library variable (in case the user
# prefers frameworks). You can adjust this logic as you see fit.
if(SDL2_FRAMEWORK)
    set(SDL2_LIBRARY ${SDL2_FRAMEWORK})
endif()

# ----------------------------------------------------------------------------
# 4. Setup final variables
# ----------------------------------------------------------------------------
# Consolidate found or user-overridden includes/libraries into standard variables:
set(SDL2_INCLUDE_DIRS "${SDL2_INCLUDE_DIR}")
set(SDL2_LIBRARIES "${SDL2_LIBRARY}")

# ----------------------------------------------------------------------------
# 5. Check and report
# ----------------------------------------------------------------------------
find_package_handle_standard_args(SDL2
    REQUIRED_VARS SDL2_INCLUDE_DIRS SDL2_LIBRARIES
    FAIL_MESSAGE "Could NOT find SDL2 (missing: SDL.h library). Install libsdl2-dev / sdl2-devel?"
)

# For external modules that might check these “old style” variables:
if(SDL2_FOUND)
    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
    set(SDL2_LIBRARIES   ${SDL2_LIBRARIES})
    # SDL2_LIBRARY is a legacy variable name you might see in older code
    set(SDL2_LIBRARY     ${SDL2_LIBRARIES})
endif()

message(STATUS "SDL2_FOUND: ${SDL2_FOUND}")
message(STATUS "SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
