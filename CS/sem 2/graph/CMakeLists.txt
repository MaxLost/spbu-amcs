﻿# CMakeList.txt: файл проекта CMake верхнего уровня; проведите глобальную конфигурацию
# и включите сюда подпроекты.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDART 11)
project ("GRAPH")

# Включите подпроекты.
add_subdirectory ("GRAPH")
