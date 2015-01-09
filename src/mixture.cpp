#include "mixture.h"

Mixture::Mixture(){
}


double Mixture::computeProbability( cv::Vec3b BGR ){
    double prob = finalWeight * exp( -0.5 * ( pow((double)BGR[2] - mean.at<double>( 0 ), 2 ) * invCovariance.at<double>( 0 ) + // BGR->RGB
                                              pow((double)BGR[1] - mean.at<double>( 1 ), 2 ) * invCovariance.at<double>( 1 ) +
                                              pow((double)BGR[0] - mean.at<double>( 2 ), 2 ) * invCovariance.at<double>( 2 )
                                              )
                                     );

    return prob;
}
