#ifndef VIEWER_H
#define VIEWER_H

#include <iostream>
#include <OpenNI.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Viewer {
public:
    int limitx_min;
    int limitx_max;
    int limity_min;
    int limity_max;
    int limitz_min;
    int limitz_max;
    int frame_width;
    int frame_height;
    bool exit_flag;
    bool save_memory;

    Viewer();
    void initialize();
    int loop();
    std::vector<cv::Mat> rgb_images;
    std::vector<cv::Mat> depth_images;
    ~Viewer();
private:
    void error_manager( int error_type , bool critical=false);
    openni::Status rc;
    openni::Device device;
    openni::VideoStream depth, color;
    openni::VideoFrameRef depthFrame, colorFrame;
    cv::Mat frame, frameDepth, bgrMat;

    bool only_depth;
    void createRGBD(cv::Mat& depth_mat, cv::Mat& color_mat, cv::Mat& dst_rgbd, cv::Mat& dst_depth);
    void key_parse(char key);
};

#endif // VIEWER_H
