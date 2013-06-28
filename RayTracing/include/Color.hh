#pragma once

typedef double ft;

class Color
{
public:
  Color(ft r = 0, ft g = 0, ft b = 0) : r(r), g(g), b(b) {}
  Color operator+(const Color &o) const { return Color(r + o.r, g + o.g, b + o.b); }
  Color operator-(const Color &o) const { return Color(r - o.r, g - o.g, b - o.b); }
  Color operator*(const Color &o) const { return Color(r * o.r, g * o.g, b * o.b); }
  Color operator*(ft o) const { return Color(r * o, g * o, b * o); }
  friend Color operator*(ft o, const Color &t) { return Color(o * t.r, o * t.g, o * t.b); }
  Color &operator+=(const Color &o) { r += o.r; g += o.g; b += o.b; return *this; }
  Color &operator-=(const Color &o) { r += o.r; g += o.g; b += o.b; return *this; }
  Color &operator*=(ft o) { r *= o; g *= o; b *= o; return *this; }
  ft r, g, b;
protected:
};

extern Color BLACK, WHITE, RED, GREEN, BLUE;
