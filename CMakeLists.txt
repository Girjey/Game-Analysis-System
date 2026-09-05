cmake_minimum_required(VERSION 3.16)
project(GameAnalysisSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(CPR_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# fixes for links
set(HAVE_SYS_ERRLIST OFF CACHE BOOL "" FORCE)
set(HAVE_DECL_SYS_ERRLIST OFF CACHE BOOL "" FORCE)
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)

include(FetchContent)

# CPR
FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 1.10.x
)
FetchContent_MakeAvailable(cpr)



# ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        docking
)
FetchContent_MakeAvailable(imgui)

# GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.3.8
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# --------------------------------------

# Game Analysis
add_executable(GameAnalysisSystem
        main.cpp
        BattleUnit.cpp
        CombatEngine.cpp
        DataLoader.cpp
        Enemy.cpp
        Hero.cpp
        Item.cpp

        # Исходники ImGui и бэкендов
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Linking
target_link_libraries(GameAnalysisSystem PRIVATE
        cpr::cpr
        glfw
        opengl32
        ws2_32
        crypt32
)

target_include_directories(GameAnalysisSystem PRIVATE
        "${CMAKE_SOURCE_DIR}/header files"
        "${CMAKE_SOURCE_DIR}/extern"


        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)