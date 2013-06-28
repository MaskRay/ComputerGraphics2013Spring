#include <algorithm>
#include <cstdio>
#include <functional>
#include <map>
#include <set>
#include <vector>
using namespace std;
using namespace std::placeholders;

#define FOR(i, a, b) for (decltype(b) i = (a); i < (b); i++)
#define ROF(i, a, b) for (decltype(a) i = (b); --i >= (a); )
#define REP(i, n) FOR(i, 0, n)
#define ALL(a) a.begin(), a.end()

typedef double ft;

template<typename T>
void setMax(T &a, T b)
{ if (b > a) a = b; }

template<typename T>
void setMin(T &a, T b)
{ if (b < a) a = b; }

class Vec
{
public:
  Vec() : x(0), y(0), z(0) {}
  Vec(ft x, ft y, ft z) : x(x), y(y), z(z) {}
  ft norm() const { return sqrt(x * x + y * y + z * z); }
  ft dot(const Vec &r) const { return x * r.x + y * r.y + z * r.z; }
  Vec cross(const Vec &r) const { return Vec(y*r.z-z*r.y, z*r.x-x*r.z, x*r.y-y*r.x); }
  Vec operator-(const Vec &o) const { return Vec(x - o.x, y - o.y, z - o.z); }
  Vec operator*(ft o) const { return Vec(x * o, y * o, z * o); }
  Vec unit() const { return *this * (1 / norm()); }
  ft x, y, z;
};

class Triangle;
class Vertex;
ft getEdgeCost(Vertex *u, Vertex *v);

class Vertex
{
public:
  Vertex(ft x, ft y, ft z) : pos(x, y, z) {}
  ft cost;
  Vertex *peer;
  Vec pos;
  set<Triangle *> adjF;
  set<Vertex *> adjV;
  bool operator<(const Vertex &rhs) {
    return cost < rhs.cost;
  }
  Vec operator-(const Vertex &o) const { return Vec(pos.x - o.pos.x, pos.y - o.pos.y, pos.z - o.pos.z); }
  void updateCost() {
    peer = nullptr;
    cost = -1;
    for (auto u: adjV) {
      ft t = getEdgeCost(this, u);
      if (! peer || t < cost) {
        cost = t;
        peer = u;
      }
    }
  }
};

class Triangle
{
public:
  Triangle(Vertex *p, Vertex *q, Vertex *r) : a{p, q, r}, normal((*q-*p).cross(*r-*p).unit()) {}
  Vertex *operator[](int i) { return a[i]; }
  bool has(Vertex *v) { return a[0] == v || a[1] == v || a[2] == v; }
  void replace(Vertex *u, Vertex *v) {
    if (a[0] == u) a[0] = v;
    else if (a[1] == u) a[1] = v;
    else if (a[2] == u) a[2] = v;
  }
  Vertex *a[3];
  Vec normal;
};

template<typename T>
class Heap
{
public:
  Heap(const vector<T> &a) : a(a) {
    REP(i, a.size())
      lkp[a[i]] = i;
    ROF(i, 0, a.size() / 2)
      down(i);
  }
  void up(int x) {
    T key = a[x];
    for (; x && *key < *a[(x-1)/2]; x = (x-1) / 2)
      lkp[a[x] = a[(x-1)/2]] = x;
    lkp[a[x] = key] = x;
  }
  void down(int x) {
    T key = a[x];
    for (int y; y = 2*x+1, y < a.size(); x = y) {
      if (y + 1 < a.size() && *a[y+1] < *a[y])
        y++;
      if (! (*a[y] < *key)) break;
      lkp[a[x] = a[y]] = x;
    }
    lkp[a[x] = key] = x;
  }
  bool empty() const { return a.empty(); }
  T pop() {
    T key = a[0];
    lkp.erase(key);
    a[0] = a.back();
    a.pop_back();
    down(0);
    return key;
  }
  void update(T key) {
    int x = lkp[key];
    up(x);
    down(x);
  }
  vector<T> a;
  map<T, int> lkp;
};

ft getEdgeCost(Vertex *u, Vertex *v)
{
  vector<Triangle *> fs;
  for (auto f: u->adjF)
    if (f->has(v))
      fs.push_back(f);

  ft curvature = 0;
  for (auto f: u->adjF) {
    ft curv = -1;
    for (auto ff: fs)
      setMax(curv, f->normal.dot(ff->normal));
    setMax(curvature, 1 - curv);
  }
  return (*u - *v).norm() * curvature;
}

int collapse(Heap<Vertex *> &heap, Vertex *u, Vertex *v)
{
  if (v == nullptr) return 0;

  int nremove = 0;
  for (auto it = begin(u->adjF); it != end(u->adjF); )
    if ((*it)->has(v)) {
      REP(i, 3)
        if ((*it)->a[i] != u)
          (*it)->a[i]->adjF.erase(*it);
      nremove++;
      it = u->adjF.erase(it);
    } else {
      (*it)->replace(u, v);
      v->adjF.insert(*it);
      ++it;
    }

  for (auto w: u->adjV) {
    w->adjV.erase(u);
    w->updateCost();
    heap.update(w);
    if (v != w) {
      v->adjV.insert(w);
      w->adjV.insert(v);
    }
  }

  u->adjV.clear();
  u->adjF.clear();
  return nremove;
}

void simplifyMesh(vector<Vertex *> vs, int nf, double percentage)
{
  ft nf2 = nf * percentage;
  for (auto v: vs)
    v->updateCost();

  Heap<Vertex *> heap(vs);
  while (nf > nf2 && ! heap.empty()) {
    Vertex *v = heap.pop(), *u = v->peer;
    ft t = v->cost;
    nf -= collapse(heap, v, u);
  }

  map<Vertex *, int> m;
  int id = 0;
  for (auto v: heap.a) {
    printf("v %lf %lf %lf\n", v->pos.x, v->pos.y, v->pos.z);
    m[v] = ++id;
  }
  for (auto v: heap.a)
    for (auto f: v->adjF) {
      printf("f %d %d %d\n", m[(*f)[0]], m[(*f)[1]], m[(*f)[2]]);
      REP(i, 3)
        if (f->a[i] != v)
          f->a[i]->adjF.erase(f);
    }
}

int main(int argc, char *argv[])
{
  if (argc != 4) return 1;
  freopen(argv[1], "r", stdin);
  freopen(argv[2], "w", stdout);

  vector<Vertex *> vs;
  vector<Triangle *> fs;
  int nf = 0;
  set<pair<Vertex *, Vertex *>> es;

  char s[256];
  while (scanf("%s", s) == 1)
  switch (s[0]) {
  case '#': fgets(s, sizeof s, stdin); break;
  case 'v': {
    double x, y, z;
    scanf("%lf%lf%lf", &x, &y, &z);
    vs.push_back(new Vertex(x, y, z));
    break;
  }
  case 'f': {
    int a, b, c;
    scanf("%d%d%d", &a, &b, &c);
    Vertex *p = vs[a-1], *q = vs[b-1], *r = vs[c-1];
    auto tri = new Triangle(p, q, r);
    fs.push_back(tri);
    p->adjF.insert(tri);
    q->adjF.insert(tri);
    r->adjF.insert(tri);
    auto go = [](Vertex *p, Vertex *q) {
      p->adjV.insert(q);
    };
    go(p, q); go(q, p);
    go(q, r); go(r, q);
    go(r, p); go(p, r);
    nf++;
    break;
  }
  }

  simplifyMesh(vs, nf, atof(argv[3]));

  for (auto &f: fs)
    delete f;
  for (auto &v: vs)
    delete v;
}
