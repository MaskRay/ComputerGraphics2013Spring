#include "common.hh"
#include "Config.hh"
#include "geometry/sphere.hh"
#include "geometry/plane.hh"
#include "geometry/triangle.hh"
#include "prim.hh"
#include "material/Texture.hh"
#include "renderer/CVRenderer.hh"
#include "ObjReader.hh"

int main(int argc, char *argv[])
{
  cfg.getopt(argc, argv);
  ft ambient = 0.1, specular = 0.4, reflect = 0.8, refract = 0.8, shine = 10, refractIdx = 1.2;
  Material m0(BLUE, ambient, specular, reflect, refract, shine, refractIdx);
  Material m1(RED, ambient, specular, reflect, refract, shine, refractIdx);

  Scene scene;
  scene.density = 0.1;

  ft r = 0.6;

  //REP(i, 8) {
    //auto l = new Light(Vec(20, 20, 20), Color(1, 1, 1));
    //if (i & 1) l->O.x *= -1;
    //if (i & 2) l->O.y *= -1;
    //if (i & 4) l->O.z *= -1;
    //scene.addLight(l);
  //}
  //auto l = new Light(Vec(-3, -3, 20), Color(1, 1, 1));
  //scene.addLight(l);
  //auto ll = new Light(Vec(3, 30, 1), Color(1, 1, 1));
  //scene.addLight(ll);

  //SolidColorTexture tA(Material(Color(176.0/256, 196.0/256, 222.0/256), ambient, 0.5, reflect, refract, 20, r));
  //REP(i, 11)
    //REP(j, 11) {
      //auto A = new Sphere(Vec(i, j, -0.6), 0.4);
      //auto pA = new Prim(A, &tA);
      //scene.addPrim(pA);
    //}

  //Sphere B(Vec(0, -1.3, 0), 1);
  ////SolidColorTexture tB(Material(Color(196.0/256, 48.0/256, 256.0/256), ambient, 0.6, reflect, refract, 40, r));
  //ImageTexture tB(Material(Color(96.0/256, 128.0/256, 256.0/256), ambient, 0.3, 0, 0, 0, r), "/tmp/w.png");
  //Prim pB(&B, &tB);
  //scene.addPrim(&pB);

  //Sphere C(Vec(2.5, 0, -0.5), 0.5);
  //SolidColorTexture tC(Material(Color(148.0/256, 256.0/256, 222.0/256), ambient, 0.3, reflect, refract, 30, r));
  //Prim pC(&C, &tC);
  //scene.addPrim(&pC);

  //Sphere D(Vec(-1, 5, -0.2), 0.8);
  //SolidColorTexture tD(Material(Color(40.0/256, 128.0/256, 196.0/256), ambient, 0.3, reflect, refract, 40, r));
  //Prim pD(&D, &tD);
  //scene.addPrim(&pD);

  //Triangle E(Vec(0, 0, 0), Vec(3, 0, 0), Vec(0, 4, 3));
  //SolidColorTexture tE(Material(Color(40.0/256, 128.0/256, 196.0/256), ambient, 0.3, reflect, refract, 40, r));
  //Prim pE(&E, &tE);
  //scene.addPrim(&pE);

  //ObjReader reader("/tmp/a.obj");
  //shared_ptr<Mesh> F = reader.readMesh();
  //SolidColorTexture tF(Material(Color(255.0/256, 128.0/256, 255.0/256), ambient, 0.3, reflect, refract, 40, r));
  //Prim pF(F.get(), &tF);
  //scene.addPrim(&pF);
  //scene.complete();

  //Plane P(Vec(0, 0, -1), Vec(1, 0, 0), Vec(0, 1, 0));
  //ChequerboardTexture tP(Material(Color(192.0/256, 48.0/256, 224.0/256), ambient, 0.3, reflect, refract, 0, refractIdx),
                       //Material(Color(256.0/256, 256.0/256, 192.0/256), ambient, 0.3, reflect, refract, 0, r));
  //Prim pP(&P, &tP);
  //scene.addPrim(&pP);
  
  // path tracing
  Sphere left(Vec(1e5+1, 40.8, 81.6), 1e5);
  auto m = new SolidColorTexture(Material(Color(.75, .25, .25), ambient, 0, 1, 0, 0, r));
  scene.addPrim(new Prim(&left, m));

  Sphere right(Vec(-1e5+99, 40.8, 81.6), 1e5);
  m = new SolidColorTexture(Material(Color(.25,.25,.75), ambient, 0, 1, 0, 0, r));
  scene.addPrim(new Prim(&right, m));

  Sphere back(Vec(50, 40.8, 1e5), 1e5);
  m = new SolidColorTexture(Material(Color(.75,.75,.75), ambient, 0, 1, 0, 0, r));
  scene.addPrim(new Prim(&back, m));

  Sphere front(Vec(50, 40.8, -1e5+170), 1e5);
  m = new SolidColorTexture(Material(Color(.75,.75,.75), ambient, 0, 1, 0, 0, r));
  scene.addPrim(new Prim(&front, m));

  Sphere bottom(Vec(50, 1e5, 81.6), 1e5);
  m = new SolidColorTexture(Material(Color(.75,.75,.75), ambient, 0, 1, 0, 0, r));
  scene.addPrim(new Prim(&bottom, m));

  Sphere top(Vec(50, -1e5+81.6, 81.6), 1e5);
  m = new SolidColorTexture(Material(Color(.75,.75,.75), ambient, 0, 1, 0, 0, r));
  scene.addPrim(new Prim(&top, m));

  Sphere mirror(Vec(27, 16.5, 47), 16.5);
  m = new SolidColorTexture(Material(Color(1,1,1), ambient, 1, 0, 0, 0, r));
  scene.addPrim(new Prim(&mirror, m));

  Sphere glass(Vec(73, 16.5, 78), 16.5);
  m = new SolidColorTexture(Material(Color(1,1,1), ambient, 0.5, 0.5, 0, 0, r));
  scene.addPrim(new Prim(&glass, m));

  Sphere light(Vec(50, 681.6-.27, 81.6), 600);
  Material mm = Material(Color(0, 0, 0), ambient, 0, 1, 0, 0, r);
  mm.emission = Color(12, 12, 12);
  m = new SolidColorTexture(mm);
  scene.addPrim(new Prim(&light, m));

  scene.complete();

  CVRenderer cv;
  Vec at = Vec(0, -1, 0);
  Vec up = Vec(0, 0, 1);
  cv.scene = &scene;
  at = (at-cv.camera).unit() * 2 + cv.camera;
  up -= up.dot(at - cv.camera) * up;
  //cv.screenOrig = at;
  //cv.screenD2 = up.unit() * 6;
  //cv.screenD1 = (at - cv.camera).cross(cv.screenD2).unit() * 6;
  //cv.screenOrig -= cv.screenD1 / 2 + cv.screenD2 / 2;

  cv.width = 1024;
  cv.height = 768;
  cv.camera = Vec(50, 52, 295.6);
  cv.screenOrig = Vec(0, -0.042612, -1).unit() * 140 + cv.camera;
  cv.screenD1 = Vec(cv.width * 0.5135 / cv.height);
  cv.screenD2 = cv.screenD1.cross(cv.screenOrig-cv.camera).unit() * .5135;

  cv.render();
}
