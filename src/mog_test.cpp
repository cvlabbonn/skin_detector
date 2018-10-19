#include <mog.h>

void MOG::test( std::vector<cv::Mat>& test_images ){
    test_result.clear();
    for (int i = 0; i < test_images.size(); i++){
        cv::Mat res;
        compute_SkinnBinMap_4_Image(test_images[i],res,0,false);
        //res.convertTo(res,CV_8UC1);
        cv::normalize(res, res, 0, 255, CV_MINMAX, CV_8UC1);
        test_result.push_back(res);
    }
}

void   MOG::compute_SkinnBinMap_4_Image( const cv::Mat &rgb, /*const cv::Mat &depth,*/ cv::Mat &result ,double threshold, bool skipInvalidDepthFLAG/*, bool printFLAG*/ )
{

        //PARAM_THRESH_Ratio = threshold;
        cv::Mat logRatio_map_RAWW;
        compute_LogRatioMap_4_Image( rgb, /*depth, */logRatio_map_RAWW/*, skipInvalidDepthFLAG*/);
        result = logRatio_map_RAWW.clone();
//        result = cv::Mat::zeros(logRatio_map_RAWW.rows,logRatio_map_RAWW.cols,CV_8UC1);
//        for     (int iii=0; iii<logRatio_map_RAWW.rows; iii++)
//        {   for (int jjj=0; jjj<logRatio_map_RAWW.cols; jjj++)
//            {
//                if (logRatio_map_RAWW.at<double>(iii,jjj) > PARAM_THRESH_Ratio)
//                {
//                    result.at<uchar>(iii,jjj) = 255;
//                }
//            }
//        }
}

void MOG::compute_LogRatioMap_4_Image( const cv::Mat &rgb, /*const cv::Mat &depth,*/ cv::Mat &result/*, bool skipInvalidDepthFLAG*/)
{

        result = cv::Mat::zeros(rgb.rows,rgb.cols,CV_64FC1);
        for     (int iii=0; iii<result.rows; iii++)
        {   for (int jjj=0; jjj<result.cols; jjj++)
            {

//                    if (skipInvalidDepthFLAG  &&  depth.at<unsigned short>(iii,jjj) == 0) // from YML file
//                    {
//                        result.at<double>(iii,jjj) = -999999; // so that is far below THRESH
//                        continue;
//                    }

                    result.at<double>(iii,jjj) = compute_LogRatioMap_4_Pixel(  rgb.at<cv::Vec3b>(iii,jjj) ); // LogRatio - will have useful negative values !!!
            }
        }

}

double MOG::compute_LogRatioMap_4_Pixel( const cv::Vec3b pixelBGR)
{
    double skinnProb_POS   = 0;
    double skinnProb_NEG   = 0;
    double skinnProb_RATIO = 0;

    for (int mmm=0; mmm< posMixtureModels.size(); mmm++) // page 10 @ paper
    {
        skinnProb_POS += posMixtureModels[mmm].computeProbability( pixelBGR );
        skinnProb_NEG += negMixtureModels[mmm].computeProbability( pixelBGR );
    }
    skinnProb_RATIO = log( skinnProb_POS / (1e-10+skinnProb_NEG) ); // tiny constant to avoid division by zero

    return skinnProb_RATIO;
}


void MOG::test2( std::vector<cv::Mat>& test_images ){
    test_result.clear();
    for (int i = 0; i < test_images.size(); i++){
        cv::Mat res_pos, res_neg, res;
        test_image(test_images[i], res_pos, pos_model);
        test_image(test_images[i], res_neg, neg_model);
//        res = res_pos / (res_pos + res_neg);
        cv::log(res_pos / (1e-10+res_neg), res);
        cv::normalize(res, res, 0, 255, CV_MINMAX, CV_8UC1);
        test_result.push_back(res);
    }
}

void MOG::test_image(cv::Mat &img, cv::Mat &dst, cv::Ptr<cv::ml::EM> model){
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
      dst.at<double>(yyy,xxx) = std::exp(model->predict( sample ));
    }
  }
}
