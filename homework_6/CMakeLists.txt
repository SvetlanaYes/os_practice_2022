cmake_minimum_required(VERSION 3.5)

project(ping_pong)

add_executable(ping_singular "ping_singular.c")
target_link_libraries(ping_singular PRIVATE pthread)

add_executable(pong_singular "pong_singular.c")
target_link_libraries(pong_singular PRIVATE pthread)