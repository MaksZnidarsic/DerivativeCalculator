


cmake_minimum_required(VERSION 3.20)

project(main)
add_executable(${PROJECT_NAME} src/main.c)

#linking the math.h library
#set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(src)
