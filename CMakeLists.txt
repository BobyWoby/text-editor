cmake_minimum_required(VERSION 3.10)
project(text-editor)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #This makes neovim aware of the headers

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_ttf EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
PRIVATE 
    src/main.cpp
    src/PieceTable.cpp
)
add_executable(${PROJECT_NAME}-test src/main_test.cpp src/PieceTable.cpp)

target_link_libraries( ${PROJECT_NAME} PUBLIC
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
)
install(TARGETS text-editor RUNTIME)
