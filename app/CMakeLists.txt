cmake_minimum_required(VERSION 3.4.1)

set(ASSETS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/main/assets)

add_subdirectory(src/main/cpp/jni)
add_subdirectory(src/main/cpp/hellonativetriangle)
add_subdirectory(src/main/cpp/libs)
