cmake_minimum_required(VERSION 4.2)
project(ultimate_texture_packer)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SRC src/*.cpp)

set(IMGUI_DIR external/imgui)
set(IMGUI
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
)

set(RL_IMGUI_DIR external/rlImGui)
set(RL_IMGUI ${RL_IMGUI_DIR}/rlImGui.cpp)

add_executable(ultimate_texture_packer ${SRC} ${IMGUI} ${RL_IMGUI})

include_directories(src/)
include_directories(external/rlImGui)
include_directories(external/imgui/)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} dwmapi)
endif ()

add_subdirectory(external/raylib)
target_link_libraries(${PROJECT_NAME} raylib)

include_directories(external/raygui/src)

add_subdirectory(external/nativefiledialog-extended)
target_link_libraries(${PROJECT_NAME} nfd)

add_subdirectory(external/pugixml)
target_link_libraries(${PROJECT_NAME} pugixml)

add_subdirectory(external/RectangleBinPack)
target_link_libraries(${PROJECT_NAME} RectangleBinPack)

add_subdirectory(external/eventpp)
target_link_libraries(${PROJECT_NAME} eventpp)