#include "console.h"
#include "array.h"
#include "formatted.h"
#include "io_port.h"
#include "vargs.h"
#include "virtual_memory.h"

using namespace hubbardos::interface;

namespace hubbardos {
namespace kernel {

bool Console::title_drawn = false;

static const uint8_t kDefaultTableLength = 4;
static uint16_t* const kVramMappingMemory =
  reinterpret_cast<uint16_t*>(0xB8000 + VirtualMemory::kOffset_);
static const size_t kBufferLength = 1024;

static inline uint16_t
CreateByteWithColor(const char character, const BackgroudColor& backgroud_color,
                    const ForegroudColor& foregroud_color) {
    return character |
           (((backgroud_color.GetValue() << 4) | (foregroud_color.GetValue() & 0x0F))
            << 8);
}

Console::Console()
  : table_length_(kDefaultTableLength)
  , start_x_(0)
  , start_y_(0)
  , end_x_(kXMax_)
  , end_y_(kYMax_)
  , cursor_x_(start_x_)
  , cursor_y_(start_y_)
  , kColumn_(end_x_ - cursor_x_ + 1)
  , kRow_(end_y_ - cursor_y_ + 1) {
    for (uint16_t i = 0; i < kRow_; i++) {
        mapping_memory_[i] = kVramMappingMemory + start_x_ + (start_y_ + i) * kColumnMax_;
    }
}

Console::Console(const Location& start, const Location& end)
  : table_length_(kDefaultTableLength)
  , start_x_(start.GetX())
  , start_y_(start.GetY())
  , end_x_(end.GetX())
  , end_y_(end.GetY())
  , cursor_x_(start_x_)
  , cursor_y_(start_y_)
  , kColumn_(end_x_ - cursor_x_ + 1)
  , kRow_(end_y_ - cursor_y_ + 1) {
    for (uint16_t i = 0; i < kRow_; i++) {
        mapping_memory_[i] = kVramMappingMemory + start_x_ + (start_y_ + i) * kColumnMax_;
    }
}

Console&
Console::Instance(void) {
    static Console singleton;
    return singleton;
}

Console&
Console::SubInstance(void) {
    static Console sub_singleton(Location(5, 5), Location(kXMax_ - 5, kYMax_ - 5));
    return sub_singleton;
}

void
Console::DisplayCursor(void) {
    uint16_t cursor_location = cursor_x_ + cursor_y_ * kColumnMax_; // 绝对地址

    IoPort::Instance().WriteByte(0x3D4, 14);
    IoPort::Instance().WriteByte(0x3D5, cursor_location >> 8);
    IoPort::Instance().WriteByte(0x3D4, 15);
    IoPort::Instance().WriteByte(0x3D5, cursor_location);
}

void
Console::Scroll(void) {
    uint16_t blank =
      CreateByteWithColor(' ', BackgroudColor::Default(), ForegroudColor::Default());
    if (cursor_y_ <= end_y_) {
        return;
    }

    for (uint16_t r = 0; r < kRow_ - 1; r++) {
        for (uint16_t c = 0; c < kColumn_; c++) {
            mapping_memory_[r][c] = mapping_memory_[r + 1][c];
        }
    }

    for (uint16_t i = 0; i < kColumn_; i++) {
        mapping_memory_[kRow_ - 1][i] = blank;
    }

    cursor_y_ = end_y_;
}

void
Console::Clear(void) {
    uint16_t blank =
      CreateByteWithColor(' ', BackgroudColor::Default(), ForegroudColor::Default());
    for (uint16_t r = 0; r < kRow_; r++) {
        for (uint16_t c = 0; c < kColumn_; c++) {
            mapping_memory_[r][c] = blank;
        }
    }

    cursor_x_ = start_x_;
    cursor_y_ = start_y_;
    DisplayCursor();
}

void
Console::PrintCharacter(char character, const BackgroudColor& backgroud_color,
                        const ForegroudColor& foregroud_color) {
    if (character == '\b' && cursor_x_ != 0) {
        cursor_x_--;
    } else if (character == '\t') {
        cursor_x_ = table_length_ * (1 + cursor_x_ / table_length_);
    } else if (character == '\r') {
        cursor_x_ = start_x_;
    } else if (character == '\n') {
        cursor_x_ = start_x_;
        cursor_y_++;
    } else if (character >= ' ') {
        mapping_memory_[cursor_y_ - start_y_][cursor_x_ - start_x_] =
          CreateByteWithColor(character, backgroud_color, foregroud_color);
        cursor_x_++;
    }

    if (cursor_x_ > end_x_) {
        cursor_x_ = start_x_;
        cursor_y_++;
    }

    Scroll();
    DisplayCursor();
}

void
Console::PrintString(const char* string, const BackgroudColor& backgroud_color,
                     const ForegroudColor& foregroud_color) {
    for (size_t i = 0; string[i] != '\0'; i++) {
        PrintCharacter(string[i], backgroud_color, foregroud_color);
    }
}

void
Console::PrintBinary(uint32_t number, const BackgroudColor& backgroud_color,
                     const ForegroudColor& foregroud_color) {
    char* buffer = ToString(number, NumberSystem::kBinary);
    PrintString(buffer, backgroud_color, foregroud_color);
}

void
Console::PrintDecimalInteger(uint32_t number, const BackgroudColor& backgroud_color,
                             const ForegroudColor& foregroud_color) {
    char* buffer = ToString(number, NumberSystem::kDecimal);
    PrintString(buffer, backgroud_color, foregroud_color);
}

void
Console::PrintDecimalInteger(int32_t number, const BackgroudColor& backgroud_color,
                             const ForegroudColor& foregroud_color) {
    char* buffer = ToString(number, NumberSystem::kDecimal);
    PrintString(buffer, backgroud_color, foregroud_color);
}

void
Console::PrintHexadecimal(uint32_t number, const BackgroudColor& backgroud_color,
                          const ForegroudColor& foregroud_color) {
    char* buffer = ToString(number, NumberSystem::kHexadecimal);
    PrintString(buffer, backgroud_color, foregroud_color);
}

void
Console::PrintCharacter(char character) {
    PrintCharacter(character, BackgroudColor::Default(), ForegroudColor::Default());
}

void
Console::PrintString(const char* string) {
    PrintString(string, BackgroudColor::Default(), ForegroudColor::Default());
}

void
Console::PrintBinary(uint32_t number) {
    PrintBinary(number, BackgroudColor::Default(), ForegroudColor::Default());
}

void
Console::PrintDecimalInteger(uint32_t number) {
    PrintDecimalInteger(number, BackgroudColor::Default(), ForegroudColor::Default());
}

void
Console::PrintDecimalInteger(int32_t number) {
    PrintDecimalInteger(number, BackgroudColor::Default(), ForegroudColor::Default());
}

void
Console::PrintHexadecimal(uint32_t number) {
    PrintHexadecimal(number, BackgroudColor::Default(), ForegroudColor::Default());
}

size_t
Console::PrintFormatted(const BackgroudColor& backgroud_color,
                        const ForegroudColor& foregroud_color, const char* message, ...) {
    char buffer[kBufferLength];
    va_list arg_list;
    va_start(arg_list, message);
    size_t message_length = FormatMessage(buffer, kBufferLength, message, arg_list);
    va_end(arg_list);
    PrintString(buffer, backgroud_color, foregroud_color);
    return message_length;
}

size_t
Console::PrintFormatted(const char* message, ...) {
    char buffer[kBufferLength] = { 0 };
    va_list arg_list;
    va_start(arg_list, message);
    size_t message_length = FormatMessage(buffer, kBufferLength, message, arg_list);
    va_end(arg_list);
    PrintString(buffer, BackgroudColor::Default(), ForegroudColor::Default());
    return message_length;
}

void
Console::DrawTitle(const char* title) {
    if (title_drawn == true) {
        return;
    }

    int gap = 8;
    int title_length = GetLength(title);
    int left_space_length = (end_x_ - start_x_ + 1) / 2 - (title_length / 2 + gap);
    int middle_length = (title_length / 2 + gap) * 2 + 2;

    for (int i = 0; i < left_space_length; i++) {
        PrintFormatted(" ");
    }
    for (int i = 0; i < middle_length; i++) {
        PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightGray(), "-");
    }
    PrintFormatted("\n");
    /**********************************************/

    for (int i = 0; i < left_space_length; i++) {
        PrintFormatted(" ");
    }
    PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightGray(), "|");
    for (int i = 0; i < middle_length - 2; i++) {
        PrintFormatted(" ");
    }
    PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightGray(), "|\n");

    for (int i = 0; i < left_space_length; i++) {
        PrintFormatted(" ");
    }
    PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightGray(), "|");
    for (int i = 0; i < gap - (title_length & 0x1); i++) {
        PrintFormatted(" ");
    }
    PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightRed(), "%s", title);
    for (int i = 0; i < gap; i++) {
        PrintFormatted(" ");
    }
    PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightGray(), "|\n");
    /****************************************/

    for (int i = 0; i < left_space_length; i++) {
        PrintFormatted(" ");
    }
    PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightGray(), "|");
    for (int i = 0; i < middle_length - 2; i++) {
        PrintFormatted(" ");
    }
    PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightGray(), "|\n");

    for (int i = 0; i < left_space_length; i++) {
        PrintFormatted(" ");
    }
    for (int i = 0; i < middle_length; i++) {
        PrintFormatted(BackgroudColor::Default(), ForegroudColor::LightGray(), "-");
    }
    PrintFormatted("\n");

    title_drawn = true;
}

} // namespace kernel
} // namespace hubbardos