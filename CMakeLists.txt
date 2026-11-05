cmake_minimum_required(VERSION 3.15)

project(StudentManager VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(StudentManager
    src/main.c
    src/student_manager.c
)

# 头文件目录，让 #include "student_manager.h" 能被找到
target_include_directories(StudentManager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (MSVC)
    target_compile_options(StudentManager PRIVATE /W3)
    # 关闭 MSVC 对 scanf 的"不安全函数"告警（C4996）
    target_compile_definitions(StudentManager PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(StudentManager PRIVATE -Wall -Wextra)
endif()
