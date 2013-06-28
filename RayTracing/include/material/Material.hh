#pragma once

class Material
{
public:
  Material(Color diffuse, ft ambient, ft specular, ft reflect, ft refract, ft shine, ft refractIdx)
    : diffuse(diffuse), ambient(ambient), specular(specular), reflect(reflect), refract(refract),
    shine(shine), refractIdx(refractIdx) {}
  Color diffuse;
  ft ambient, specular, reflect, refract, shine, refractIdx;
  Color emission;
};
