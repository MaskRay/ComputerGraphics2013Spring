#include <limits>
#include "geometry/mesh.hh"
using namespace std;

Mesh::~Mesh()
{
  if (use_kd_tree)
    delete kdtree;
}

void Mesh::addVertex(Vec O)
{
  vs.push_back(Vertex(O));
  setMin(bound.lo.x, O.x);
  setMin(bound.lo.y, O.y);
  setMin(bound.lo.z, O.z);
  setMax(bound.hi.x, O.x);
  setMax(bound.hi.y, O.y);
  setMax(bound.hi.z, O.z);
}

void Mesh::addFace(int p, int q, int r)
{
  fs.push_back(Face(&vs[p], &vs[q], &vs[r]));
}

void Mesh::complete()
{
  use_kd_tree = fs.size() > 8;
  use_kd_tree = true;

  if (use_kd_tree) {
    vector<int> ids(fs.size());
    aabbs.resize(fs.size());
    REP(i, fs.size())
      aabbs[i] = fs[i].toTri().getAABB();
    iota(ALL(ids), 0);
    auto cb = [&](const Ray &ray, int id) {
      return fs[id].toTri().getTrace(ray, numeric_limits<ft>::max());
    };
    kdtree = new KdTree(&aabbs[0], ids, bound, cb);
  }
}

shared_ptr<Trace> Mesh::getTrace(const Ray &ray, ft far) const
{
  shared_ptr<Trace> optTrace;

  if (use_kd_tree)
    optTrace = kdtree->getTrace(ray, far);
  else {
    for (auto f: fs) {
      auto tri = f.toTri();
      shared_ptr<Trace> trace = tri.getTrace(ray, far);
      if (trace) {
        optTrace = trace;
        far = trace->dist;
      }
    }
  }

  return optTrace;
}
