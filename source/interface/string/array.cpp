#include "array.h"
#include "console.h"

using namespace hubbardos::kernel;

namespace hubbardos {
namespace interface {

static const size_t kBufferSize = 512;
static char buffer[kBufferSize];

static void
ToStringFromBinary(const uint32_t source, char* buffer) {
    if (!source) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    size_t i = 31;
    while (((source >> i) & (0x1)) == 0) {
        i--;
    }
    size_t length = i + 1;
    size_t k;
    for (k = 0; k < length; k++, i--) {
        buffer[k] = ((source >> i) & (0x1)) + '0';
    }
    buffer[k] = '\0';
}

static void
ToStringFromDecimal(const uint32_t source, char* buffer) {
    if (!source) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }
    uint32_t temp = source;
    size_t i;
    for (i = 0; i < 10; i++) {
        buffer[i] = (temp % 10) + '0';
        temp /= 10;
    }

    size_t length = 0;
    for (i = 9; i >= 1; i--) {
        if (buffer[i] == '0') {
            buffer[i] = '\0';
        } else {
            length = i + 1;
            break;
        }
    }
    buffer[10] = '\0';
    if (length > 1) {
        ReverseArray(buffer, 0, length - 1);
    }
}

static void
ToStringFromDecimal(const int32_t source, char* buffer) {
    if (!source) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    uint32_t temp = static_cast<uint32_t>(source);

    if ((temp >> 31) & 1) {
        buffer[0] = '-';
        ToStringFromDecimal(static_cast<uint32_t>(~(source - 1)), buffer + 1);
    } else {
        ToStringFromDecimal(static_cast<uint32_t>(source), buffer);
    }
}

static void
ToStringFromHexadecimal(const uint32_t source, char* buffer) {
    if (!source) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    size_t i = 28;
    for (i = 28; !((source >> i) & 0xF); i -= 4) {
        ;
    }
    size_t k;
    for (k = 0; 4 * k <= i; k++) {
        uint8_t temp = (source >> (i - 4 * k)) & 0xF;
        if (temp < 10) {
            buffer[k] = temp - 0 + '0';
        } else {
            buffer[k] = temp - 10 + 'A';
        }
    }
    buffer[k] = '\0';
}

char*
ToString(const uint32_t source, NumberSystem number_system) {
    switch (number_system) {
        case NumberSystem::kBinary:
            ToStringFromBinary(source, buffer);
            break;
        case NumberSystem::kDecimal:
            ToStringFromDecimal(source, buffer);
            break;
        case NumberSystem::kHexadecimal:
            ToStringFromHexadecimal(source, buffer);
            break;
        default:
            break;
    }
    return buffer;
}

char*
ToString(const int32_t source, NumberSystem number_system) {
    switch (number_system) {
        case NumberSystem::kBinary:
            ToStringFromBinary(source, buffer);
            break;
        case NumberSystem::kDecimal:
            ToStringFromDecimal(source, buffer);
            break;
        case NumberSystem::kHexadecimal:
            ToStringFromHexadecimal(source, buffer);
            break;
        default:
            break;
    }
    return buffer;
}

void
SetMember(void* target, const size_t length, const char value) {
    char* start_address = static_cast<char*>(target);
    for (size_t i = 0; i < length; i++) {
        start_address[i] = value;
    }
}

size_t
GetLength(const char* target) {
    size_t length;
    for (length = 1; target[length]; length++) {
        ;
    }
    return length;
}

char*
CopyString(char* destination, const char* source) {
    if ((!source) || (!destination)) {
        return nullptr;
    }

    size_t i;
    for (i = 0; source[i] != '\0'; i++) {
        destination[i] = source[i];
    }
    return destination + i;
}

int
FindCharacter(const char* source, const char target) {
    int position = 0;

    for (; source[position] != '\0'; position++) {
        if (source[position] == target) {
            return position;
        }
    }
    position = -1;

    return position;
}

int
Compare(const char* string_a, const char* string_b) {
    if (string_a == nullptr || string_b == nullptr) {
        return -1;
    }

    size_t i;
    for (i = 0; string_a[i] != '\0' && string_b[i] != '\0'; i++) {
        if (string_a[i] != string_b[i]) {
            return string_a[i] - string_b[i];
        }
    }

    if (string_a[i] == '\0' && string_b[i] == '\0') {
        return 0;
    } else {
        return (string_a[i] == '\0') ? string_b[i] : string_a[i];
    }
}

} // namespace interface
} // namespace hubbardos