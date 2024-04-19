cmake_minimum_required(VERSION 3.13)
project(GameProject)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

include_directories(
        ../Engine/External-Libs/RakNet/Include
        ../Engine/External-Libs/SDL/include
        ../Engine/Engine
        ./GameProject
)

if(APPLE)
    link_directories(
            /Library/Frameworks
            ../Engine/External-Libs/RakNet/Lib/macOS
    )
else()  # Linux
    link_directories(
            ../Engine/External-Libs/RakNet/Lib/Linux
    )
endif()

set(GAME_SOURCES
        ./GameProject/Player.cpp
        ./GameProject/Game_Register.cpp
        ./GameProject/GameOver.cpp
        ./GameProject/Asteroid.cpp
        ./GameProject/AsteroidSpawner.cpp
        ./GameProject/Projectile.cpp
        ./GameProject/main.cpp
)

add_executable(game ${GAME_SOURCES})

if(TARGET SDL2::SDL2main)
    # It has an implicit dependency on SDL2 functions, so it MUST be added before SDL2::SDL2 (or SDL2::SDL2-static)
    target_link_libraries(game PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(
        game
        PRIVATE
        engine
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        raknet
)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Build/bin")
