#include "mog.h"

MOG::MOG(){
    const cv::TermCriteria criteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 10, FLT_EPSILON);
    pos_model = new cv::EM(cv::EM::DEFAULT_NCLUSTERS, cv::EM::COV_MAT_DIAGONAL, criteria);
    neg_model = new cv::EM(cv::EM::DEFAULT_NCLUSTERS, cv::EM::COV_MAT_DIAGONAL, criteria);
}

MOG::~MOG(){
    delete pos_model;
    delete neg_model;
}
