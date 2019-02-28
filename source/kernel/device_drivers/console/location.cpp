#include "location.h"

Location::Location(uint8_t x, uint8_t y)
  : x_(x)
  , y_(y) {}

uint8_t
Location::GetX(void) const {
    return x_;
}

uint8_t
Location::GetY(void) const {
    return y_;
}