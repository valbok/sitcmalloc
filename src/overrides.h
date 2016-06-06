/**
 * @author VaL Doroshchuk <valbok@gmail.com>
 * @date May 2016
 * @copyright VaL Doroshchuk
 * @license GNU GPL v2
 */

#ifndef SITCMALLOC_OVERRIDES_H
#define SITCMALLOC_OVERRIDES_H

#if defined(__GNUC__)
#include "overrides_libc.h"
#else
#error No implementation for current libc
#endif

#endif  // SITCMALLOC_OVERRIDES_H

