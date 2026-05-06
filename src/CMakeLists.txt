cmake_minimum_required(VERSION 3.16)
project(qsynq)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SQLiteCpp REQUIRED)

add_library(repo STATIC components/index-db/repo/file-repository.cpp)
target_link_libraries(repo PRIVATE SQLiteCpp )
