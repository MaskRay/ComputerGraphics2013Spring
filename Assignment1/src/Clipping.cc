#include <cmath>
#include <functional>
#include <tuple>
#include <vector>
using std::function;
using std::make_tuple;
#include "Clipping.hh"
#include "Util.hh"

const real_t eps = 1e-3;

// 裁剪矩形的四個方向
enum {
  TOP = 1,
  RIGHT = 2,
  BOTTOM = 4,
  LEFT = 8,
};

// 裁剪線段
void LineClipping::cohenSutherland(real_t xmin, real_t xmax, real_t ymin, real_t ymax, Vector2<real_t> a, Vector2<real_t> b, function<void(Vector2<real_t>, Vector2<real_t>)> plotLine)
{
  auto encode = [&](real_t x, real_t y) {
    return (y > ymax ? TOP : 0) | (x > xmax ? RIGHT : 0)
      | (y < ymin ? BOTTOM : 0) | (x < xmin ? LEFT : 0);
  };

  real_t x0 = a.x, y0 = a.y,
         x1 = b.x, y1 = b.y;
  int c0 = encode(x0, y0), c1 = encode(x1, y1);
  for(;;) {
    if (c0 & c1) return;
    if (! (c0 | c1)) break;

    // 計算線段和裁剪矩形的交點
    int c = c0 ? c0 : c1;
    real_t x, y;
    if (c & TOP) {
      x = x0 + (x1-x0) * (ymax-y0) / (y1-y0);
      y = ymax;
    } else if (c & RIGHT) {
      y = y0 + (y1-y0) * (xmax-x0) / (x1-x0);
      x = xmax;
    } else if (c & BOTTOM) {
      x = x0 + (x1-x0) * (ymin-y0) / (y1-y0);
      y = ymin;
    } else if (c & LEFT) {
      y = y0 + (y1-y0) * (xmin-x0) / (x1-x0);
      x = xmin;
    }

    // 把裁剪矩形外的點替換爲交點
    if (c == c0) {
      x0 = x;
      y0 = y;
      c0 = encode(x, y);
    } else {
      x1 = x;
      y1 = y;
      c1 = encode(x, y);
    }
  }

  plotLine(Vector2<real_t>{x0, y0}, Vector2<real_t>{x1, y1});
}

void LineClipping::liangBarsky(real_t xmin, real_t xmax, real_t ymin, real_t ymax, Vector2<real_t> a, Vector2<real_t> b, function<void(Vector2<real_t>, Vector2<real_t>)> plotLine)
{
  real_t tl = 0, tu = 1;
  auto clip = [&](real_t p, real_t q) {
    real_t r = q / p;
    if (p < 0) {
      if (r > tu) return false;
      if (r > tl) tl = r;
      return true;
    } else if (p > 0) {
      if (r < tl) return false;
      if (r < tu) tu = r;
      return true;
    } else
      return q >= 0;
  };

  Vector2<real_t> d = b - a;
  if (clip(- d.x, a.x - xmin) &&
      clip(d.x, xmax - a.x) &&
      clip(- d.y, a.y - ymin) &&
      clip(d.y, ymax - a.y))
    plotLine(a + (b - a) * tl, a + (b - a) * tu);
}

void LineClipping::midpointSubdivision(real_t xmin, real_t xmax, real_t ymin, real_t ymax, Vector2<real_t> a, Vector2<real_t> b, function<void(Vector2<real_t>, Vector2<real_t>)> plotLine)
{
  // 與 Cohen-Sutherland 同樣的編碼方式
  auto encode = [&](real_t x, real_t y) {
    return (y > ymax ? TOP : 0) | (x > xmax ? RIGHT : 0)
      | (y < ymin ? BOTTOM : 0) | (x < xmin ? LEFT : 0);
  };

  function<void(real_t x0, real_t y0, real_t x1, real_t y1)> go;
  go = [&](real_t x0, real_t y0, real_t x1, real_t y1) {
    int c0 = encode(x0, y0), c1 = encode(x1, y1);
    if (c0 & c1 || hypot(x0-x1, y0-y1) < eps) return;
    if (! (c0 | c1))
      plotLine(Vector2<real_t>{x0, y0}, Vector2<real_t>{x1, y1});
    else {
      real_t x = (x0+x1)/2, y = (y0+y1)/2;
      go(x0, y0, x, y);
      go(x, y, x1, y1);
    }
  };

  go(a.x, a.y, b.x, b.y);
}

// 用凸多邊形裁剪多邊形
vector<Vector2<real_t>> PolygonClipping::sutherlandHodgman(int n, const Vector2<real_t> subject[], int m, const Vector2<real_t> clip[])
{
  vector<Vector2<real_t>> out(subject, subject + n);
  Vector2<real_t> c = clip[m-1], c2, s;
  auto inside = [&](Vector2<real_t> a) {
    return (c2 - c).cross(a - c) > 0;
  };
  auto intersect = [&](Vector2<real_t> e) {
    return Util::intersection(c, c2, s, e);
  };
  // 枚舉用於裁剪的邊
  for (int i = 0; i < m; i++) {
    c2 = clip[i];

    vector<Vector2<real_t>> in;
    out.swap(in);
    s = subject[n-1];
    for (auto e : in) {
      // 末端在裁剪多邊形內部
      if (inside(e)) {
        if (! inside(s))
          out.push_back(intersect(e));
        out.push_back(e);
      } else if (inside(s))
        out.push_back(intersect(e));
      s = e;
    }

    c = c2;
  }
  return out;
}
