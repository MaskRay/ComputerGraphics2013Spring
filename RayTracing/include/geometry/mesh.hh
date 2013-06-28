#pragma once

#include "Geometry.hh"
#include "geometry/triangle.hh"
#include "KdTree.hh"
#include "prim.hh"
#include "Trace.hh"

class Face;

class Vertex
{
public:
  Vertex(Vec O) : O(O) {}
  Vec O;
  vector<Face *> adjF;
};

class Face
{
public:
  Face(Vertex *p, Vertex *q, Vertex *r) : a{p, q, r}, normal((q->O - p->O).cross(r->O - p->O).unit()) {}
  Vertex &operator[](int i) const { return *a[i]; }
  Triangle toTri() const {
    return Triangle(a[0]->O, a[1]->O, a[2]->O);
  }
  Vertex *a[3];
  Vec normal;
};

class Mesh : public Geometry
{
public:
  virtual ~Mesh();
  shared_ptr<Trace> getTrace(const Ray &ray, ft far) const override;
  AABB getAABB() const override { return bound; }
  void addVertex(Vec);
  void addFace(int, int, int);
  bool use_kd_tree = false;
  void complete();
protected:
  vector<Vertex> vs;
  vector<Face> fs;
  AABB bound;

  // valid iff use_kd_tree
  vector<AABB> aabbs;
  KdTree *kdtree;
};

class MeshTrace : public Trace
{
public:
  Vec2 uv() const override {
    return Vec2(u, v);
  }
  ft u, v;
};
