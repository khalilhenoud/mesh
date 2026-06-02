cmake_minimum_required(VERSION 3.22)

project(mesh VERSION 1.0)

add_library(${PROJECT_NAME} SHARED
        ./source/mesh_asset.c
        ./source/skinned_mesh_asset.c
        ./include/mesh/internal/module.h
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC library
        PUBLIC math)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/include"
        )