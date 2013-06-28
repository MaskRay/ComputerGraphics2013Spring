#pragma once

#include "common.hh"
#include "AABB.hh"

class Trace;

class Geometry
{
public:
  virtual ~Geometry() {}
  virtual shared_ptr<Trace> getTrace(const Ray &ray, ft far) const = 0;
  virtual AABB getAABB() const {}
  bool finite = true;
};
