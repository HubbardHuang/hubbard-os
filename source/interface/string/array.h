#ifndef ARRAY_H
#define ARRAY_H

#include "type.h"

namespace hubbardos {
namespace interface {

enum class NumberSystem {
    kDecimal = 10,
    kHexadecimal = 16,
    kBinary = 2,
};

char* ToString(const uint32_t source, NumberSystem number_system);
char* ToString(const int32_t source, NumberSystem number_system);
void SetMember(void* target, const size_t length, const char value);
size_t GetLength(const char* target);
char* CopyString(char* destination, const char* source);
void CopyString(char* destination, const char* source, size_t start, size_t end);
int FindCharacter(const char* source, const char target);
int Compare(const char* string_a, const char* string_b);

inline bool
IsDigit(const char character) {
    return (character >= '0' && character <= '9') ? (true) : (false);
}

template <typename T>
void
Swap(T& a, T& b) {
    if (a == b) {
        return;
    }
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
void
ReverseArray(T array[], size_t start, size_t end) {
    for (size_t i = start; i <= ((start + end) / 2); i++) {
        Swap(array[i], array[start + end - i]);
    }
}

} // namespace interface
} // namespace hubbardos

#endif