#pragma once

#include "common.hh"

class Scene;

class Light
{
public:
  Light(Vec O, Color intensity) : O(O), intensity(intensity) {}
  ft getShade(Scene *scene, Vec IP) const;
  Vec O;
  Color intensity;
};
