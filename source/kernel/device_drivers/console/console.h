#ifndef CONSOLE_H
#define CONSOLE_H

#include "color.h"
#include "location.h"

namespace hubbardos {
namespace kernel {

class Console {
public:
    Console(const Location& start, const Location& end);
    static Console& Instance(void);
    static Console& SubInstance(void);

    void Clear(void);
    void SetTableLength(uint8_t length);

    void PrintCharacter(char character, const BackgroudColor& backgroud_color,
                        const ForegroudColor& foregroud_color);
    void PrintString(const char* string, const BackgroudColor& backgroud_color,
                     const ForegroudColor& foregroud_color);
    void PrintBinary(uint32_t number, const BackgroudColor& backgroud_color,
                     const ForegroudColor& foregroud_color);
    void PrintDecimalInteger(uint32_t number, const BackgroudColor& backgroud_color,
                             const ForegroudColor& foregroud_color);
    void PrintDecimalInteger(int32_t number, const BackgroudColor& backgroud_color,
                             const ForegroudColor& foregroud_color);
    void PrintHexadecimal(uint32_t number, const BackgroudColor& backgroud_color,
                          const ForegroudColor& foregroud_color);

    void PrintCharacter(char character);
    void PrintString(const char* string);
    void PrintBinary(uint32_t number);
    void PrintDecimalInteger(uint32_t number);
    void PrintDecimalInteger(int32_t number);
    void PrintHexadecimal(uint32_t number);

    size_t PrintFormatted(const BackgroudColor& backgroud_color,
                          const ForegroudColor& foregroud_color, const char* message,
                          ...);
    size_t PrintFormatted(const char* message, ...);

    void DrawTitle(const char* title);

    static const uint8_t kColumnMax_ = 80;
    static const uint8_t kRowMax_ = 25;
    static const uint8_t kXMax_ = kColumnMax_ - 1;
    static const uint8_t kYMax_ = kRowMax_ - 1;

private:
    uint8_t table_length_;
    uint8_t start_x_;
    uint8_t start_y_;
    uint8_t end_x_;
    uint8_t end_y_;
    uint8_t cursor_x_;
    uint8_t cursor_y_;

    const uint8_t kColumn_;
    const uint8_t kRow_;
    uint16_t* mapping_memory_[kRowMax_];

    static bool title_drawn;

    void Scroll(void);
    void DisplayCursor(void);

    Console();
    Console(const Console&);
    Console& operator=(const Console&);
};

} // namespace kernel
} // namespace hubbardos

#endif