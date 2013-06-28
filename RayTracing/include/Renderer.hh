#pragma once

#include "common.hh"
#include "Scene.hh"

class Renderer
{
public:
  Color renderRay(Vec screenPos);
  virtual void render() = 0;
  Scene *scene;
  Vec camera, screenOrig, screenD1, screenD2;
};
