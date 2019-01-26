#include "formatted.h"
#include "array.h"

namespace hubbardos {
namespace interface {

// using namespace hubbardos::interface;

static const size_t kBufferLength = 1024;
static const size_t kParsedLength = 20;
static const char formated_type_flag[] = { 'd', 'u', 'c', 's', 'p',
                                           'f', 'g', 'x', 'o', 'b' };

enum Type {
    kSignedInt32,
    kUnsignedInt32,
    kSignedInt16,
    kUnsignedInt16,
    kSignedInt8,
    kUnsignedInt8,
    kAddress,
    kCharacter,
    kString,
    kHexadecimal32,
    kHexadecimal16,
    kHexadecimal8,
    kBinary32,
    kBinary16,
    kBinary8,
    kOctal32,
    kOctal16,
    kOctal8
};

typedef struct FormatedResult {
    uint8_t type;
    uint8_t width;
    uint8_t decimal_place;
} FormatedResult;

static void
ParseFormatedCommand(va_list& arg_list, char target[], size_t target_length,
                     FormatedResult& formated_result) {
    // spilit the target[] into 3 parts: digit before point, digit after point,
    // word after digit

    if (!Compare(target, "d") || !Compare(target, "i") || !Compare(target, "ld")) {
        formated_result.type = kSignedInt32;
    } else if (!Compare(target, "u") || !Compare(target, "lu")) {
        formated_result.type = kUnsignedInt32;
    } else if (!Compare(target, "hd")) {
        formated_result.type = kSignedInt16;
    } else if (!Compare(target, "hu")) {
        formated_result.type = kUnsignedInt16;
    } else if (!Compare(target, "hhd")) {
        formated_result.type = kSignedInt8;
    } else if (!Compare(target, "hhu")) {
        formated_result.type = kUnsignedInt8;
    } else if (!Compare(target, "hu")) {
        formated_result.type = kUnsignedInt16;
    } else if (!Compare(target, "c")) {
        formated_result.type = kCharacter;
    } else if (!Compare(target, "s")) {
        formated_result.type = kString;
    } else if (!Compare(target, "p")) {
        formated_result.type = kAddress;
    } else if (!Compare(target, "x")) {
        formated_result.type = kHexadecimal32;
    } else if (!Compare(target, "hx")) {
        formated_result.type = kHexadecimal16;
    } else if (!Compare(target, "hhx")) {
        formated_result.type = kHexadecimal8;
    } else if (!Compare(target, "o")) {
        formated_result.type = kOctal32;
    } else if (!Compare(target, "ho")) {
        formated_result.type = kOctal16;
    } else if (!Compare(target, "hho")) {
        formated_result.type = kOctal8;
    } else if (!Compare(target, "b")) {
        formated_result.type = kBinary32;
    } else if (!Compare(target, "hb")) {
        formated_result.type = kBinary16;
    } else if (!Compare(target, "hhb")) {
        formated_result.type = kBinary8;
    }

    return;
}

static void
CatchArgument(va_list& arg_list, FormatedResult& formated_result, char*& buffer) {
    uint8_t uint8;
    int8_t int8;
    int32_t int32;
    uint32_t uint32;
    int16_t int16;
    uint16_t uint16;
    char ch;
    uint32_t string;

    switch (formated_result.type) {
        case kSignedInt32:
            int32 = static_cast<int32_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(int32, NumberSystem::kDecimal));
            break;
        case kUnsignedInt32:
            uint32 = static_cast<uint32_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint32, NumberSystem::kDecimal));
            break;
        case kSignedInt16:
            int16 = static_cast<int16_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(int16, NumberSystem::kDecimal));
            break;
        case kUnsignedInt16:
            uint16 = static_cast<uint16_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint16, NumberSystem::kDecimal));
            break;
        case kSignedInt8:
            int8 = static_cast<int8_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(int8, NumberSystem::kDecimal));
            break;
        case kUnsignedInt8:
            uint8 = static_cast<uint8_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint8, NumberSystem::kDecimal));
            break;
        case kAddress:
            uint32 = static_cast<uint32_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint32, NumberSystem::kDecimal));
            break;
        case kCharacter:
            ch = static_cast<char>(va_arg(arg_list, int));
            *buffer++ = ch;
            break;
        case kString:
            string = static_cast<uint32_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, (const char*)string);
            break;
        case kHexadecimal32:
            uint32 = static_cast<uint32_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint32, NumberSystem::kHexadecimal));
            break;
        case kHexadecimal16:
            uint16 = static_cast<uint16_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint16, NumberSystem::kHexadecimal));
            break;
        case kHexadecimal8:
            uint8 = static_cast<uint8_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint8, NumberSystem::kHexadecimal));
            break;
        case kBinary32:
            uint32 = static_cast<uint32_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint32, NumberSystem::kBinary));
            break;
        case kBinary16:
            uint16 = static_cast<uint16_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint16, NumberSystem::kBinary));
            break;
        case kBinary8:
            uint8 = static_cast<uint8_t>(va_arg(arg_list, int));
            buffer = CopyString(buffer, ToString(uint8, NumberSystem::kBinary));
            break;
        default:
            break;
    }
}

size_t
SetStringWithFormattedMessage(char* buffer, size_t buffer_length, const char* message,
                              ...) {
    return 1;
}
size_t
FormatMessage(char* buffer, size_t buffer_length, const char* message,
              va_list& arg_list) {
    char parsed[kParsedLength] = { 0 };
    size_t parsed_count = 0;
    char* buffer_iterator = buffer;
    FormatedResult formated_result;

    bool formated = false;
    size_t length = GetLength(message);
    length = length < kBufferLength ? length : kBufferLength;

    for (size_t i = 0; i < length; i++) {
        switch (message[i]) {
            case '%':
                formated = true;
                parsed_count = 0;
                while (formated == true && parsed_count < kParsedLength) {
                    i++;
                    parsed[parsed_count] = message[i];
                    for (size_t k = 0; formated_type_flag[k] != '\0'; k++) {
                        if (parsed[parsed_count] == formated_type_flag[k]) {
                            formated = false;
                            break;
                        }
                    }
                    parsed_count++;
                }
                ParseFormatedCommand(arg_list, parsed, parsed_count, formated_result);
                CatchArgument(arg_list, formated_result, buffer_iterator);
                // SetMember(parsed, parsed_count, '\0');
                break;
            default:
                *buffer_iterator++ = message[i];
                break;
        }
    }
    // SetMember(buffer, buffer_length, '\0');
    return (buffer_iterator - buffer);
}

} // namespace interface
} // namespace hubbardos