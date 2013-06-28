#ifndef VECTOR_HH
#define VECTOR_HH

#include "Main.hh"

template<typename T>
struct Vector2
{
  T x, y;
  Vector2<T> operator+(const Vector2<T> &rhs) const {
    return Vector2<T>{x+rhs.x, y+rhs.y};
  }
  Vector2<T> operator-(const Vector2<T> &rhs) const {
    return Vector2<T>{x-rhs.x, y-rhs.y};
  }
  T operator*(const Vector2<T> &rhs) const {
    return x * rhs.x + y * rhs.y;
  }
  T cross(const Vector2<T> &rhs) const {
    return x * rhs.y - y * rhs.x;
  }
  friend Vector2<T> operator*(T scale, Vector2<T> a) {
    return Vector2<T>{a.x * scale, a.y * scale};
  }
  Vector2<T> operator*(T scale) const {
    return Vector2<T>{x * scale, y * scale};
  }
  template<typename U>
  operator Vector2<U>() const {
    return Vector2<U>{x, y};
  }
};

template<typename T>
struct Vector3
{
  T x, y, z;
  Vector3<T> operator+(const Vector3<T> &rhs) const {
    return Vector3<T>{x+rhs.x, y+rhs.y, z+rhs.z};
  }
  Vector3<T> operator-(const Vector3<T> &rhs) const {
    return Vector3<T>{x-rhs.x, y-rhs.y, z-rhs.z};
  }
  T operator*(const Vector3<T> &rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }
  friend Vector3<T> operator*(T scale, Vector3<T> a) {
    return Vector3<T>{a.x * scale, a.y * scale, a.z * scale};
  }
  Vector3<T> cross(const Vector3<T> &rhs) const {
    return Vector3<T>{y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x};
  }
  Vector3<T> operator*(T scale) const {
    return Vector3<T>{x * scale, y * scale, z * scale};
  }
  template<typename U>
  operator Vector3<U>() const {
    return Vector3<U>{x, y, z};
  }
};

#endif
