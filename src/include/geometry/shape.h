#pragma once

#include "accessors.h"

#include <cmath>

template <typename It>
void MakeRegularPolygon(size_t n, float theta, It out) {
    const float step = M_PI * 2 / n;
    for (size_t i = 0; i < n; ++i) {
        float angle = i * step + theta;
        GetX(*out) = std::cos(angle);
        GetY(*out) = std::sin(angle);
        ++out;
    }
}

template <typename It>
void MakeRegularPolygon(size_t n, It out) {
    MakeRegularPolygon(n, 0.0f, out);
}

template <typename PolygonT>
void MakeRegularPolygon(size_t n, PolygonT* out) {
    out->resize(n);
    MakeRegularPolygon(n, 0.0f, out->begin());
}

template <typename PolygonT>
PolygonT MakeRegularPolygon(size_t n) {
    PolygonT out(n);
    MakeRegularPolygon(n, 0.0f, out.begin());
    return out;
}
