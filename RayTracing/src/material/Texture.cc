#include "material/Texture.hh"

Material ChequerboardTexture::getMaterial(ft u, ft v) const
{
  return (u < 0.5 ? 0 : 1) ^ (v < 0.5 ? 0 : 1) ? m1 : m0;
}

Material ImageTexture::getMaterial(ft u, ft v) const
{
  int x = max(min(int(u * img.cols), img.cols - 1), 0);
  int y = max(min(int(v * img.rows), img.rows - 1), 0);
  Material ret = m;
  x = (x + img.cols / 2) % img.cols;
  y = (y + img.rows / 2) % img.rows;
  cv::Vec3b pixel = img.at<cv::Vec3b>(y, x);
  ret.diffuse = Color(pixel[2] / 256.0, pixel[1] / 256.0, pixel[0] / 255.0);
  return ret;
}
