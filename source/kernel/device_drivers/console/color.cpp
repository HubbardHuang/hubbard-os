#include "color.h"

// using namespace hubbardos::kernel;

namespace hubbardos {
namespace kernel {

enum ConsoleColor {
    kBlack = 0,
    kBlue = 1,
    kGreen = 2,
    kCyan = 3,
    kRed = 4,
    kMagenta = 5,
    kBrown = 6,
    kLightGray = 7,
    kDarkGray = 8,
    kLightBlue = 9,
    kLightGreen = 10,
    kLightCyan = 11,
    kLightRed = 12,
    kLightMagenta = 13,
    kLightBrown = 14,
    kWhite = 15
};

static const uint8_t kDefaultBackgroudColor = kWhite;
static const uint8_t kDefaultForegroudColor = kLightBlue;

BackgroudColor::BackgroudColor(uint8_t color)
  : backgroud_color_(color) {}

uint8_t
BackgroudColor::GetValue(void) const {
    return backgroud_color_;
}

BackgroudColor
BackgroudColor::Black(void) {
    return BackgroudColor(kBlack);
}

BackgroudColor
BackgroudColor::Blue(void) {
    return BackgroudColor(kBlue);
}

BackgroudColor
BackgroudColor::Green(void) {
    return BackgroudColor(kGreen);
}

BackgroudColor
BackgroudColor::Cyan(void) {
    return BackgroudColor(kCyan);
}

BackgroudColor
BackgroudColor::Red(void) {
    return BackgroudColor(kRed);
}

BackgroudColor
BackgroudColor::Magenta(void) {
    return BackgroudColor(kMagenta);
}

BackgroudColor
BackgroudColor::Brown(void) {
    return BackgroudColor(kBrown);
}

BackgroudColor
BackgroudColor::LightGray(void) {
    return BackgroudColor(kLightGray);
}

BackgroudColor
BackgroudColor::DarkGray(void) {
    return BackgroudColor(kDarkGray);
}

BackgroudColor
BackgroudColor::LightBlue(void) {
    return BackgroudColor(kLightBlue);
}

BackgroudColor
BackgroudColor::LightGreen(void) {
    return BackgroudColor(kLightGreen);
}

BackgroudColor
BackgroudColor::LightCyan(void) {
    return BackgroudColor(kLightCyan);
}

BackgroudColor
BackgroudColor::LightRed(void) {
    return BackgroudColor(kLightRed);
}

BackgroudColor
BackgroudColor::LightMagenta(void) {
    return BackgroudColor(kLightMagenta);
}

BackgroudColor
BackgroudColor::LightBrown(void) {
    return BackgroudColor(kLightBrown);
}

BackgroudColor
BackgroudColor::White(void) {
    return BackgroudColor(kWhite);
}

BackgroudColor
BackgroudColor::Default(void) {
    return BackgroudColor(kDefaultBackgroudColor);
}

ForegroudColor::ForegroudColor(uint8_t color)
  : foregroud_color_(color) {}

uint8_t
ForegroudColor::GetValue(void) const {
    return foregroud_color_;
}

ForegroudColor
ForegroudColor::Black(void) {
    return ForegroudColor(kBlack);
}

ForegroudColor
ForegroudColor::Blue(void) {
    return ForegroudColor(kBlue);
}

ForegroudColor
ForegroudColor::Green(void) {
    return ForegroudColor(kGreen);
}

ForegroudColor
ForegroudColor::Cyan(void) {
    return ForegroudColor(kCyan);
}

ForegroudColor
ForegroudColor::Red(void) {
    return ForegroudColor(kRed);
}

ForegroudColor
ForegroudColor::Magenta(void) {
    return ForegroudColor(kMagenta);
}

ForegroudColor
ForegroudColor::Brown(void) {
    return ForegroudColor(kBrown);
}

ForegroudColor
ForegroudColor::LightGray(void) {
    return ForegroudColor(kLightGray);
}

ForegroudColor
ForegroudColor::DarkGray(void) {
    return ForegroudColor(kDarkGray);
}

ForegroudColor
ForegroudColor::LightBlue(void) {
    return ForegroudColor(kLightBlue);
}

ForegroudColor
ForegroudColor::LightGreen(void) {
    return ForegroudColor(kLightGreen);
}

ForegroudColor
ForegroudColor::LightCyan(void) {
    return ForegroudColor(kLightCyan);
}

ForegroudColor
ForegroudColor::LightRed(void) {
    return ForegroudColor(kLightRed);
}

ForegroudColor
ForegroudColor::LightMagenta(void) {
    return ForegroudColor(kLightMagenta);
}

ForegroudColor
ForegroudColor::LightBrown(void) {
    return ForegroudColor(kLightBrown);
}

ForegroudColor
ForegroudColor::White(void) {
    return ForegroudColor(kWhite);
}

ForegroudColor
ForegroudColor::Default(void) {
    return ForegroudColor(kDefaultForegroudColor);
}

} // namespace kernel
} // namespace hubbardos