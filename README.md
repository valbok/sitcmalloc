# Simplified Thread-Caching Malloc

Simplified version of tcmalloc: http://goog-perftools.sourceforge.net/doc/tcmalloc.html

*sitcmalloc* vs *tcmalloc*
- No heap or cpu profiling.
- Span objects are located at the same memory with data. Memory: [span|-----data----] vs [span]->[-----data-----]
- No optimizations, no special tricks, no magic maths etc. (e.g a span consists of blocks or chunks of memory, tcmalloc returns only limited amount of blocks while sitcmalloc returns all list or different conditions when objects have to be returned back from thread cache to central cache and from central cache to page heap etc.)
- No support of 32 bit systems. Related to Radix Tree to map addresses to it's span.
- Only GCC distributions.
- No mmap or /dev/mem allocators, only sbrk is supported.
- No sizemap with its magic.
- No additional allocators for internal needs expect for Radix Tree nodes.
- No special memory alignment expect spans aligned by pages and memalign().
- And no other really useful needed features from tcmalloc.

and -std=c++11 supported.

===
Build

$ cmake .; make

===
Usage

  $ LD_PRELOAD="/usr/lib/libsitcmalloc.so" firefox

