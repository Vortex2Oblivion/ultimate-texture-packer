cmake_minimum_required(VERSION 4.2)
project(ultimate_texture_packer)

set(CMAKE_CXX_STANDARD 20)

add_executable(ultimate_texture_packer src/Main.cpp
        src/ui/ImageScrollList.cpp
        src/ui/ImageScrollList.hpp
        src/utils/FileUtil.cpp
        src/utils/FileUtil.hpp
        src/ui/SpritesheetLoadMenu.cpp
        src/ui/Button.cpp)

include_directories(src/)

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