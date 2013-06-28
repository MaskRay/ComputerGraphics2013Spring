#include "prim.hh"
#include "Trace.hh"

Material Trace::material() const
{
  Vec2 o = uv();
  return prim->texture->getMaterial(o.x, o.y);
}
