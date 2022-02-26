cmake_minimum_required(VERSION 3.5)

project(homework_2)

add_executable(homework_2 main.c blocking_queue.c)

target_include_directories(homework_2 PRIVATE "./include")
target_link_libraries(homework_2 PRIVATE pthread)
