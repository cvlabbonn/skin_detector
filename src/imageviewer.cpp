#include "imageviewer.h"
#include "ui_imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageViewer)
{
    ui->setupUi(this);
    ui->threshold_slider->setEnabled(false);
    ui->threshold_label->setEnabled(false);
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::closeEvent(QCloseEvent *){
    emit closeWindow();
}
void ImageViewer::loadImage(cv::Mat image){
    if (images.size() > 0){
        QImage qt_img;
        cv::Mat current_img;
        if (image.empty()){
            current_img = images[frame];
        } else {
            current_img = image;
        }

        if(current_img.type() == CV_8UC3){
            cv::cvtColor( current_img, current_img, CV_BGR2RGB );
        } else if(current_img.type() == CV_8U){
            cv::cvtColor( current_img, current_img, CV_GRAY2RGB );
        }

        qt_img = QImage((uchar*)current_img.data, current_img.cols, current_img.rows, QImage::Format_RGB888 );
        ui->image_area->clear();
        ui->image_area->setPixmap( QPixmap::fromImage( qt_img ) );
        changeCounter();
    }
}

void ImageViewer::reset(){
    frame = 0;
    loadImage();
}

void ImageViewer::changeCounter(){
    QString new_text("Image ");
    new_text += QString::number(frame+1) + " of " + QString::number(images.size());
    ui->image_counter->setText(new_text);
}

void ImageViewer::on_prev_button_clicked()
{
    frame = std::max(0, frame - 1);
    loadImage();
}

void ImageViewer::on_next_button_clicked()
{
    frame = std::min((int) images.size()-1, frame + 1);
    loadImage();
}

void ImageViewer::on_threshold_slider_valueChanged(int value)
{
    ui->threshold_label->setText(QString::number(value));
    cv::Mat current = images[frame].clone();
    if(current.type() == CV_8UC3){
        cv::cvtColor( current, current, CV_BGR2GRAY );
    }
    cv::threshold(current, current, (double)value, 255, cv::THRESH_BINARY);
    loadImage(current);
}

void ImageViewer::on_threshold_check_toggled(bool checked)
{
    ui->threshold_slider->setEnabled(checked);
    ui->threshold_label->setEnabled(checked);
}
