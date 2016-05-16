set(CMAKE_BUILD_TYPE Release)
add_library(sitcmalloc SHARED
    sys_alloc.cpp
    PageHeap.cpp
)

