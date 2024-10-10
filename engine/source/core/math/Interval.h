#pragma once
#include "mathcommon.h"

class Interval {
  public:
    float min, max;

    Interval() : min(+infinity), max(-infinity) {} // Default interval is empty

    Interval(float _min, float _max) : min(_min), max(_max) {}


    Interval operator+(float offset) { return Interval(min+offset, max+offset); }
    friend Interval operator+(float offset, const Interval& i) { return Interval(i.min+offset, i.max+offset); }

    bool Contains(float x) const {
        return min <= x && x <= max;
    }

    bool Surrounds(float x) const {
        return min < x && x < max;
    }

    static const Interval empty, universe;
};
