#pragma once

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>
#include "Color.hh"
using namespace std;

typedef double ft;

#define FOR(i, a, b) for (decltype(b) i = (a); i < (b); i++)
#define REP(i, n) FOR(i, 0, n)
#define SET(a, v) memset(a, v, sizeof a)
#define ALL(a) a.begin(), a.end()

const ft eps = 1e-6;
const int SAMPLES = 1;
const int NTHREADS = 3;

const ft AIR_REFRACTIVE_INDEX = 1;
const ft GLASS_REFRACTIVE_INDEX = 1.5;

class Vec2
{
public:
  Vec2() : x(0), y(0) {}
  Vec2(ft x, ft y) : x(x), y(y) {}
  ft &operator()(int dim) { return (&x)[dim]; }
  ft operator()(int dim) const { return (&x)[dim]; }
  ft normSqr() const { return x * x + y * y; }
  ft norm() const { return sqrt(normSqr()); }
  ft dot(const Vec2 &r) const { return x * r.x + y * r.y; }
  Vec2 operator-() const { return Vec2(-x, -y); }
  Vec2 operator+(const Vec2 &o) const { return Vec2(x + o.x, y + o.y); }
  Vec2 operator-(const Vec2 &o) const { return Vec2(x - o.x, y - o.y); }
  Vec2 operator*(const Vec2 &o) const { return Vec2(x * o.x, y * o.y); }
  Vec2 operator*(ft o) const { return Vec2(x * o, y * o); }
  Vec2 operator/(const Vec2 &o) const { return Vec2(x / o.x, y / o.y); }
  Vec2 operator/(ft o) const { return Vec2(x / o, y / o); }
  friend Vec2 operator*(ft o, const Vec2 &t) { return Vec2(o * t.x, o * t.y); }
  Vec2 &operator+=(const Vec2 &r) { x += r.x; y += r.y; return *this; }
  Vec2 &operator-=(const Vec2 &r) { x -= r.x; y -= r.y; return *this; }
  Vec2 &operator*=(ft r) { x *= r; y *= r; return *this; }
  Vec2 unit() const {
    ft n = 1 / norm();
    Vec2 o(*this);
    o.x *= n;
    o.y *= n;
    return o;
  }
  ft x, y;
};

class Vec
{
public:
  Vec() : x(0), y(0), z(0) {}
  Vec(ft x) : x(x), y(x), z(x) {}
  Vec(ft x, ft y, ft z) : x(x), y(y), z(z) {}
  ft &operator()(int dim) { return (&x)[dim]; }
  ft operator()(int dim) const { return (&x)[dim]; }
  ft normSqr() const { return x * x + y * y + z * z; }
  ft norm() const { return sqrt(normSqr()); }
  ft dot(const Vec &r) const { return x * r.x + y * r.y + z * r.z; }
  Vec cross(const Vec &r) const { return Vec(y*r.z-z*r.y, z*r.x-x*r.z, x*r.y-y*r.x); }
  Vec operator-() const { return Vec(-x, -y, -z); }
  Vec operator+(const Vec &o) const { return Vec(x + o.x, y + o.y, z + o.z); }
  Vec operator-(const Vec &o) const { return Vec(x - o.x, y - o.y, z - o.z); }
  Vec operator*(const Vec &o) const { return Vec(x * o.x, y * o.y, z * o.z); }
  Vec operator*(ft o) const { return Vec(x * o, y * o, z * o); }
  Vec operator/(const Vec &o) const { return Vec(x / o.x, y / o.y, z / o.z); }
  Vec operator/(ft o) const { return Vec(x / o, y / o, z / o); }
  friend Vec operator*(ft o, const Vec &t) { return Vec(o * t.x, o * t.y, o * t.z); }
  Vec &operator+=(const Vec &r) { x += r.x; y += r.y; z += r.z; return *this; }
  Vec &operator-=(const Vec &r) { x -= r.x; y -= r.y; z -= r.z; return *this; }
  Vec &operator*=(ft r) { x *= r; y *= r; z *= r; return *this; }
  Vec unit() const {
    ft n = 1 / norm();
    Vec o(*this);
    o.x *= n;
    o.y *= n;
    o.z *= n;
    return o;
  }
  ft x, y, z;
};

class Mat4_4
{
public:
  Mat4_4() {
    SET(a, 0);
    a[0][0] = a[1][1] = a[2][2] = a[3][3] = 1;
  }
  Vec operator*(const Vec &o) const {
    ft x = a[0][0] * o.x + a[0][1] * o.y + a[0][2] * o.z + a[0][3],
       y = a[1][0] * o.x + a[1][1] * o.y + a[1][2] * o.z + a[1][3],
       z = a[2][0] * o.x + a[2][1] * o.y + a[2][2] * o.z + a[2][3];
    return Vec(x, y, z);
  }
  Mat4_4 operator*(const Mat4_4 &o) const {
    Mat4_4 ret;
    REP(r, 4)
      REP(c, 4)
        ret.a[r][c] = a[r][0] * o(0, c) + a[r][1] * o(1, c) + a[r][2] * o(2, c) + a[r][3] * o(3, c);
    return ret;
  }
  ft &operator()(int i, int j) { return a[i][j]; }
  ft operator()(int i, int j) const { return a[i][j]; }
  Mat4_4 rotateX(ft rx) const {
    Mat4_4 ret;
    return ret;
  }
  Mat4_4 rotateY(ft rx) const {
    Mat4_4 ret;
    return ret;
  }
  Mat4_4 rotateZ(ft rx) const {
    Mat4_4 ret;
    return ret;
  }
  ft a[4][4];
};

class Ray
{
public:
  Ray(Vec O, Vec D) : O(O), D(D.unit()) {}
  Vec operator()(ft dist) const { return O + dist * D; }
  static Ray shift(Vec O, Vec D) { D = D.unit(); return Ray(O + eps * D, D); }
  Vec O, D;
};

static inline int cmp(ft a, ft b)
{
  if (a < b - eps) return -1;
  if (b > a + eps) return 1;
  return 0;
}

static inline int sgn(ft a)
{
  if (a < -eps) return -1;
  if (a > eps) return 1;
  return 0;
}

template<typename T>
inline void setMin(T &a, T b)
{ if (b < a) a = b; }

template<typename T>
inline void setMax(T &a, T b)
{ if (b > a) a = b; }

template<typename T>
inline T min3(T a, T b, T c)
{
  return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

template<typename T>
inline T max3(T a, T b, T c)
{
  return a > b ? (a > c ? a : c) : (b > c ? b : c);
}

static inline ft clamp(ft x)
{
  return x < 0 ? 0 : x > 1 ? 1 : x;
}
