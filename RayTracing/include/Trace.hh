#pragma once

#include "common.hh"

class Prim;

class Trace
{
public:
  virtual Vec2 uv() const = 0;
  virtual Material material() const;
  double dist;
  Vec IP, N;
  Prim *prim;
  bool inside;
};
