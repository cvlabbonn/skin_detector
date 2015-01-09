#include <mog.h>

void MOG::emToMixture(){
    // get mixture variables from the EM models
    const cv::Mat pos_means = pos_model->get<cv::Mat>("means");
    const cv::Mat neg_means = neg_model->get<cv::Mat>("means");
    const cv::Mat pos_weights = pos_model->get<cv::Mat>("weights");
    const cv::Mat neg_weights = neg_model->get<cv::Mat>("weights");
    const std::vector<cv::Mat> pos_covs = pos_model->get< std::vector<cv::Mat> >("covs");
    const std::vector<cv::Mat> neg_covs = neg_model->get< std::vector<cv::Mat> >("covs");

    // resize the mixture vectors
    posMixtureModels.resize(pos_weights.cols);
    negMixtureModels.resize(neg_weights.cols);
    // add the data to the mixture models
    for(int i = 0; i < (int) pos_weights.cols; i++){
        pos_means.row(i).copyTo(posMixtureModels[i].mean);
        neg_means.row(i).copyTo(negMixtureModels[i].mean);
        posMixtureModels[i].invCovariance = pos_covs[i].inv().diag().t();
        negMixtureModels[i].invCovariance = neg_covs[i].inv().diag().t();
        posMixtureModels[i].finalWeight = pos_weights.at<double>(i) / (pow(2*M_PI, 3.0/2.0) *  pow(cv::determinant(pos_covs[i]),0.5)) ;
        negMixtureModels[i].finalWeight = neg_weights.at<double>(i) / (pow(2*M_PI, 3.0/2.0) *  pow(cv::determinant(neg_covs[i]),0.5)) ;
    }


}

void MOG::save(std::string filename){
    QSettings settings( QString::fromStdString(filename), QSettings::IniFormat );
    settings.beginGroup( "PositiveModel" );
    settings.beginWriteArray("means",posMixtureModels.size());
    for(int i = 0; i < posMixtureModels.size(); i++){
        settings.beginWriteArray(QString::number(i),posMixtureModels[i].mean.cols);
        for(int j = 0; j < posMixtureModels[i].mean.cols ; j++){
            settings.setValue(QString::number(j), posMixtureModels[i].mean.at<double>(j));
        }
        settings.endArray();
    }
    settings.endArray();
    settings.beginWriteArray("invCov",posMixtureModels.size());
    for(int i = 0; i < posMixtureModels.size(); i++){
        settings.beginWriteArray(QString::number(i),posMixtureModels[i].invCovariance.cols);
        for(int j = 0; j < posMixtureModels[i].invCovariance.cols ; j++){
            settings.setValue(QString::number(j), posMixtureModels[i].invCovariance.at<double>(j));
        }
        settings.endArray();
    }
    settings.endArray();
    settings.beginWriteArray("finalWeight",posMixtureModels.size());
    for(int i = 0; i < posMixtureModels.size(); i++){
        settings.setValue(QString::number(i), posMixtureModels[i].finalWeight);
    }
    settings.endArray();
    settings.endGroup();

    settings.beginGroup( "NegativeModel" );
    settings.beginWriteArray("means",negMixtureModels.size());
    for(int i = 0; i < negMixtureModels.size(); i++){
        settings.beginWriteArray(QString::number(i),negMixtureModels[i].mean.cols);
        for(int j = 0; j < negMixtureModels[i].mean.cols ; j++){
            settings.setValue(QString::number(j), negMixtureModels[i].mean.at<double>(j));
        }
        settings.endArray();
    }
    settings.endArray();
    settings.beginWriteArray("invCov",negMixtureModels.size());
    for(int i = 0; i < negMixtureModels.size(); i++){
        settings.beginWriteArray(QString::number(i),negMixtureModels[i].invCovariance.cols);
        for(int j = 0; j < negMixtureModels[i].invCovariance.cols ; j++){
            settings.setValue(QString::number(j), negMixtureModels[i].invCovariance.at<double>(j));
        }
        settings.endArray();
    }
    settings.endArray();
    settings.beginWriteArray("finalWeight", negMixtureModels.size());
    for(int i = 0; i < negMixtureModels.size(); i++){
        settings.setValue(QString::number(i), negMixtureModels[i].finalWeight);
    }
    settings.endArray();
    settings.endGroup();
}

void MOG::load(std::string filename){
    QSettings settings( QString::fromStdString(filename), QSettings::IniFormat );
    settings.beginGroup( "PositiveModel" );
    int size = settings.beginReadArray("means");
    if (size == 0){
        std::cout << "Nothing to load in the file" << std::endl;
        return;
    }
    // resize the mixture vectors
    posMixtureModels.resize(size);
    negMixtureModels.resize(size);

    for(int i = 0; i < size; i++){
        int mean_size = settings.beginReadArray(QString::number(i));
        posMixtureModels[i].mean.create(1,mean_size,CV_64F);
        for(int j = 0; j < mean_size ; j++){
            posMixtureModels[i].mean.at<double>(j) = settings.value(QString::number(j)).toDouble();
        }
        settings.endArray();
    }
    settings.endArray();

    size = settings.beginReadArray("invCov");
    for(int i = 0; i < size; i++){
        int cov_size = settings.beginReadArray(QString::number(i));
        posMixtureModels[i].invCovariance.create(1,cov_size,CV_64F);
        for(int j = 0; j < cov_size ; j++){
            posMixtureModels[i].invCovariance.at<double>(j) = settings.value(QString::number(j)).toDouble();
        }
        settings.endArray();
    }
    settings.endArray();

    size = settings.beginReadArray("finalWeight");
    for(int i = 0; i < size; i++){
        posMixtureModels[i].finalWeight = settings.value(QString::number(i)).toDouble();
    }
    settings.endArray();
    settings.endGroup();

    settings.beginGroup( "NegativeModel" );
    size = settings.beginReadArray("means");
    for(int i = 0; i < size; i++){
        int mean_size = settings.beginReadArray(QString::number(i));
        negMixtureModels[i].mean.create(1,mean_size,CV_64F);
        for(int j = 0; j < mean_size ; j++){
            negMixtureModels[i].mean.at<double>(j) = settings.value(QString::number(j)).toDouble();
        }
        settings.endArray();
    }
    settings.endArray();

    size = settings.beginReadArray("invCov");
    for(int i = 0; i < size; i++){
        int cov_size = settings.beginReadArray(QString::number(i));
        negMixtureModels[i].invCovariance.create(1,cov_size,CV_64F);
        for(int j = 0; j < cov_size ; j++){
            negMixtureModels[i].invCovariance.at<double>(j) = settings.value(QString::number(j)).toDouble();
        }
        settings.endArray();
    }
    settings.endArray();

    size = settings.beginReadArray("finalWeight");
    for(int i = 0; i < size; i++){
        negMixtureModels[i].finalWeight = settings.value(QString::number(i)).toDouble();
    }
    settings.endArray();
    settings.endGroup();

}
