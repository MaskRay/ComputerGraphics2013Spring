#pragma once

#include "Geometry.hh"
#include "prim.hh"
#include "Trace.hh"

class SphereTrace;

class Sphere : public Geometry
{
public:
  Sphere(Vec O, ft D) : O(O), D(D) {}
  shared_ptr<Trace> getTrace(const Ray &ray, ft far) const override;
  AABB getAABB() const override;
  Vec O;
  ft D;
};

class SphereTrace : public Trace
{
public:
  Vec2 uv() const override {
    auto sphere = (Sphere *)prim->geo;
    Vec R = IP - sphere->O;
    ft phi = acos((max(R.z / sphere->D, ft(-1)), ft(1))),
       theta = atan2(R.y, R.x);
    return Vec2(phi / M_PI, theta / (2 * M_PI) + 0.5);
  }
  Material material() const override {
    Vec2 o = uv();
    Material m = prim->texture->getMaterial(o.x, o.y);
    if (inside) m.refractIdx = AIR_REFRACTIVE_INDEX;
    return m;
  }
};
