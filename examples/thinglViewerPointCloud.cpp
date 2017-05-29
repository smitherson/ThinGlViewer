// Include standard headers
#include <stdio.h>
#include "ThinglPointcloudViewer.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

int main( void )
{

    cv::Mat image = cv::imread("../test-data/rgb-1.png"); 
    cv::Mat depth = cv::imread("../test-data/depth-1.png", CV_LOAD_IMAGE_ANYDEPTH); 
    cv::Matx33f K(517.3, 0, 318.6, 0, 516.5, 255.3, 0, 0, 1);
    ThinglPointcloudViewer thinViewer;
    if( thinViewer.initWindow("demo", 1024, 768)) {
        thinViewer.showPointcloud(image,  depth, cv::Mat(K));

    }

    return 0;
}

