#include "Util.hh"

Vector2<real_t> Util::intersection(Vector2<real_t> a, Vector2<real_t> b, Vector2<real_t> c, Vector2<real_t> d)
{
  Vector2<real_t> p = a - b, q = c - d;
  real_t n1 = a.cross(b), n2 = c.cross(d), n3 = 1.0 / p.cross(q);
  return (n1 * q - n2 * p) * n3;
}
