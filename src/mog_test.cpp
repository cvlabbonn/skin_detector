#include <mog.h>

void MOG::test( std::vector<cv::Mat>& test_images ){
    test_result.clear();
    cv::Mat fakeSamples = cv::Mat::zeros(99, 1, CV_64F);;
    if (!pos_model->isTrained())
        pos_model->train( fakeSamples );
    if (!neg_model->isTrained())
        neg_model->train( fakeSamples );
    for (int i = 0; i < test_images.size(); i++){
        cv::Mat res_pos, res_neg, res;
        test_image(test_images[i], res_pos, *pos_model);
        test_image(test_images[i], res_neg, *neg_model);
//        res = res_pos / (res_pos + res_neg);
        cv::log(res_pos / (1e-10+res_neg), res);
        cv::normalize(res, res, 0, 255, CV_MINMAX, CV_8UC1);
        test_result.push_back(res);
    }
}

void MOG::test2( std::vector<cv::Mat>& test_images ){
    test_result.clear();
    for (int i = 0; i < test_images.size(); i++){
        cv::Mat res_pos, res_neg, res;
        test_image(test_images[i], res_pos, *pos_model);
        test_image(test_images[i], res_neg, *neg_model);
//        res = res_pos / (res_pos + res_neg);
        cv::log(res_pos / (1e-10+res_neg), res);
        cv::normalize(res, res, 0, 255, CV_MINMAX, CV_8UC1);
        test_result.push_back(res);
    }
}

void MOG::test_image(cv::Mat &img, cv::Mat &dst, cv::EM& model){
    dst.create( img.size(), CV_64FC1 );

    cv::Mat_<double> sample(1, img.channels(), CV_64FC1); // 1x3 mat

    for     (int yyy=0; yyy<img.rows; yyy++)
    {   for (int xxx=0; xxx<img.cols; xxx++)
        {
            // 3 channel pixel to 1x3 mat
            sample(0) = img.at<cv::Vec3b>(yyy,xxx)(0);
            sample(1) = img.at<cv::Vec3b>(yyy,xxx)(1);
            sample(2) = img.at<cv::Vec3b>(yyy,xxx)(2);

            // returns a 2-element double vector, we use just
            // the 1st element (likelihood logarithm value)
            dst.at<double>(yyy,xxx) = std::exp(model.predict( sample )[0]);
        }
    }
}
