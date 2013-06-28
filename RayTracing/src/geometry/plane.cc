#include "geometry/plane.hh"

shared_ptr<Trace> Plane::getTrace(const Ray &ray, ft far) const
{
  auto trace = make_shared<PlaneTrace>();
  trace->N = N;
  trace->inside = false;
  ft p = (base - ray.O).dot(N);
  if (p > 0) {
    trace->N *= -1;
    p = -p;
  }
  ft q = trace->N.dot(ray.D);
  if (q > -eps)
    return nullptr;

  trace->dist = p / q;
  if (trace->dist >= far)
    return nullptr;

  trace->IP = ray(trace->dist);
  return trace;
}
