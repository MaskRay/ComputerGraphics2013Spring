#pragma once

#include "Renderer.hh"
#include <opencv2/opencv.hpp>

class CVRenderer : public Renderer
{
public:
  CVRenderer() {}
  void render() override;
  int width, height;
};
