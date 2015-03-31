#ifndef MOG_H
#define MOG_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <mixture.h>
#include <QSettings>

class MOG {
public:
    MOG();
    void train(std::vector<cv::Mat>& training_images , std::vector<cv::Mat> &masks);
    void test(std::vector<cv::Mat>& test_images);
    void test2(std::vector<cv::Mat>& test_images);
    cv::EM* pos_model;
    cv::EM* neg_model;
    std::vector<cv::Mat> test_result;
    std::vector<Mixture> posMixtureModels;
    std::vector<Mixture> negMixtureModels;
    void save(std::string filename);
    bool load(std::string filename);
    ~MOG();
    void emToMixture();
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
    void compute_SkinnBinMap_4_Image( const cv::Mat &rgb, /*const cv::Mat &depth,*/ cv::Mat &result ,double threshold, bool skipInvalidDepthFLAG/*, bool printFLAG*/ );
    void compute_LogRatioMap_4_Image( const cv::Mat &rgb, /*const cv::Mat &depth,*/ cv::Mat &result/*, bool skipInvalidDepthFLAG*/);
    double compute_LogRatioMap_4_Pixel( const cv::Vec3b pixelBGR);

};

#endif // MOG_H
