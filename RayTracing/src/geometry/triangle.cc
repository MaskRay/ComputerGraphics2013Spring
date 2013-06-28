#include "geometry/triangle.hh"

AABB Triangle::getAABB() const
{
  return AABB(Vec(min3(A.x, A.x+D1.x, A.x+D2.x),
                  min3(A.y, A.y+D1.y, A.y+D2.y),
                  min3(A.z, A.z+D1.z, A.z+D2.z)),
              Vec(max3(A.x, A.x+D1.x, A.x+D2.x),
                  max3(A.y, A.y+D1.y, A.y+D2.y),
                  max3(A.z, A.z+D1.z, A.z+D2.z)));
}

shared_ptr<Trace> Triangle::getTrace(const Ray &ray, ft far) const
{
  auto trace = make_shared<TriangleTrace>();
  Vec N = D1.cross(D2).unit();
  trace->N = N;
  trace->inside = false;
  ft p = (A - ray.O).dot(N);
  if (p > 0) {
    trace->N *= -1;
    p = -p;
  }
  ft q = trace->N.dot(ray.D);
  if (q > -eps)
    return nullptr;
  trace->dist = p / q;
  if (trace->dist > far)
    return nullptr;

  trace->IP = ray(trace->dist);

  ft denom = D1.cross(D2).dot(N);
  ft alpha = (D2-D1).cross(trace->IP-(A+D1)).dot(N) / denom;
  ft beta = (-D2).cross(trace->IP-(A+D2)).dot(N) / denom;
  ft gamma = D1.cross(trace->IP-A).dot(N) / denom;

  trace->u = beta;
  trace->v = gamma;
  return alpha >= 0 && beta >= 0 && gamma >= 0 ? trace : nullptr;
}
