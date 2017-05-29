#include "ThinglPointcloudViewer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

void ThinglPointcloudViewer::showPointcloud(const cv::Mat_<cv::Vec3b>& colorImage,
                    const cv::Mat_<unsigned short>& depthImage,
                    const cv::Mat& K) {

    float fxi,fyi,cxi,cyi;
    cv::Mat_<float> Kinv = K.inv();
    fxi = Kinv(0,0);
    fyi = Kinv(1,1);
    cxi = Kinv(0,2);
    cyi = Kinv(1,2);

    CloudVertex* cloudBuffer = new CloudVertex[colorImage.rows*colorImage.cols];
    unsigned short depth;
    int points = 0;
    for (int row=0;row<colorImage.rows;row++) { //height
        for (int col=0;col<colorImage.cols;col++) { //width
            if ( depthImage(row,col) <= 0 )
                continue;
            points++;

            depth = depthImage(row, col);
            cloudBuffer[points].point[0] = (col*fxi + cxi) * depth;
            cloudBuffer[points].point[1] = (row*fyi + cyi) * depth;
            cloudBuffer[points].point[2] = depth;

            cloudBuffer[points].color[3] = 100;
            cloudBuffer[points].color[2] = colorImage(row,col)[2];
            cloudBuffer[points].color[1] = colorImage(row,col)[1];
            cloudBuffer[points].color[0] = colorImage(row,col)[0];

        } 
    } 
    int vertexBufferNumPoints = points;
    // create new ones, static

    glDeleteBuffers(1, &vertexBufferId);

    vertexBufferId=0;
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);         // for vertex coordinates
    glBufferData(GL_ARRAY_BUFFER, sizeof(CloudVertex) * points, cloudBuffer, GL_STATIC_DRAW);

    delete cloudBuffer;

    while(1==1 ){
        glClear(GL_COLOR_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

        glVertexPointer(3, GL_FLOAT, sizeof(CloudVertex), 0);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(CloudVertex), (const void*) (3*sizeof(float)));

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glDrawArrays(GL_POINTS, 0, vertexBufferNumPoints);

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

