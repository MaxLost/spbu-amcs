# CMakeList.txt: проект CMake для graph; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Добавьте источник в исполняемый файл этого проекта.
add_library(str_input STATIC str_input.cpp)
add_library(list STATIC list.cpp)
add_library(graph STATIC graph.cpp)
add_library(bipartity_check STATIC bipartity_check.cpp)
add_library(topological_sort STATIC topological_sort.cpp)
add_library(file_input STATIC file_input.cpp)
add_library(scc_partition STATIC scc_partition.cpp)

add_executable (GRAPH main.cpp)

target_link_libraries(graph str_input list)
target_link_libraries(bipartity_check graph)
target_link_libraries(topological_sort graph)
target_link_libraries(file_input graph)
target_link_libraries(scc_partition graph)
target_link_libraries(GRAPH graph bipartity_check topological_sort file_input scc_partition)

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
