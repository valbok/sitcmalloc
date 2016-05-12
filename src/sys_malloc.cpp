#include <errno.h>                      // for EAGAIN, errno
#include <stdint.h>                     // for uintptr_t, intptr_t
#include <sys/mman.h>                   // for munmap, mmap, MADV_DONTNEED, etc
#include <unistd.h>                     // for sbrk, getpagesize, off_t
#include "sys_malloc.h"

namespace sitcmalloc {

static size_t pagesize = 0;

namespace {

void* sbrk_alloc(size_t size, size_t *actual_size,
  size_t alignment) {
  // sbrk will release memory if passed a negative number, so we do
  // a strict check here
  if (static_cast<ptrdiff_t>(size + alignment) < 0) return NULL;

  // This doesn't overflow because TCMalloc_SystemAlloc has already
  // tested for overflow at the alignment boundary.
  size = ((size + alignment - 1) / alignment) * alignment;

  // "actual_size" indicates that the bytes from the returned pointer
  // p up to and including (p + actual_size - 1) have been allocated.
  if (actual_size) {
    *actual_size = size;
  }

  // Check that we we're not asking for so much more memory that we'd
  // wrap around the end of the virtual address space.  (This seems
  // like something sbrk() should check for us, and indeed opensolaris
  // does, but glibc does not:
  //    http://src.opensolaris.org/source/xref/onnv/onnv-gate/usr/src/lib/libc/port/sys/sbrk.c?a=true
  //    http://sourceware.org/cgi-bin/cvsweb.cgi/~checkout~/libc/misc/sbrk.c?rev=1.1.2.1&content-type=text/plain&cvsroot=glibc
  // Without this check, sbrk may succeed when it ought to fail.)
  if (reinterpret_cast<intptr_t>(sbrk(0)) + size < size) {
    return NULL;
  }

  void* result = sbrk(size);
  if (result == reinterpret_cast<void*>(-1)) {
    return NULL;
  }

  // Is it aligned?
  uintptr_t ptr = reinterpret_cast<uintptr_t>(result);
  if ((ptr & (alignment-1)) == 0)  return result;

  // Try to get more memory for alignment
  size_t extra = alignment - (ptr & (alignment-1));
  void* r2 = sbrk(extra);
  if (reinterpret_cast<uintptr_t>(r2) == (ptr + size)) {
    // Contiguous with previous result
    return reinterpret_cast<void*>(ptr + extra);
  }

  // Give up and ask for "size + alignment - 1" bytes so
  // that we can find an aligned region within it.
  result = sbrk(size + alignment - 1);
  if (result == reinterpret_cast<void*>(-1)) {
    return NULL;
  }
  ptr = reinterpret_cast<uintptr_t>(result);
  if ((ptr & (alignment-1)) != 0) {
    ptr += alignment - (ptr & (alignment-1));
  }
  return reinterpret_cast<void*>(ptr);
}


void* mmap_alloc(size_t size, size_t *actual_size,
                              size_t alignment) {
  // Enforce page alignment
  if (pagesize == 0) pagesize = getpagesize();
  if (alignment < pagesize) alignment = pagesize;
  size_t aligned_size = ((size + alignment - 1) / alignment) * alignment;
  if (aligned_size < size) {
    return NULL;
  }
  size = aligned_size;

  // "actual_size" indicates that the bytes from the returned pointer
  // p up to and including (p + actual_size - 1) have been allocated.
  if (actual_size) {
    *actual_size = size;
  }

  // Ask for extra memory if alignment > pagesize
  size_t extra = 0;
  if (alignment > pagesize) {
    extra = alignment - pagesize;
  }

  // Note: size + extra does not overflow since:
  //            size + alignment < (1<<NBITS).
  // and        extra <= alignment
  // therefore  size + extra < (1<<NBITS)
  void* result = mmap(NULL, size + extra,
                      PROT_READ|PROT_WRITE,
                      MAP_PRIVATE|MAP_ANONYMOUS,
                      -1, 0);
  if (result == reinterpret_cast<void*>(MAP_FAILED)) {
    return NULL;
  }

  // Adjust the return memory so it is aligned
  uintptr_t ptr = reinterpret_cast<uintptr_t>(result);
  size_t adjust = 0;
  if ((ptr & (alignment - 1)) != 0) {
    adjust = alignment - (ptr & (alignment - 1));
  }

  // Return the unused memory to the system
  if (adjust > 0) {
    munmap(reinterpret_cast<void*>(ptr), adjust);
  }
  if (adjust < extra) {
    munmap(reinterpret_cast<void*>(ptr + adjust + size), extra - adjust);
  }

  ptr += adjust;
  return reinterpret_cast<void*>(ptr);
}
} // namespace

void* sys_alloc(size_t size, size_t *actual_size,
  size_t alignment) {
  return sbrk_alloc(size, actual_size, alignment);
}

bool sys_free(void* start, size_t length) {
  if (pagesize == 0) 
    pagesize = getpagesize();
  const size_t pagemask = pagesize - 1;

  size_t new_start = reinterpret_cast<size_t>(start);
  size_t end = new_start + length;
  size_t new_end = end;

  // Round up the starting address and round down the ending address
  // to be page aligned:
  new_start = (new_start + pagesize - 1) & ~pagemask;
  new_end = new_end & ~pagemask;

  if (new_end > new_start) {
    int result;
    do {
      result = madvise(reinterpret_cast<char*>(new_start),
          new_end - new_start, MADV_DONTNEED);
    } while (result == -1 && errno == EAGAIN);

    return result != -1;
  }
  return false;
}

}  // namespace sitcmalloc
