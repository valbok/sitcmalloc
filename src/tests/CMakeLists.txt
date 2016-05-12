set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(sys_malloc sys_malloc_test.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(sys_malloc gtest sitcmalloc)

