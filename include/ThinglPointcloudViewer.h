#pragma once 
#include <ThinglViewer.h>
#include <opencv2/core/core.hpp>

struct CloudVertex {
  float point[3];
  uchar color[4];
};

struct CloudVertexNoColor {
  float point[3];
};

class ThinglPointcloudViewer : public ThinglViewer {
    public:
        void showPointcloud(const cv::Mat_<cv::Vec3b>& colorImage, const cv::Mat_<unsigned short>& depthImage,const cv::Mat& K);
    private:
        GLuint vertexBufferId;
};
