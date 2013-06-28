#include "Config.hh"
#include "renderer/CVRenderer.hh"

void CVRenderer::render()
{
  cv::Mat img;
  img.create(height, width, CV_8UC3);
  img.setTo(cv::Scalar(0, 0, 0));

#pragma omp parallel for schedule(static) num_threads(cfg.nthreads)
  REP(y, height) {
    REP(x, width) {
      Color c;
      Vec dx = ft(1) / width * screenD1, dy = ft(1) / height * screenD2;
      if (cfg.mode[0] == 'r') {
        Vec screenPos = screenOrig + ft(x) * dx + ft(y) * dy;
        c = renderRay(screenPos);
        REP(i, cfg.samples - 1) {
          Vec pos = screenPos + rand() / ft(RAND_MAX) * dx + rand() / ft(RAND_MAX) * dy;
          c += renderRay(pos);
        }
      } else {
        REP(i, cfg.samples) {
          Vec screenPos = screenOrig + ft(x) * dx + ft(y) * dy;
          ft r0 = 2 * drand48(), r1 = 2 * drand48();
          r0 = r0 < 0 ? sqrt(r0) - 0 : 0 - sqrt(2-r0);
          r1 = r1 < 1 ? sqrt(r1) - 1 : 1 - sqrt(2-r1);
          c += scene->pathTracing(Ray::shift(camera, screenPos - camera), 0);
        }
      }
      c *= ft(1) / SAMPLES;
      img.ptr<uchar>(height-1-y)[x*3+2] = int(round(pow(clamp(c.r), cfg.gammaR) * 255));
      img.ptr<uchar>(height-1-y)[x*3+1] = int(round(pow(clamp(c.g), cfg.gammaG) * 255));
      img.ptr<uchar>(height-1-y)[x*3+0] = int(round(pow(clamp(c.b), cfg.gammaB) * 255));
    }
    printf("%d..", y);
  }

  if (cfg.outfile)
    cv::imwrite(cfg.outfile, img);
  else {
    cv::imshow("show", img);
    cv::waitKey(0);
  }
}
