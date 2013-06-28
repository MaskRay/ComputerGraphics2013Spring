#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
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
#define SET(a, v) memset(a, v, sizeof a)
#define CPY(a, b) memcpy(b, a, sizeof b)

typedef double ft;
const double eps = 1e-9;

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
  ft operator[](int i) const { return (&x)[i]; }
  ft norm() const { return sqrt(x * x + y * y + z * z); }
  ft dot(const Vec &r) const { return x * r.x + y * r.y + z * r.z; }
  Vec cross(const Vec &r) const { return Vec(y*r.z-z*r.y, z*r.x-x*r.z, x*r.y-y*r.x); }
  Vec operator+(const Vec &o) const { return Vec(x + o.x, y + o.y, z + o.z); }
  Vec operator-(const Vec &o) const { return Vec(x - o.x, y - o.y, z - o.z); }
  Vec operator*(ft o) const { return Vec(x * o, y * o, z * o); }
  Vec unit() const { return *this * (1 / norm()); }
  ft x, y, z;
};

class Triangle;
class Vertex;

class Quad
{
public:
  Quad() { SET(a, 0); }
  void operator+=(const Quad &o) {
    REP(i, 4)
      REP(j, 4)
        a[i][j] += o.a[i][j];
  }
  ft a[4][4];
};

void join(const Quad &q0, const Quad &q1, ft a[4][5])
{
  REP(i, 4)
    REP(j, 4)
      a[i][j] = q0.a[i][j] + q1.a[i][j];
}

Vec gauss(ft a[4][5], const Vec &p0, const Vec &p1)
{
  a[0][4] = a[1][4] = a[2][4] = a[3][0] = a[3][1] = a[3][2] = 0;
  a[3][3] = a[3][4] = 1;
  REP(i, 4) {
    int p = i;
    FOR(j, i+1, 4)
      if (fabs(a[j][i]) > fabs(a[p][i]))
        p = j;
    if (i != p)
      swap_ranges(a[i], a[i] + 5, a[p]);
    if (fabs(a[i][i]) < eps)
      return (p0 + p1) * 0.5;
    ft t = a[i][i];
    FOR(j, i, 5)
      a[i][j] /= t;
    FOR(j, i+1, 4) {
      t = a[j][i];
      FOR(k, i, 5)
        a[j][k] -= a[i][k] * t;
    }
  }
  ROF(i, 0, 4)
    FOR(j, 0, i)
      a[j][4] -= a[j][i] * a[i][4];
  return Vec{a[0][4], a[1][4], a[2][4]};
}

class Vertex
{
public:
  Vertex(ft x, ft y, ft z) : pos(x, y, z) {}
  ft cost;
  Vertex *peer;
  Vec pos, peerPos;
  Quad q;
  set<Triangle *> adjF;
  set<Vertex *> adjV;
  bool operator<(const Vertex &rhs) {
    return cost < rhs.cost;
  }
  Vec operator-(const Vertex &o) const { return Vec(pos.x - o.pos.x, pos.y - o.pos.y, pos.z - o.pos.z); }
  void updateQ(); 
  void updateCost() {
    ft a[4][5], b[4][5];
    peer = nullptr;
    cost = -1;
    for (auto u: adjV) {
      join(q, u->q, a);
      memcpy(b, a, sizeof a);
      Vec W = gauss(a, pos, u->pos);
      ft t = 0;
      REP(i, 4)
        REP(j, 4)
          t += (i < 3 ? W[i] : 1) * b[i][j] * (j < 3 ? W[j] : 1);
      if (! peer || t < cost) {
        cost = t;
        peer = u;
        peerPos = W;
      }
    }
  }
};

class Triangle
{
public:
  Triangle(Vertex *p, Vertex *q, Vertex *r) : a{p, q, r} {
    calcNormal();
  }
  Vertex *operator[](int i) { return a[i]; }
  void calcNormal() {
    normal = (*a[1]-*a[0]).cross(*a[2]-*a[0]).unit();
  }
  bool has(Vertex *v) { return a[0] == v || a[1] == v || a[2] == v; }
  void replace(Vertex *u, Vertex *v) {
    if (a[0] == u) a[0] = v;
    else if (a[1] == u) a[1] = v;
    else if (a[2] == u) a[2] = v;
  }
  Vertex *a[3];
  Vec normal;
};

void Vertex::updateQ()
{
  SET(q.a, 0);
  for (auto f: adjF) {
    ft d = - pos.dot(f->normal);
    REP(i, 4)
      REP(j, 4) {
        ft t = (i < 3 ? f->normal[i] : d) * (j < 3 ? f->normal[j] : d);
        q.a[i][j] += t;
      }
  }
}

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

  // adjV
  for (auto w: u->adjV) {
    w->adjV.erase(u);
    if (v != w) {
      v->adjV.insert(w);
      w->adjV.insert(v);
    }
  }

  // position
  v->pos = u->peerPos;

  // normal
  for (auto f: v->adjF)
    f->calcNormal();

  // Q
  v->updateQ();
  for (auto w: v->adjV)
    w->updateQ();

  // cost
  v->updateCost();
  heap.update(v);
  for (auto w: v->adjV) {
    w->updateCost();
    heap.update(w);
  }

  u->adjV.clear();
  u->adjF.clear();
  return nremove;
}

void simplifyMesh(vector<Vertex *> vs, int nf, double percentage)
{
  ft nf2 = nf * percentage;
  for (auto v: vs)
    v->updateQ();
  for (auto v: vs)
    v->updateCost();

  Heap<Vertex *> heap(vs);
  while (nf > nf2 && ! heap.empty()) {
    Vertex *v = heap.pop(), *u = v->peer;
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
      if (! (m.count((*f)[0]) && m.count((*f)[1]) && m.count((*f)[2])))
        fputs("---\n", stderr), exit(0);
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
