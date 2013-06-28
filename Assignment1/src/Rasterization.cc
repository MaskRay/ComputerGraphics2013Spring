#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iterator>
#include <map>
#include <stack>
#include <vector>
using namespace std;
#include "Rasterization.hh"

// 線段光柵化
void Context::plotLine(Vector2<int> a, Vector2<int> b)
{
  int x = a.x, y = a.y;
  int dx = abs(b.x-a.x), sx = a.x < b.x ? 1 : -1,
      dy = abs(b.y-a.y), sy = a.y < b.y ? 1 : -1,
      err = dx-dy;
  for(;;) {
    setPixel(x, y);
    int e = 2*err;
    if (e >= - dy) {
      if (x == b.x) break;
      err -= dy;
      x += sx;
    }
    if (e <= dx) {
      if (y == b.y) break;
      err += dx;
      y += sy;
    }
  }
}

// 圓光柵化
void Context::plotCircle(int cx, int cy, int r)
{
  int x = - r, y = 0, err = 2 - 2 * r;
  do {
    // 利用對稱性只計算八分之一
    setPixel(cx + x, cy + y);
    setPixel(cx - y, cy + x);
    setPixel(cx - x, cy - y);
    setPixel(cx + y, cy - x);
    setPixel(cx + y, cy + x);
    setPixel(cx - x, cy + y);
    setPixel(cx - y, cy - x);
    setPixel(cx + x, cy - y);
    int e = err;
    if (e <= y) err += ++y * 2 + 1;
    if (e > x || ! (err <= y)) err += ++x * 2 + 1;
  } while (- x >= y);
}

struct Segment
{
  int x, xx, dx, dy, yend;
  bool operator<(const Segment& rhs) const {
    if (x != rhs.x) return x < rhs.x;
    long long t = (long long)xx * rhs.dy - (long long)dy * rhs.xx;
    if (t != 0) return t < 0;
    return (long long)dx * rhs.dy - (long long)dy * rhs.dx < 0;
  }
};

void Context::plotPolygon(int n, const Vector2<int> a[])
{
  for (int i = 0; i < n; i++)
    plotLine(a[i], a[(i+1) % n]);
}

void Context::plotPolygon(int n, const Vector2<real_t> a[])
{
  for (int i = 0; i < n; i++)
    plotLine(a[i], a[(i+1) % n]);
}

void Context::fillPolygon(int n, const Vector2<int> a[])
{
  int miny = INT_MAX, maxy = INT_MIN;
  // rows[y] 儲存縱座標y處活躍的邊集
  multimap<int, Segment> rows;
  for (int i = 0; i < n; i++) {
    Vector2<int> p = a[i], q = a[(i+1) % n];
    if (p.y > q.y) swap(p, q);
    miny = min(miny, p.y);
    maxy = max(maxy, q.y);

    // 把邊插入rows
    Segment s;
    s.yend = q.y;
    s.dx = q.x - p.x;
    s.dy = q.y - p.y;
    s.x = p.x;
    s.xx = 0;
    rows.insert(make_pair(p.y, s));
    // 對於水平邊，交換端點後插入
    if (! s.dy) {
      s.x = q.x;
      rows.insert(make_pair(p.y, s));
    }
  }

  vector<Segment> active;
  for (int y = miny; y < maxy; y++) {
    // 從活躍邊表中刪除失效的邊
    active.erase(remove_if(active.begin(), active.end(), [&](Segment s) { return s.yend <= y; }),
                 active.end());

    // 把新活躍的邊排序
    auto segs_range = rows.equal_range(y);
    vector<Segment> segs;
    for (auto i = segs_range.first; i != segs_range.second; ++i)
      segs.push_back(i->second);
    sort(segs.begin(), segs.end());

    // 插入活躍邊表
    vector<Segment> active2;
    merge(active.begin(), active.end(), segs.begin(), segs.end(), back_inserter(active2));
    active.swap(active2);

    for (size_t i = 0; i+1 < active.size(); i += 2)
      for (int x = active[i].x + (active[i].dx >= 0); x < active[i+1].x + (active[i+1].xx > 0); x++)
        setPixel(x, y);

    // 對於下一y座標，更新活躍邊表中各邊的和水平線的交點
    for (auto &s: active)
      if (s.dy) {
        s.x += s.dx / s.dy;
        s.xx += s.dx % s.dy;
        if (s.xx >= s.dy) {
          s.x++;
          s.xx -= s.dy;
        }
        if (s.xx <= - s.dy) {
          s.x--;
          s.xx += s.dy;
        }
      }
  }
}

void Context::floodFill(int x, int y)
{
  stack<Vector2<int>> st;
  st.push(Vector2<int>{x, y});
  setPixel(x, y);
  while (! st.empty()) {
    Vector2<int> p = st.top();
    st.pop();
    int dx[] = {0,1,0,-1}, dy[] = {1,0,-1,0};
    for (int d = 0; d < 4; d++) {
      int xx = p.x + dx[d], yy = p.y + dy[d];
      if (inBound(xx, yy) && getPixel(xx, yy) != color) {
        setPixel(xx, yy);
        st.push(Vector2<int>{xx, yy});
      }
    }
  }
}

void Context::scanlineFill(int x, int y)
{
  stack<Vector2<int>> st;

  st.push(Vector2<int>{x, y});
  while (! st.empty()) {
    Vector2<int> p = st.top();
    st.pop();
    setPixel(p.x, p.y);

    // 左邊界
    int xl = p.x;
    for (; xl > 0 && getPixel(xl-1, p.y) != color; )
      setPixel(--xl, p.y);

    // 右邊界
    int xr = p.x;
    for (; xr+1 < width && getPixel(xr+1, p.y) != color; )
      setPixel(++xr, p.y);

    for (int dy = -1; dy <= 1; dy += 2)
      if (inBound(0, p.y + dy))
        for (int x = xl; x <= xr; x++) {
          bool need = false;
          for (; x <= xr && getPixel(x, p.y+dy) != color; x++)
            need = true;
          if (need)
            st.push(Vector2<int>{x-1, p.y+dy});
        }
  }
}
