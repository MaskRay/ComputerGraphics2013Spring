#ifndef RASTERIZATION_HH
#define RASTERIZATION_HH

#include "Vector.hh"

class Context
{
public:
  void plotLine(Vector2<int> a, Vector2<int> b);
  void plotCircle(int x, int y, int radius);
  void plotPolygon(int n, const Vector2<int> a[]);
  void plotPolygon(int n, const Vector2<real_t> a[]);
  void fillPolygon(int n, const Vector2<int> a[]);
  void floodFill(int x, int y);
  void scanlineFill(int x, int y);
  virtual void setPixel(int x, int y) = 0;
  virtual int getPixel(int x, int y) = 0;
  bool inBound(int x, int y) {
    return 0 <= x && x < width && 0 <= y && y < height;
  }
  void setColor(int r, int g, int b) {
    color = (r * 256 + g) * 256 + b;
  }
protected:
  int color, width, height;
};

#endif
