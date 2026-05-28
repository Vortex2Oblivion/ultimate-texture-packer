cmake_minimum_required(VERSION 4.2)
project(ultimate_texture_packer)

set(CMAKE_CXX_STANDARD 20)

add_executable(ultimate_texture_packer src/Main.cpp
        src/ui/ImageScrollList.cpp
        src/ui/ImageScrollList.hpp)

add_subdirectory(external/raylib)
target_link_libraries(${PROJECT_NAME} raylib)

include_directories(external/raygui/src)

add_subdirectory(external/nativefiledialog-extended)
target_link_libraries(${PROJECT_NAME} nfd)