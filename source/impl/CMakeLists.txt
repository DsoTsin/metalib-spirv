set(vk_SOURCES
    vk/common.cpp
    vk/device.cpp
    vk/pipeline.cpp
    vk/commands.cpp
    vk/resource.cpp
    ../../include/metal/common.h
    ../../include/metal/device.h
    ../../include/metal/enums.h
    ../../include/metal/pipeline.h
    ../../include/metal/resource.h
)

source_group(vk FILES ${vk_SOURCES})

add_library(vk_metal SHARED ${vk_SOURCES})
target_include_directories(vk_metal PRIVATE ${CMAKE_SOURCE_DIR}/include)