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
};

#endif // MOG_H
