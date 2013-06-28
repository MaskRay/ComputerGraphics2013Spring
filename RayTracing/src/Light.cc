#include "Light.hh"
#include "Scene.hh"

ft Light::getShade(Scene *scene, Vec IP) const
{
  return scene->isOccluded(IP, O) ? 0 : 1;
}
