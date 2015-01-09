#ifndef MIXTURE_H
#define MIXTURE_H

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Mixture {
public:
    cv::Mat mean;
    cv::Mat invCovariance;  // inverse of the diagonal Covariance matrix
    double finalWeight;             // mixture weight / ( (2*pi)^3/2 * sqrt( |cov| ) ); // see page 10 @ the paper "Statistical Color Models with Application to Skin Detection" by M.Jones and J.Rehg

    Mixture();
    Mixture(cv::Mat mu, cv::Mat invCov, double weight) : mean(mu), invCovariance(invCov), finalWeight(weight){}
    double computeProbability( cv::Vec3b pixelBGRB );

};

#endif // MIXTURE_H
