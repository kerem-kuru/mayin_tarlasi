cmake_minimum_required(VERSION 3.20)
project(mayn_tarlas)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/SDL3-3.4.4/x86_64-w64-mingw32/include")
link_directories("C:/SDL3-3.4.4/x86_64-w64-mingw32/lib")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} SDL3)
target_link_libraries(${PROJECT_NAME} SDL3 SDL3_ttf)