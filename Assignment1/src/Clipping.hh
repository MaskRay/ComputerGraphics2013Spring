#ifndef CLIPPING_HH
#define CLIPPING_HH

#include <functional>
#include <tuple>
using std::function;
using std::tuple;
using std::vector;
#include "Vector.hh"

namespace LineClipping
{
  void cohenSutherland(real_t xmin, real_t xmax, real_t ymin, real_t ymax, Vector2<real_t> a, Vector2<real_t> b, function<void(Vector2<real_t>, Vector2<real_t>)> plotLine);
  void liangBarsky(real_t xmin, real_t xmax, real_t ymin, real_t ymax, Vector2<real_t> a, Vector2<real_t> b, function<void(Vector2<real_t>, Vector2<real_t>)> plotLine);
  void midpointSubdivision(real_t xmin, real_t xmax, real_t ymin, real_t ymax, Vector2<real_t> a, Vector2<real_t> b, function<void(Vector2<real_t>, Vector2<real_t>)> plotLine);
};

namespace PolygonClipping
{
  vector<Vector2<real_t>> sutherlandHodgman(int n, const Vector2<real_t> subject[], int m, const Vector2<real_t> clip[]);
};

#endif
