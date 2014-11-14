#ifndef MOG_H
#define MOG_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>

class MOG {
public:
    MOG();
    void train(std::vector<cv::Mat>& training_images , std::vector<cv::Mat> &masks);
    void test(std::vector<cv::Mat>& test_images);
    void test2(std::vector<cv::Mat>& test_images);
    cv::EM* pos_model;
    cv::EM* neg_model;
    std::vector<cv::Mat> test_result;
    ~MOG();
private:
    void organizeSamples(std::vector<cv::Mat> &imgs,
                          std::vector<cv::Mat> &masks,
                          cv::Mat& pos_samples,
                          cv::Mat& neg_samples );
    void organizeSample (const cv::Mat& img,
                          const cv::Mat& mask,
                          std::vector<double> &pos_samples,
                          std::vector<double> &neg_samples );
    void test_image(cv::Mat& img, cv::Mat& dst, cv::EM &model);
    void test_image2(cv::Mat& img, cv::Mat& dst);
};

#endif // MOG_H
