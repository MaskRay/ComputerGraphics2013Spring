#pragma once

#include "common.hh"
#include "AABB.hh"
#include "KdTree.hh"
#include "Light.hh"
#include "prim.hh"
#include "Trace.hh"

class Scene
{
public:
  void addPrim(Prim *prim);
  void addLight(Light *prim);
  void complete();
  Color recursive(const Ray &ray, int depth, ft reflect, ft energy);
  Color pathTracing(const Ray &ray, int depth);
  Color trace(const Ray &ray);
  shared_ptr<Trace> findNearest(const Ray &ray) const;
  bool isOccluded(Vec A, Vec B) const;
  bool use_kd_tree = false;
  ft ambient = 0.05, density = 0.01;
protected:
  void resetTracer();
  void addTracer(Color);
  vector<Prim *> finiteObjs, infiniteObjs;
  vector<Light *> lights;
  AABB bound;

  // valid iff use_kd_tree
  vector<AABB> aabbs;
  KdTree *kdtree;
};
