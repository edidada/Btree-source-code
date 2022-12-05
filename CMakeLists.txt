cmake_minimum_required(VERSION 3.12)
project(btree-source-code VERSION 1.0.0 LANGUAGES C)

add_subdirectory(btree2)
add_subdirectory(jaluta)
add_subdirectory(latch-free-skiplist)
add_subdirectory(threads2)
add_executable(btree-source-code systime.c)
target_link_libraries(btree-source-code pthread)
