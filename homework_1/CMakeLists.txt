cmake_minimum_required(VERSION 3.5)

project(homework_1)

add_executable(homework_1 hello.c main.c)

target_include_directories(homework_1 PRIVATE "./include")