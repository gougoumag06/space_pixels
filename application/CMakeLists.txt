
set(NAME application)
add_executable(${NAME})

file(GLOB_RECURSE
    PROJECT_SOURCE_FILES CONFIGURE_DEPENDS
    "src/*.cpp" "src/*.c"
)
file(GLOB_RECURSE
    PROJECT_HEADER_FILES CONFIGURE_DEPENDS
    "src/*.hpp" "src/*.h"
)

target_compile_features(${NAME} PUBLIC c_std_11)
target_compile_definitions(${NAME} PUBLIC _CRT_SECURE_NO_WARNINGS)

target_sources(${NAME} PRIVATE
    ${PROJECT_SOURCE_FILES}
    ${PROJECT_HEADER_FILES}
)

set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake_targets")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src"
    PREFIX "sources"
    FILES ${PROJECT_SOURCE_FILES} ${PROJECT_HEADER_FILES}
)

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(${NAME} PUBLIC ${MATH_LIBRARY})
endif()

#-------------------------------------------------------------------------------
# Third party libraries

if(TARGET SDL2::SDL2main)
    target_link_libraries(${NAME} PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(${NAME} PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)

#-------------------------------------------------------------------------------
# Other include directories

target_include_directories(
    ${NAME} PUBLIC
    "src"
)

#-------------------------------------------------------------------------------
# Copy DLL and files

if(MSVC)
    function(copy_dll PATH_TO_DLL)
        foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
            message(STATUS "[INFO] Copy: " ${PATH_TO_DLL} " to " ${CONFIG})
            file(
                COPY ${PATH_TO_DLL}
                DESTINATION "${CMAKE_SOURCE_DIR}/_bin/${CONFIG}"
            )
        endforeach(CONFIG)
    endfunction()
    copy_dll("${THIRD_PARTY_SDL2_DIR}/lib/x64/SDL2.dll")
    copy_dll("${THIRD_PARTY_SDL2_IMAGE_DIR}/lib/x64/SDL2_image.dll")
    copy_dll("${THIRD_PARTY_SDL2_TTF_DIR}/lib/x64/SDL2_ttf.dll")
    copy_dll("${THIRD_PARTY_SDL2_MIXER_DIR}/lib/x64/SDL2_mixer.dll")
endif()
