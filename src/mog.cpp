#include "mog.h"

MOG::MOG(){
    const cv::TermCriteria criteria(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 10, FLT_EPSILON);
    
    this->neg_model = cv::ml::EM::create();
    this->pos_model = cv::ml::EM::create();

    this->neg_model->setClustersNumber(16);
    this->pos_model->setClustersNumber(16);

    this->neg_model->setCovarianceMatrixType(cv::ml::EM::COV_MAT_DIAGONAL);
    this->pos_model->setCovarianceMatrixType(cv::ml::EM::COV_MAT_DIAGONAL);

    this->neg_model->setTermCriteria(criteria);
    this->pos_model->setTermCriteria(criteria);
    
}

MOG::~MOG(){
}

