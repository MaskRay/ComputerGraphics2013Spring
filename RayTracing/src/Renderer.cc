#include "Renderer.hh"

Color Renderer::renderRay(Vec screenPos)
{
  return scene->trace(Ray::shift(camera, screenPos - camera));
}
