cmake_minimum_required(VERSION 3.13)
project(Engine)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")  # Debugging information
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

include_directories(
        ./External-Libs/RakNet/Include
        ./External-Libs/SDL/include
        ./Engine
)

if(APPLE)
    link_directories(
        ./External-Libs/RakNet/Lib/macOS
    )
else()  # Linux
    link_directories(
        ./External-Libs/RakNet/Lib/Linux
    )
endif()

set(ENGINE_SOURCES
        ./Engine/AnimatedSprite.cpp
        ./Engine/Asset.cpp
        ./Engine/AssetManager.cpp
        ./Engine/AudioSystem.cpp
        ./Engine/BoxCollider.cpp
        ./Engine/CircleCollider.cpp
        ./Engine/Collider.cpp
        ./Engine/CollisionSystem.cpp
        ./Engine/Component.cpp
        ./Engine/Engine.cpp
        ./Engine/Engine_Register.cpp
        ./Engine/EngineMath.cpp
        ./Engine/EngineTime.cpp
        ./Engine/Entity.cpp
        ./Engine/FontAsset.cpp
        ./Engine/FontSprite.cpp
        ./Engine/GameEngine.cpp
        ./Engine/InputSystem.cpp
        ./Engine/IRenderable.cpp
        ./Engine/json.cpp
        ./Engine/mersenne.cpp
        ./Engine/MusicAsset.cpp
        ./Engine/NetworkEngine.cpp
        ./Engine/NetworkRPC.cpp
        ./Engine/Object.cpp
        ./Engine/RenderSystem.cpp
        ./Engine/Scene.cpp
        ./Engine/SceneManager.cpp
        ./Engine/SoundAsset.cpp
        ./Engine/Sprite.cpp
        ./Engine/TextureAsset.cpp
        ./Engine/DebugDraw.cpp
        ./Engine/Color.cpp
        ./Engine/Transform.cpp
        ./Engine/TypeClass.cpp
        )

add_library(engine STATIC ${ENGINE_SOURCES})

target_link_libraries(engine SDL2::SDL2 SDL2_image::SDL2_image raknet SDL2_ttf::SDL2_ttf SDL2_mixer::SDL2_mixer)

set(LIBRARY_OUTPUT_PATH  "${CMAKE_CURRENT_SOURCE_DIR}/Engine-Libs")
