#pragma once

#include "Geometry.hh"
#include "prim.hh"
#include "Trace.hh"

class PlaneTrace;

class Plane : public Geometry
{
public:
  Plane(Vec base, Vec D1, Vec D2) : base(base), D1(D1.unit()), D2(D2.unit()), N(D1.cross(D2).unit()) { finite = false; }
  shared_ptr<Trace> getTrace(const Ray &ray, ft far) const override;
  Vec base, D1, D2, N;
};

class PlaneTrace : public Trace
{
public:
  Vec2 uv() const override {
    auto plane = (Plane *)prim->geo;
    Vec R = IP - plane->base;
    ft u = fmod(R.dot(plane->D1), 1);
    if (u < 0) u += 1;
    ft v = fmod(R.dot(plane->D2), 1);
    if (v < 0) v += 1;
    return Vec2(u, v);
  }
};
