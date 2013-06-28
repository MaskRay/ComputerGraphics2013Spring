#pragma once

#include "common.hh"
#include "geometry/mesh.hh"

class ObjReader
{
public:
  ObjReader(const char *filename) : file(fopen(filename, "r")) {
    if (file == nullptr)
      throw "fail";
  }
  ~ObjReader() { fclose(file); }
  shared_ptr<Mesh> readMesh();
protected:
  FILE *file;
};
