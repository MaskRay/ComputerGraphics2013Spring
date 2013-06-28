#include "AABB.hh"

ft AABB::intersectRay(const Ray &ray, ft near, ft far) const
{
  Vec u = (lo - ray.O) / ray.D;
  Vec v = (hi - ray.O) / ray.D;
#define GO(x) \
  if (u.x < v.x) { \
    setMax(near, u.x); \
    setMin(far, v.x); \
  } else { \
    setMax(near, v.x); \
    setMin(far, u.x); \
  }
  GO(x);
  GO(y);
  GO(z);
#undef GO
  return far < near - eps || far < 0 ? numeric_limits<ft>::max() : near;
}
