cmake_minimum_required(VERSION 3.5...4.0)
project(rotating-cube VERSION 1.0 LANGUAGES C)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE src/rotating-cube.c)

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE src/init-win.c)
elseif(UNIX)
    target_sources(${PROJECT_NAME} PRIVATE src/init-pos.c)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "inc")