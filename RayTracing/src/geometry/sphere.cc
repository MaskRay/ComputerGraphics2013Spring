#include "geometry/sphere.hh"

AABB Sphere::getAABB() const
{
  return AABB(O - Vec(D), O + Vec(D));
}

// http://wiki.cgsociety.org/index.php/Ray_Sphere_Intersection
shared_ptr<Trace> Sphere::getTrace(const Ray &ray, ft far) const
{
  shared_ptr<SphereTrace> trace = make_shared<SphereTrace>();

  Vec rayO = ray.O - O;

  ft a = ray.D.dot(ray.D);
  ft b = 2 * ray.D.dot(rayO);
  ft c = rayO.dot(rayO) - D * D;

  ft disc = b * b - 4 * a * c;
  if (disc < 0) return nullptr;

  disc = sqrt(disc);
  ft q = b < 0 ? (-b - disc) * 0.5 : (-b + disc) * 0.5;
  ft t0 = q / a, t1 = c / q;
  if (t0 > t1) swap(t0, t1);

  if (t1 < 0) return nullptr;
  trace->dist = t0 < 0 ? t1 : t0;
  if (trace->dist >= far) return nullptr;

  trace->IP = ray(trace->dist);
  trace->N = (trace->IP - O).unit();
  trace->inside = (trace->dist * ray.D).dot(trace->N) > 0;
  return trace;
}
