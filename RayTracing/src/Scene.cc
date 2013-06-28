#include <limits>
#include "Geometry.hh"
#include "Light.hh"
#include "prim.hh"
#include "Scene.hh"
using namespace std;

void Scene::addPrim(Prim *prim)
{
  if (prim->geo->finite) {
    AABB ab = prim->geo->getAABB();
    finiteObjs.push_back(prim);
    setMin(bound.lo.x, ab.lo.x);
    setMin(bound.lo.y, ab.lo.y);
    setMin(bound.lo.z, ab.lo.z);
    setMax(bound.hi.x, ab.hi.x);
    setMax(bound.hi.y, ab.hi.y);
    setMax(bound.hi.z, ab.hi.z);
  } else
    infiniteObjs.push_back(prim);
}

void Scene::addLight(Light *light)
{
  lights.push_back(light);
}

void Scene::complete()
{
  use_kd_tree = finiteObjs.size() > 50;
  printf("objects: %zd\n", finiteObjs.size());
  printf("aabb: (%.2lf, %.2lf, %.2lf) (%.2lf, %.2lf, %.2lf)\n",
         bound.lo.x, bound.lo.y, bound.lo.z,
         bound.hi.x, bound.hi.y, bound.hi.z);

  if (use_kd_tree) {
    vector<int> ids(finiteObjs.size());
    aabbs.resize(finiteObjs.size());
    REP(i, finiteObjs.size())
      aabbs[i] = finiteObjs[i]->geo->getAABB();
    iota(ALL(ids), 0);
    auto cb = [&](const Ray &ray, int id) {
      return finiteObjs[id]->getTrace(ray, numeric_limits<ft>::max());
    };
    kdtree = new KdTree(&aabbs[0], ids, bound, cb);
  }
}

Color Scene::trace(const Ray &ray)
{
  return recursive(ray, 0, AIR_REFRACTIVE_INDEX, 1);
}

bool Scene::isOccluded(Vec A, Vec B) const
{
  ft dist = (B-A).norm();
  shared_ptr<Trace> trace = findNearest(Ray::shift(A, B-A));
  return trace && cmp(trace->dist, dist) < 0;
}

Color Scene::recursive(const Ray &ray, int depth, ft refractIdx, ft energy)
{
  if (depth >= 3) return BLACK;
  shared_ptr<Trace> trace = findNearest(ray);
  if (! trace) return BLACK;

  Color res;
  ft dist = trace->dist;
  Vec IP = ray(trace->dist),
      N = trace->N,
      NN = trace->inside ? -N : N;

  Material m = trace->material();
  ft energy2 = energy * exp(- density * trace->dist);

  res += ambient * m.ambient * m.diffuse;

  for (auto &light: lights) {
    ft shade = light->getShade(this, IP);

    if (shade > 0) {
      ft atten = energy2 * exp(- density * trace->dist);

      // diffuse component
      Vec L = (light->O - IP).unit();
      ft dot = L.dot(N);
      if (dot > 0)
        res += dot * shade * m.diffuse * light->intensity * atten;

      // specular component
      Vec R = L - 2 * L.dot(N) * N;
      dot = R.dot(ray.D);
      if (dot > 0)
        res += pow(dot, m.shine) * shade * m.specular * light->intensity * atten;
      //res += dot * m.specular * shade / (50 - 50 * dot + dot) * m.diffuse; (Schlick's approximation)
    }
  }

  // reflection
  if (m.reflect > 0) {
    Vec R = ray.D - 2 * ray.D.dot(NN) * NN;
    Color res2 = recursive(Ray::shift(IP, R), depth + 1, refractIdx, energy2);
    res += m.diffuse * m.reflect * res2;
  }

  // refraction
  if (m.refract > 0) {
    ft n = refractIdx / m.refractIdx;
    ft cosI = - NN.dot(ray.D);
    ft cosT2 = 1 - n * n * (1 - cosI * cosI);
    if (cosT2 > 0) {
      Vec T = n * ray.D + n * (cosI - sqrt(cosT2)) * NN;
      Color res2 = recursive(Ray::shift(IP, T), depth + 1, m.refractIdx, energy2);
      res += m.diffuse * m.refract * res2;
    }
  }

  return res;
}

shared_ptr<Trace> Scene::findNearest(const Ray &ray) const
{
  shared_ptr<Trace> optTrace;
  ft dist = numeric_limits<ft>::max();

  // intersection with finite objs
  if (use_kd_tree)
    optTrace = kdtree->getTrace(ray, dist);
  else {
    int c = 0;
    for (auto &i: finiteObjs) {
      shared_ptr<Trace> trace = i->getTrace(ray, dist);
      if (trace) {
        optTrace = trace;
        dist = trace->dist;
      }
    }
  }

  // intersection with infinite objs
  for (auto &i: infiniteObjs) {
    shared_ptr<Trace> trace = i->getTrace(ray, dist);
    if (trace) {
      optTrace = trace;
      dist = trace->dist;
    }
  }

  return optTrace;
}

Color Scene::pathTracing(const Ray &ray, int depth)
{
  shared_ptr<Trace> trace = findNearest(ray);
  if (! trace) return BLACK;

  Color res;
  ft dist = trace->dist;
  Vec IP = ray(trace->dist),
      N = trace->N,
      NN = trace->inside ? -N : N;

  Material m = trace->material();
  Color diffuse = m.diffuse;
  ft maxdiffuse = max(max(diffuse.r, diffuse.g), diffuse.b);
  if (depth > 4) {
    if (drand48() < maxdiffuse) diffuse *= 1 / maxdiffuse;
    else return m.emission;
  }

  Vec R = ray.D - 2*N.dot(ray.D) * ray.D;

  // ideal diffuse reflection
  if (m.specular == 0) {
    double r1 = 2*M_PI*drand48(), r2 = drand48(), r2s = sqrt(r2);
    Vec w = NN, u = ((fabs(NN.x) > .1 ? Vec(0, 1, 0) : Vec(1, 0, 0)).cross(w)).unit(), v = w.cross(u);
    Vec d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1-r2)).unit();
    return m.emission + diffuse * pathTracing(Ray::shift(IP, d), depth + 1);
  } else if (m.specular == 1) // ideal specular reflection
    return m.emission + diffuse * pathTracing(Ray::shift(IP, R), depth + 1);

  // dielectric refraction

  double nc = AIR_REFRACTIVE_INDEX, nt = m.refractIdx,
         nnt = ! trace->inside ? nc/nt : nt/nc,
         ddn = ray.D.dot(NN), cos2t = 1 - nnt * nnt * (1 - ddn * ddn);
  if (cos2t < 0) // total internal reflection
    return m.emission + diffuse * pathTracing(Ray::shift(IP, R), depth + 1);
  Vec tdir = (ray.D * nnt - N * ((! trace->inside ? 1 : -1) * (ddn*nnt+sqrt(cos2t)))).unit();
  double a = nt-nc, b = nt+nc, R0 = a*a/(b*b), c = 1-(! trace->inside ? -ddn : tdir.dot(N));
  double Re = R0+(1-R0)*c*c*c*c*c, Tr=1-Re, P=.25+.5*Re, RP=Re/P, TP=Tr/(1-P);
  return m.emission + diffuse * (depth > 1 ? (drand48() < P ?
    pathTracing(Ray::shift(IP, R), depth + 1) * RP : pathTracing(Ray::shift(IP, tdir), depth + 1) * TP) :
    pathTracing(Ray::shift(IP, R), depth + 1) * Re + pathTracing(Ray::shift(IP, tdir), depth + 1) * Tr);
}
