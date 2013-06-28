#pragma once

#include "common.hh"
#include "AABB.hh"
#include "prim.hh"
#include "Trace.hh"

struct AAPlane
{
  int cid;
  ft coord;
  void set(int cid_, ft coord_) {
    cid = cid_;
    coord = coord_;
  }
};

class KdTree
{
public:
  KdTree(const AABB *aabbs, const vector<int> &ids, AABB c, function<shared_ptr<Trace>(const Ray &, int)> cb)
    : aabbs(aabbs), cb(cb)
  { root = build(ids, c); }
  ~KdTree() { delete root; }
  struct Node {
    Node* ch[2];
    AABB c;
    AAPlane plane;
    vector<int> ids;
    Node() : ch{nullptr, nullptr} {}
    ~Node() { delete ch[0]; delete ch[1]; }
    bool isLeaf() { return ch[0] == nullptr && ch[1] == nullptr; }
  };
  shared_ptr<Trace> getTrace(const Ray &ray, ft far) const {
    return getTrace(root, ray, 0, far);
  }
protected:
  shared_ptr<Trace> getTrace(Node *rt, const Ray &ray, ft near, ft far) const;
  Node* build(const vector<int> &, AABB);
  Node* root;
  const AABB* aabbs;
  function<shared_ptr<Trace>(const Ray &, int)> cb;
};
