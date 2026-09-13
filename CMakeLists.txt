cmake_minimum_required(VERSION 3.15)
project(Lone LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/extern/SFML")
list(APPEND CMAKE_PREFIX_PATH "${SFML_ROOT}")
list(APPEND CMAKE_PREFIX_PATH "${SFML_ROOT}/lib/cmake/SFML")

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System Audio)

add_executable(Lone 
    src/main.cpp 
    src/Player.cpp 
    src/Projectile.cpp
    src/Enemy.cpp
    src/FallenHuman.cpp
    src/Room.cpp)
target_link_libraries(Lone PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Audio)
target_compile_definitions(Lone PRIVATE SFML_STATIC)


if(WIN32)
    add_custom_command(TARGET Lone POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_ROOT}/bin"
        $<TARGET_FILE_DIR:Lone>
    )

    add_custom_command(TARGET Lone POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:Lone>/assets"
    )
endif()
