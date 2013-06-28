#include "Geometry.hh"
#include "prim.hh"

shared_ptr<Trace> Prim::getTrace(const Ray &ray, ft far)
{
  shared_ptr<Trace> trace = geo->getTrace(ray, far);
  if (trace && trace->dist < far) {
    trace->prim = this;
    return trace;
  }
  return nullptr;
}
