cmake_minimum_required(VERSION 3.20)

project(GyaruDatingBattleToxicLove2007 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(SFML_BUILD_AUDIO TRUE CACHE BOOL "" FORCE)
set(SFML_BUILD_GRAPHICS TRUE CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC FALSE CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

add_library(gyaru_model STATIC
    src/model/DialogueScene.cpp
    src/model/GameState.cpp
    src/model/RhythmBattle.cpp
    src/model/SongChart.cpp
)

target_include_directories(gyaru_model PUBLIC src)

if(MSVC)
    target_compile_options(gyaru_model PRIVATE /utf-8)
endif()

add_executable(gyaru_dating_battle
    src/main.cpp
    src/app/Application.cpp
    src/controller/GameController.cpp
    src/model/GameModel.cpp
    src/view/DialogueView.cpp
    src/view/MenuView.cpp
    src/view/ResultView.cpp
    src/view/RhythmView.cpp
    src/view/TextureUtils.cpp
)

target_link_libraries(gyaru_dating_battle
    PRIVATE
        gyaru_model
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
)

target_compile_definitions(gyaru_dating_battle
    PRIVATE
        ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets"
)

if(MSVC)
    target_compile_options(gyaru_dating_battle PRIVATE /utf-8)
endif()

add_custom_command(TARGET gyaru_dating_battle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:sfml-system>
        $<TARGET_FILE:sfml-window>
        $<TARGET_FILE:sfml-graphics>
        $<TARGET_FILE:sfml-audio>
        $<TARGET_FILE_DIR:gyaru_dating_battle>
)

if(WIN32)
    add_custom_command(TARGET gyaru_dating_battle POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${sfml_SOURCE_DIR}/extlibs/bin/x64/openal32.dll"
            $<TARGET_FILE_DIR:gyaru_dating_battle>
    )
endif()

add_executable(gyaru_model_tests
    tests/model_tests.cpp
)

target_link_libraries(gyaru_model_tests PRIVATE gyaru_model)

if(MSVC)
    target_compile_options(gyaru_model_tests PRIVATE /utf-8)
endif()

enable_testing()
add_test(NAME gyaru_model_tests COMMAND gyaru_model_tests)
