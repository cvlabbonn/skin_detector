#include <mog.h>


void MOG::train( std::vector<cv::Mat>& training_images, std::vector<cv::Mat>& masks ){
    cv::Mat pos_samples, neg_samples;
    const cv::TermCriteria criteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 10, FLT_EPSILON);
    cv::EM pos_model(cv::EM::DEFAULT_NCLUSTERS, cv::EM::COV_MAT_DIAGONAL, criteria);
    cv::EM neg_model(cv::EM::DEFAULT_NCLUSTERS, cv::EM::COV_MAT_DIAGONAL, criteria);

    // organize all the samples
    organizeSamples(training_images, masks, pos_samples, neg_samples);

    // train the models
    pos_model.train( pos_samples );
    neg_model.train( neg_samples );

}


void MOG::organizeSamples( std::vector<cv::Mat>& imgs,
                           std::vector<cv::Mat>& masks,
                           cv::Mat& pos_samples,
                           cv::Mat& neg_samples ){
    std::vector<double> pos, neg;

    const size_t channels = imgs[0].channels();
    for ( int i = 0; i < (int)imgs.size(); i++ )
        organizeSample( imgs[i], masks[i], pos, neg );

     // std::vector -> cv::Mat           // 3Nx1 -> Nx3
    pos_samples = cv::Mat( pos, true );
    pos_samples = pos_samples.reshape( 1, pos.size() / channels );
    neg_samples = cv::Mat( neg, true );
    neg_samples = neg_samples.reshape( 1, neg.size() / channels );
}


// organize samples in an array
void MOG::organizeSample( const cv::Mat& img,
                          const cv::Mat& mask,
                          std::vector<double>& pos,
                          std::vector<double>& neg ){

    CV_Assert( img.cols == mask.cols &&
               img.rows == mask.rows &&
               mask.type() == CV_16UC1 );



    const size_t channels = img.channels();
    double sample[channels];

    for ( int yyy = 0; yyy < img.rows; yyy++ ) {
        for ( int xxx = 0; xxx < img.cols; xxx++ ) {
            // 3 channel pixel to 1x3 mat
            sample[0] = img.at<cv::Vec3b>( yyy, xxx ) ( 0 );
            sample[1] = img.at<cv::Vec3b>( yyy, xxx ) ( 1 );
            sample[2] = img.at<cv::Vec3b>( yyy, xxx ) ( 2 );

            // appends 3 elements (of 3 channels)
            // at the end of *pos* or *neg*
            if ( mask.at<unsigned short>( yyy, xxx ) > 0 )
                pos.insert( pos.end(), sample, sample + channels );
            else
                neg.insert( neg.end(), sample, sample + channels );
        }
    }

}
