#ifndef SITCMALLOC_SPAN_H
#define SITCMALLOC_SPAN_H

#include <stddef.h> // for size_t, NULL, ptrdiff_t

namespace sitcmalloc {

/**
 *
 */
class Span {
public:
	Span() : m_data(0), m_length(0) {}
	void init(void* data, size_t length) { 
		m_data = data;
		m_length = length;
	}
private:
	void* m_data;
	size_t m_length;

};


}  // namespace sitcmalloc

#endif  // SITCMALLOC_SPAN_H
