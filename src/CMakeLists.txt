add_library(sitcmalloc SHARED
    sys_alloc.cpp
    PageHeap.cpp
    Span.cpp
    CentralCache.cpp
    ThreadCache.cpp
    sitcmalloc.cpp
)

