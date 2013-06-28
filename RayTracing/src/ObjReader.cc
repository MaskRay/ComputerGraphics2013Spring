#include "ObjReader.hh"

shared_ptr<Mesh> ObjReader::readMesh()
{
  auto mesh = make_shared<Mesh>();

  char s[256];
  while (fscanf(file, "%s", s) == 1)
  switch (s[0]) {
  case 'v': {
    double x, y, z;
    fscanf(file, "%lf%lf%lf", &x, &y, &z);
    mesh->addVertex(Vec(x, y, z));
    break;
  }
  case 'f': {
    int a, b, c;
    fscanf(file, "%d%d%d", &a, &b, &c);
    mesh->addFace(a-1, b-1, c-1);
    break;
  }
  default: fgets(s, sizeof s, file); break;
  }

  mesh->complete();
  return mesh;
}
