cmake_minimum_required(VERSION 3.21)
project(JsonClass)

set(CMAKE_CXX_STANDARD 17)

add_executable(JsonClass main.cpp moduls/Json.hpp cJSON/cJSON.c cJSON/cJSON_Utils.c cJSON/cJSON_Utils.h cJSON/cJSON.h)
