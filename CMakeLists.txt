cmake_minimum_required(VERSION 3.24.0)
project(hide_seek VERSION 0.1.0 LANGUAGES C)

# ==========================================
# DIRECTORIES CONFIGURATION
# ==========================================
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)


# DEBUG CONFIGURATION
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 -Wall")



# ==========================================
# PLATFORM-SPECIFIC SETTINGS
# ==========================================
if(WIN32)
    set(LP_SOLVE ${LIB_DIR}/lp_solve_win)
    set(SDL2_DIR "C:/SDL2")
    set(SDL2_IMAGE_DIR "C:/SDL2_image")
    set(SDL2_MIXER_DIR "C:/SDL2_mixer")
elseif(UNIX)
    set(LP_SOLVE ${LIB_DIR}/lp_solve_ux)
endif()

# ==========================================
# DEPENDENCIES - SDL2
# ==========================================
if(WIN32)
    # Windows - Use provided paths
    include_directories(${SDL2_DIR}/include ${SDL2_IMAGE_DIR}/include ${SDL2_MIXER_DIR}/include)
    link_directories(${SDL2_DIR}/lib/x64 ${SDL2_IMAGE_DIR}/lib/x64 ${SDL2_MIXER_DIR}/lib/x64)
    set(SDL2_LIBS SDL2 SDL2main)
    set(SDL2_IMAGE_LIBS SDL2_image)
    set(SDL2_MIXER_LIBS SDL2_mixer)
elseif(UNIX)
    find_package(SDL2 REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS})
    set(SDL2_LIBS ${SDL2_LIBRARIES})
    set(SDL2_IMAGE_LIBS ${SDL2_IMAGE_LIBRARIES})
    set(SDL2_MIXER_LIBS ${SDL2_MIXER_LIBRARIES})
endif()

# Find SDL2_ttf package
find_package(SDL2_ttf REQUIRED)

# Find SDL2_mixer package
if(WIN32)
    # Windows configuration
    # Add SDL2_mixer paths if needed
else()
    # Linux configuration
    find_package(SDL2_mixer REQUIRED)
    include_directories(${SDL2_MIXER_INCLUDE_DIRS})
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)

include_directories(${CAIRO_INCLUDE_DIRS})
link_directories(${CAIRO_LIBRARY_DIRS})


# ==========================================
# INCLUDES & SOURCE FILES
# ==========================================
include_directories(${INCLUDE_DIR} ${LP_SOLVE})
file(GLOB_RECURSE SOURCES ${SOURCE_DIR}/*.c)

# ==========================================
# BUILD TARGETS
# ==========================================
# Main executable
add_executable(hide_seek ${SOURCES})

# Link Cairo libraries
target_link_libraries(hide_seek
    ${CAIRO_LIBRARIES}
    SDL2 SDL2_image
)

# Set output directory and disable PIE
set_target_properties(hide_seek PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    POSITION_INDEPENDENT_CODE FALSE  # Disable PIE
)

# Disable PIE for gcc specifically
if(UNIX)
    set_property(TARGET hide_seek PROPERTY COMPILE_FLAGS "-fno-pie")
    set_property(TARGET hide_seek PROPERTY LINK_FLAGS "-no-pie")
endif()

# ==========================================
# LINKING
# ==========================================
if(WIN32)
    target_link_libraries(hide_seek
        ${LP_SOLVE}/lpsolve55.lib
        ${SDL2_LIBS}
        ${SDL2_IMAGE_LIBS}
        ${SDL2_MIXER_LIBS}
        SDL2_ttf
        SDL2_mixer  # Add SDL2_mixer here
    )

    # Copy DLLs to output directory for Windows
    add_custom_command(TARGET hide_seek POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_DIR}/lib/x64/SDL2.dll
            ${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll
            ${SDL2_MIXER_DIR}/lib/x64/SDL2_mixer.dll
            ${OUTPUT_DIR}
        COMMENT "Copying SDL2 DLLs to bin directory"
    )
elseif(UNIX)
    target_link_libraries(hide_seek
        ${LP_SOLVE}/liblpsolve55.a
        ${SDL2_LIBS}
        ${SDL2_IMAGE_LIBS}
        ${SDL2_MIXER_LIBS}
        SDL2_ttf
        SDL2_mixer  # Add SDL2_mixer here
        m  # Add the math library explicitly
    )
endif()

# Add SDL2_ttf include directories to your target
target_include_directories(hide_seek PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

# ==========================================
# UTILITY TARGETS
# ==========================================
# Clean target - works on both Windows and Unix
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${OUTPUT_DIR}/hide_seek${CMAKE_EXECUTABLE_SUFFIX}
    COMMENT "Cleaning all build files"
)

# ==========================================
# BUILD INFORMATION
# ==========================================
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Source directory: ${SOURCE_DIR}")
message(STATUS "Include directory: ${INCLUDE_DIR}")
message(STATUS "Output directory: ${OUTPUT_DIR}")
if(WIN32)
    message(STATUS "SDL2 directory: ${SDL2_DIR}")
    message(STATUS "SDL2_image directory: ${SDL2_IMAGE_DIR}")
    message(STATUS "SDL2_mixer directory: ${SDL2_MIXER_DIR}")
endif()