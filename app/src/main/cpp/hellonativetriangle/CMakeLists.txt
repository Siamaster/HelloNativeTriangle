set(SOURCE_FILES
        include/file.h
        include/game_loop.h
        include/logger.h
        include/renderer.h
        include/shader.h
        src/file.cpp
        src/game_loop.cpp
        src/renderer.cpp
        src/shader.cpp)

add_library(hellonativetriangle ${SOURCE_FILES})
target_link_libraries(hellonativetriangle EGL GLESv3 glm)
target_include_directories(hellonativetriangle PUBLIC include)

# Creates a shader folder in the Android assets directory and puts the shader files there.
add_custom_target(ALWAYS_DO_POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSETS_DIRECTORY}/shaders
        COMMAND cp shaders/* ${ASSETS_DIRECTORY}/shaders/
        COMMAND ${CMAKE_COMMAND} -E touch .always_do_post_build.h
        COMMAND ${CMAKE_COMMAND} -E echo \"// Hack to always perform a post build operation\" > .always_do_post_build.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Makes sure the above command runs everytime we build
add_dependencies(hellonativetriangle ALWAYS_DO_POST_BUILD)