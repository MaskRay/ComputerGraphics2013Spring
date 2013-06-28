#pragma once

#include "common.hh"

class AABB
{
public:
  AABB(Vec lo = Vec(numeric_limits<ft>::max()), Vec hi = Vec(- numeric_limits<ft>::max())) : lo(lo), hi(hi) {}
  ft intersectRay(const Ray &ray, ft near, ft far) const;
  AABB operator+(Vec O) const { return AABB(lo + O, hi + O); }
  ft area() const {
    Vec d = hi - lo;
    return d.x * d.y + d.y * d.z + d.z * d.x;
  }
  void extend(const AABB &o) {
    setMin(lo.x, o.lo.x);
    setMin(lo.y, o.lo.y);
    setMin(lo.z, o.lo.z);
    setMax(hi.x, o.hi.x);
    setMax(hi.y, o.hi.y);
    setMax(hi.z, o.hi.z);
  }
  int id;
  Vec lo, hi;
};
