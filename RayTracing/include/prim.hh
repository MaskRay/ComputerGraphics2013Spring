#pragma once

#include "common.hh"
#include "material/Texture.hh"
#include "Trace.hh"

class Geometry;

class Prim
{
public:
  Prim(Geometry *geo, Texture *texture) : geo(geo), texture(texture) {}
  virtual shared_ptr<Trace> getTrace(const Ray &, ft);
  Texture *texture;
  Geometry *geo;
};
