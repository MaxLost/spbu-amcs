cmake_minimum_required (VERSION 3.8)

add_library(graph STATIC graph.cpp)
add_library(list STATIC list.cpp)
add_library(dfa STATIC dfa.cpp)

add_executable (DFA "main.cpp" )

target_link_libraries(graph list)
target_link_libraries(dfa graph)
target_link_libraries(DFA dfa)