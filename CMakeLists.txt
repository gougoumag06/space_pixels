cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(NAME space_pixels)
project(${NAME} VERSION 1.0)

option(VS_DEBUG_RELEASE "Generate only DEBUG and RELEASE configuration on VS" ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_bin/")

message(STATUS "[INFO] Current directory: " ${CMAKE_SOURCE_DIR})

if(MSVC)
    include(ConfigureVS.cmake)
endif()

#-------------------------------------------------------------------------------
# Third party libraries

if(MSVC)
    message(STATUS "MSCV DETECTED")
    set(THIRD_PARTY_SDL2_DIR       "${CMAKE_SOURCE_DIR}/third_party/SDL2-2.30.6")
    set(THIRD_PARTY_SDL2_IMAGE_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2_image-2.8.2")
    set(THIRD_PARTY_SDL2_TTF_DIR   "${CMAKE_SOURCE_DIR}/third_party/SDL2_ttf-2.22.0")
    set(THIRD_PARTY_SDL2_MIXER_DIR "${CMAKE_SOURCE_DIR}/third_party/SDL2_mixer-2.8.0")

    list(APPEND CMAKE_PREFIX_PATH
        "${THIRD_PARTY_SDL2_DIR}/cmake"
        "${THIRD_PARTY_SDL2_IMAGE_DIR}/cmake"
        "${THIRD_PARTY_SDL2_TTF_DIR}/cmake"
        "${THIRD_PARTY_SDL2_MIXER_DIR}/cmake"
    )
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

#-------------------------------------------------------------------------------
# Projects

add_subdirectory(application)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT application)
endif()
