cmake_minimum_required(VERSION 3.10)
project(GameAnalysisSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(GameAnalysisSystem
        main.cpp
        DataLoader.cpp
        Item.cpp
        Enemy.cpp
        Hero.cpp
        BattleUnit.cpp
        CombatEngine.cpp
)

target_include_directories(GameAnalysisSystem PRIVATE ${CMAKE_SOURCE_DIR}/header\ files)
target_include_directories(GameAnalysisSystem PRIVATE ${CMAKE_SOURCE_DIR}/extern)

# Копирование JSON файлов в папку сборки
file(COPY ${CMAKE_SOURCE_DIR}/Items.json ${CMAKE_SOURCE_DIR}/enemies.json
     DESTINATION ${CMAKE_BINARY_DIR})

