cmake_minimum_required(VERSION 3.5)

project(homework_3)

add_library(ipc SHARED ./ipc/ipc.c)
target_include_directories(ipc PRIVATE "./include")

add_executable(homework_3 main.c)
target_include_directories(homework_3 PRIVATE "./include")
target_link_libraries(homework_3 PRIVATE ipc)
target_link_libraries(homework_3 PRIVATE pthread)