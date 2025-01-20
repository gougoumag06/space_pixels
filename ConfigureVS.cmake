if(VS_DEBUG_RELEASE)
    message(STATUS "[INFO] Configurations: DEBUG; RELEASE")
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
endif()
