#pragma once

#include "common.hh"
#include "Color.hh"
#include "material/Material.hh"
#include <opencv2/opencv.hpp>

class Texture
{
public:
  virtual ~Texture() {}
  virtual Material getMaterial(ft, ft) const = 0;
};

class SolidColorTexture : public Texture
{
public:
  SolidColorTexture(Material m) : m(m) {}
  Material getMaterial(ft, ft) const override { return m; }
  Material m;
};

class ChequerboardTexture : public Texture
{
public:
  ChequerboardTexture(Material m0, Material m1) : m0(m0), m1(m1) {}
  Material getMaterial(ft x, ft y) const override;
protected:
  Material m0, m1;
};

class ImageTexture : public Texture
{
public:
  ImageTexture(Material m, const char *filename) : m(m), img(cv::imread(filename, CV_LOAD_IMAGE_COLOR)) {}
  Material getMaterial(ft, ft) const override;
protected:
  cv::Mat img;
  Material m;
};
