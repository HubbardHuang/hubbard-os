#ifndef FORMATTED_H
#define FORMATTED_H

#include "type.h"
#include "vargs.h"

namespace hubbardos {
namespace interface {

size_t SetStringWithFormattedMessage(char* buffer, size_t buffer_length,
                                     const char* message, ...);
size_t FormatMessage(char* buffer, size_t buffer_length, const char* message,
                     va_list& arg);

} // namespace interface
} // namespace hubbardos

#endif