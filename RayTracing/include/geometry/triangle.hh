#pragma once

#include "Geometry.hh"
#include "prim.hh"
#include "Trace.hh"

class Triangle : public Geometry
{
public:
  Triangle(Vec A, Vec B, Vec C) : A(A), D1(B-A), D2(C-A) {}
  shared_ptr<Trace> getTrace(const Ray &ray, ft far) const override;
  AABB getAABB() const override;
  Vec A, D1, D2;
};

class TriangleTrace : public Trace
{
public:
  Vec2 uv() const override {
    return Vec2(u, v);
  }
  ft u, v;
};
