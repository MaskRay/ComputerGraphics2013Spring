#include <algorithm>
#include <cassert>
#include "common.hh"
#include "KdTree.hh"
using namespace std;

typedef vector<int> VI;

const int NODE_SIZE_THREASHOLD = 8;

struct Event
{
  bool type;
  int cid, id;
  ft pos;
  bool operator<(const Event& r) const {
    return pos < r.pos;
  }
};

KdTree::Node* KdTree::build(const VI& ids, AABB c)
{
  if (ids.empty())
    return nullptr;
  Node* rt = new Node;
  rt->c = c;
  if (ids.size() <= NODE_SIZE_THREASHOLD) {
    rt->ids = ids;
    rt->c = AABB();
    for (auto i: rt->ids)
      rt->c.extend(aabbs[i]);
    return rt;
  }

  ft pivotpos;
  int pivotcid = -1;
  ft pivotsah = numeric_limits<ft>::max();
  AABB pivotlab, pivotrab;
  vector<Event> es;
  REP(cid, 3) {
    es.clear();
    for (auto i: ids) {
      Event e;
      e.type = true;
      e.id = i;
      e.pos = aabbs[i].lo(cid);
      es.push_back(e);

      e.type = false;
      e.pos = aabbs[i].hi(cid);
      es.push_back(e);
    }
    sort(es.begin(), es.end());

    // extend from left
    AABB lab;
    vector<AABB> labs;
    for (auto j = es.begin(), i = es.begin(); i != es.end(); i = j) {
      for (; j != es.end() && (j == i || j->pos == i->pos); ++j)
        lab.extend(aabbs[j->id]);
      labs.push_back(lab);
    }

    // extend from right
    AABB rab;
    vector<AABB> rabs;
    for (auto j = es.rbegin(), i = es.rbegin(); i != es.rend(); i = j) {
      for (; j != es.rend() && (j == i || j->pos == i->pos); ++j)
        rab.extend(aabbs[j->id]);
      rabs.push_back(rab);
    }
    reverse(ALL(rabs));

    assert(labs.size() == rabs.size());

    int idx = 0, nl = 0, nm = 0, nr = ids.size();
    for (auto j = es.begin(), i = es.begin(); i != es.end(); i = j) {
      for (; j != es.end() && (i == j || j->pos == i->pos); ++j)
        if (j->type)
          nm++, nr--;
        else
          nm--, nl++;
      ft sah = (nl+nm) * labs[idx].area() + (nr+nm) * rabs[idx].area();
      if (sah < pivotsah && nl+nm < ids.size() && nr+nm < ids.size()) {
        pivotsah = sah;
        pivotcid = cid;
        pivotpos = i->pos;
        pivotlab = labs[idx];
        pivotrab = rabs[idx];
      }
      idx++;
    }
  }

  if (pivotsah < ids.size() * c.area()) {
    rt->plane.set(pivotcid, pivotpos);
    VI ls, rs;
    for (auto i: ids)
      if (aabbs[i].hi(pivotcid) <= pivotpos)
        ls.push_back(i);
      else if (pivotpos <= aabbs[i].lo(pivotcid))
        rs.push_back(i);
      else
        ls.push_back(i), rs.push_back(i);
    rt->ch[0] = build(ls, pivotlab);
    rt->ch[1] = build(rs, pivotrab);
  } else {
    rt->ids = ids;
    rt->c = AABB();
    for (auto i: rt->ids)
      rt->c.extend(aabbs[i]);
    return rt;
  }
  return rt;
}

shared_ptr<Trace> KdTree::getTrace(Node *rt, const Ray &ray, ft near, ft far) const
{
  if (rt == nullptr)
    return false;

  shared_ptr<Trace> optTrace, trace;
  if (rt->isLeaf())
    for (auto i: rt->ids) {
      trace = cb(ray, i);
      if (trace && trace->dist < far) {
        optTrace = trace;
        far = trace->dist;
      }
    }
  else {
    ft d[] = { rt->ch[0]->c.intersectRay(ray, near, far), rt->ch[1]->c.intersectRay(ray, near, far) };
    int first = d[0] > d[1];
    if (d[first] < numeric_limits<ft>::max()) {
      optTrace = getTrace(rt->ch[first], ray, near, far);
      if (d[first^1] < numeric_limits<ft>::max() && (! optTrace || optTrace->dist > d[first^1])) {
        trace = getTrace(rt->ch[first^1], ray, near, optTrace ? optTrace->dist : far);
        if (trace) optTrace = trace;
      }
    }
  }
  return optTrace;
}
