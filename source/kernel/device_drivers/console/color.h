#ifndef COLOR_H
#define COLOR_H

#include "type.h"

namespace hubbardos {
namespace kernel {

class BackgroudColor {
public:
    uint8_t GetValue(void) const;
    static BackgroudColor Black(void);
    static BackgroudColor Blue(void);
    static BackgroudColor Green(void);
    static BackgroudColor Cyan(void);
    static BackgroudColor Red(void);
    static BackgroudColor Magenta(void);
    static BackgroudColor Brown(void);
    static BackgroudColor LightGray(void);
    static BackgroudColor DarkGray(void);
    static BackgroudColor LightBlue(void);
    static BackgroudColor LightGreen(void);
    static BackgroudColor LightCyan(void);
    static BackgroudColor LightRed(void);
    static BackgroudColor LightMagenta(void);
    static BackgroudColor LightBrown(void);
    static BackgroudColor White(void);
    static BackgroudColor Default(void);

private:
        explicit BackgroudColor(uint8_t color);
    uint8_t backgroud_color_;
};

class ForegroudColor {
public:
    uint8_t GetValue(void) const;
    static ForegroudColor Black(void);
    static ForegroudColor Blue(void);
    static ForegroudColor Green(void);
    static ForegroudColor Cyan(void);
    static ForegroudColor Red(void);
    static ForegroudColor Magenta(void);
    static ForegroudColor Brown(void);
    static ForegroudColor LightGray(void);
    static ForegroudColor DarkGray(void);
    static ForegroudColor LightBlue(void);
    static ForegroudColor LightGreen(void);
    static ForegroudColor LightCyan(void);
    static ForegroudColor LightRed(void);
    static ForegroudColor LightMagenta(void);
    static ForegroudColor LightBrown(void);
    static ForegroudColor White(void);
    static ForegroudColor Default(void);

private:
    explicit ForegroudColor(uint8_t color);
    uint8_t foregroud_color_;
};

} // namespace kernel
} // namespace hubbardos

#endif