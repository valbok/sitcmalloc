set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(sys_alloc_test sys_alloc_test.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(sys_alloc_test gtest sitcmalloc)

add_executable(PageHeapTest PageHeapTest.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(PageHeapTest gtest sitcmalloc)

add_executable(SpanTest SpanTest.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(SpanTest gtest sitcmalloc)

add_executable(BlockTest BlockTest.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(BlockTest gtest sitcmalloc)

add_executable(FreeListTest FreeListTest.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(FreeListTest gtest sitcmalloc)

add_executable(CentralCacheTest CentralCacheTest.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(CentralCacheTest gtest sitcmalloc)

add_executable(ThreadCacheTest ThreadCacheTest.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(ThreadCacheTest gtest sitcmalloc)

add_executable(common_test common_test.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(common_test gtest sitcmalloc)

add_executable(PageMapTest PageMapTest.cpp)
link_directories(${CMAKE_SOURCE_DIR}/src)
target_link_libraries(PageMapTest gtest sitcmalloc)
