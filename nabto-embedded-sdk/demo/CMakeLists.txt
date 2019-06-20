cmake_minimum_required(VERSION 3.2)

set(src
  gopt/gopt.c
  test_device.c
  )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(test_device ${src})
target_link_libraries(test_device nabto_device)
