#ifndef LOCATION_H
#define LOCATION_H

#include "type.h"

class Location {
public:
    Location(uint8_t x, uint8_t y);
    uint8_t GetX(void) const;
    uint8_t GetY(void) const;

private:
    uint8_t x_;
    uint8_t y_;
};

#endif