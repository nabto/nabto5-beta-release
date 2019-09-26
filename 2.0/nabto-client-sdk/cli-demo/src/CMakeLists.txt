cmake_minimum_required(VERSION 3.2)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

set(src test_client.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

add_executable(test_client "${src}")
target_link_libraries(test_client nabto_client)
