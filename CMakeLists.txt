cmake_minimum_required(VERSION 3.24.0)
project(hide_seek VERSION 0.1.0 LANGUAGES C)

# ==========================================
# DIRECTORIES CONFIGURATION
# ==========================================
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/bin)



# ==========================================
# PLATFORM-SPECIFIC SETTINGS
# ==========================================
if(WIN32)
    set(LP_SOLVE ${LIB_DIR}/lp_solve_win)
    set(SDL2_DIR "C:/SDL2")
    set(SDL2_IMAGE_DIR "C:/SDL2_image")
elseif(UNIX)
    set(LP_SOLVE ${LIB_DIR}/lp_solve_ux)
endif()

# ==========================================
# DEPENDENCIES - SDL2
# ==========================================
if(WIN32)
    # Windows - Use provided paths
    include_directories(${SDL2_DIR}/include ${SDL2_IMAGE_DIR}/include)
    link_directories(${SDL2_DIR}/lib/x64 ${SDL2_IMAGE_DIR}/lib/x64)
    set(SDL2_LIBS SDL2 SDL2main)
    set(SDL2_IMAGE_LIBS SDL2_image)
elseif(UNIX)
    find_package(SDL2 REQUIRED)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    set(SDL2_LIBS ${SDL2_LIBRARIES})
    set(SDL2_IMAGE_LIBS ${SDL2_IMAGE_LIBRARIES})
endif()

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

# Set output directory
set_target_properties(hide_seek PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

# ==========================================
# LINKING
# ==========================================
if(WIN32)
    target_link_libraries(hide_seek
        ${LP_SOLVE}/lpsolve55.lib
        ${SDL2_LIBS}
        ${SDL2_IMAGE_LIBS}
    )

    # Copy DLLs to output directory for Windows
    add_custom_command(TARGET hide_seek POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${SDL2_DIR}/lib/x64/SDL2.dll
            ${SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll
            ${OUTPUT_DIR}
        COMMENT "Copying SDL2 DLLs to bin directory"
    )
elseif(UNIX)
    target_link_libraries(hide_seek
        ${LP_SOLVE}/liblpsolve55.a
        ${SDL2_LIBS}
        ${SDL2_IMAGE_LIBS}
    )
endif()

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
endif()







