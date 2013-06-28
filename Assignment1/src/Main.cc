#include <opencv2/opencv.hpp>
#include <tuple>
using std::get;
#include "Rasterization.hh"
#include "Clipping.hh"

class DemoContext : public Context
{
public:
  DemoContext() {
    width = 400;
    height = 400;
    setColor(255, 255, 255);
    img.create(height, width, CV_8UC3);
    img.setTo(cv::Scalar(0, 0, 0));
  }
  void setPixel(int x, int y) {
    img.ptr<uchar>(y)[x*3] = color % 256;
    img.ptr<uchar>(y)[x*3+1] = color / 256 % 256;
    img.ptr<uchar>(y)[x*3+2] = color / 256 / 256 % 256;
  }
  int getPixel(int x, int y) {
    if (0 <= x && x < width && 0 <= y && y < height)
      return (img.ptr<uchar>(y)[x*3+2] * 256 + img.ptr<uchar>(y)[x*3+1]) * 256
        + img.ptr<uchar>(y)[x*3];
    return -1;
  }
  // 使用構造析構函數提供每幀暫停功能
  struct Frame {
    Frame(DemoContext *c) : img(c->img) { img.setTo(cv::Scalar(0, 0, 0)); }
    ~Frame() { cv::imshow("show", img); cv::waitKey(0); }
    cv::Mat &img;
  };
  void show() {
    {
      Frame f(this);
      Vector2<int> ps[] = {{100, 20}, {200, 30}, {210, 100}, {304, 100},
        {300, 16}, {390, 23}, {390, 200}, {100, 200}};
      plotPolygon(8, ps);
      floodFill(160, 60);
    }

    {
      Frame f(this);
      Vector2<int> ps[] = {{100, 20}, {200, 30}, {210, 100}, {304, 100},
        {300, 16}, {390, 23}, {390, 200}, {100, 200}};
      plotPolygon(8, ps);
      scanlineFill(160, 60);
    }

    void (*clips[])(real_t, real_t, real_t, real_t, Vector2<real_t>, Vector2<real_t>, function<void(Vector2<real_t>, Vector2<real_t>)>) = {
      LineClipping::cohenSutherland, LineClipping::liangBarsky, LineClipping::midpointSubdivision
    };
    for (auto c : clips) {
      Frame f(this);
      Vector2<int> ps[] = {{50, 50}, {350, 50}, {350, 350}, {50, 350}};
      setColor(255, 255, 255);
      plotPolygon(4, ps);

      auto clip = [&](Vector2<real_t> a, Vector2<real_t> b) {
        c(50, 350, 50, 350, a, b, [&](Vector2<real_t> a, Vector2<real_t>b) {
          plotLine(Vector2<int>(a), Vector2<int>(b));
          });
      };

      setColor(255, 0, 0);
      clip(Vector2<real_t>{30, 360}, Vector2<real_t>{80, 30});
      clip(Vector2<real_t>{40, 40}, Vector2<real_t>{360, 370});
      clip(Vector2<real_t>{170, 40}, Vector2<real_t>{10, 360});
      clip(Vector2<real_t>{370, 40}, Vector2<real_t>{43, 380});
      clip(Vector2<real_t>{370, 40}, Vector2<real_t>{23, 300});
      clip(Vector2<real_t>{177, 40}, Vector2<real_t>{380, 144});
    }

    {
      Frame f(this);
      plotCircle(100, 300, 60);
      plotCircle(120, 130, 40);
      plotCircle(320, 250, 34);
    }

    {
      Frame f(this);
      Vector2<real_t> clip[] =  {{100, 100}, {300, 100}, {300, 300}, {100, 300}};
      Vector2<real_t> subject[] = {{50, 150}, {200, 50}, {350, 150}, {350, 300},
          {250, 300}, {200, 250}, {150, 350}, {100, 250},
          {100, 200}};
      auto out = PolygonClipping::sutherlandHodgman(9, subject, 4, clip);

      vector<Vector2<int>> out2;
      for (auto i : clip)
        out2.push_back(i);
      setColor(0, 255, 0);
      plotPolygon(4, &out2[0]);
      out2.clear();

      for (auto i : out)
        out2.push_back(i);
      setColor(255, 0, 0);
      plotPolygon(out.size(), &out2[0]);
    }
  }
protected:
  cv::Mat img;
};

int main()
{
  DemoContext demo;
  demo.show();
}
