cmake_minimum_required(VERSION 3.16)

project( Snake LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )

file( GLOB snake_src "*.cpp" )

add_executable(Snake ${snake_src})

find_package( PkgConfig REQUIRED )
pkg_check_modules( SDL2 REQUIRED IMPORTED_TARGET sdl2 )
pkg_check_modules( SDL2_ttf REQUIRED IMPORTED_TARGET SDL2_ttf )
pkg_check_modules( SDL2_image REQUIRED IMPORTED_TARGET SDL2_image )

target_link_libraries( Snake PRIVATE PkgConfig::SDL2 )
target_link_libraries( Snake PRIVATE PkgConfig::SDL2_ttf )
target_link_libraries( Snake PRIVATE PkgConfig::SDL2_image )
