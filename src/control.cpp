#include <control.h>
#include "ui_control.h"

Control::Control(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Control)
{
    ui->setupUi(this);
    mog = new MOG;
    img_viewer = new ImageViewer();
    connect(img_viewer,SIGNAL(closeWindow()),this, SLOT(closeViewer()));
    img_viewer->hide();
}

Control::~Control()
{
    delete ui;
    delete mog;
    delete img_viewer;
}

void Control::closeEvent(QCloseEvent *){
    viewer->exit_flag=true;
}

void Control::setInitialVariables(){
    ui->x_min_slider->setRange(0, viewer->frame_width);
    ui->x_max_slider->setRange(0, viewer->frame_width);
    ui->x_max_slider->setValue(ui->x_max_slider->maximum());
    ui->y_min_slider->setRange(0, viewer->frame_height);
    ui->y_max_slider->setRange(0, viewer->frame_height);
    ui->y_max_slider->setValue(ui->y_max_slider->maximum());
    ui->z_min_slider->setRange(0, 10000);
    ui->z_max_slider->setRange(0, 10000);
    ui->z_max_slider->setValue(ui->z_max_slider->maximum());

}

void Control::trainMoG(){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Train", "Are you sure you want to train "+ QString::number(viewer->rgb_images.size())+" images ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      if ( viewer->rgb_images.size() == viewer->depth_images.size() && viewer->rgb_images.size()>0){
          mog->train(viewer->rgb_images, viewer->depth_images);
      }
    }
}

void Control::closeViewer(){
    img_viewer->hide();
    this->setEnabled(true);
}

void Control::on_x_min_slider_valueChanged(int value)
{
    ui->x_min_label->setText(QString::number(value));
    viewer->limitx_min = value;
}

void Control::on_x_max_slider_valueChanged(int value)
{
    ui->x_max_label->setText(QString::number(value));
    viewer->limitx_max = value;
}

void Control::on_y_min_slider_valueChanged(int value)
{
    ui->y_min_label->setText(QString::number(value));
    viewer->limity_min = value;
}

void Control::on_y_max_slider_valueChanged(int value)
{
    ui->y_max_label->setText(QString::number(value));
    viewer->limity_max = value;
}

void Control::on_z_min_slider_valueChanged(int value)
{
    ui->z_min_label->setText(QString::number(value));
    viewer->limitz_min = value;
}

void Control::on_z_max_slider_valueChanged(int value)
{
    ui->z_max_label->setText(QString::number(value));
    viewer->limitz_max = value;
}

void Control::on_train_button_clicked()
{
    trainMoG();
}

void Control::on_view_record_button_clicked()
{
    img_viewer->show();
    img_viewer->images = viewer->rgb_images;
    img_viewer->reset();
    this->setEnabled(false);
}

void Control::on_save_model_button_clicked()
{
//    cv::FileStorage fs_pos("pos_model.xml", cv::FileStorage::WRITE);
//    mog->pos_model->write(fs_pos);
//    fs_pos.release();
//    cv::FileStorage fs_neg("neg_model.xml", cv::FileStorage::WRITE);
//    mog->pos_model->write(fs_neg);
//    fs_neg.release();
    QDir basedir = QDir(QApplication::applicationDirPath());
    basedir.cdUp();
    basedir.cd("config");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                    basedir.absolutePath()+"/models.ini",
                                                    tr("Config file (*.ini)"));
    mog->save(fileName.toStdString());
    QMessageBox messageBox;
    messageBox.information(this, "Models saved", "The current mixture models have been saved.");
}

void Control::on_load_model_button_clicked()
{
//    cv::FileStorage fs_pos("pos_model.xml", cv::FileStorage::READ);
//    const cv::FileNode& fn_pos = fs_pos["StatModel.EM"];
//    mog->pos_model->read(fn_pos);
//    fs_pos.release();
//    cv::FileStorage fs_neg("neg_model.xml", cv::FileStorage::READ);
//    const cv::FileNode& fn_neg = fs_neg["StatModel.EM"];
//    mog->neg_model->read(fn_neg);
//    fs_neg.release();
//    mog->emToMixture();
    QDir basedir = QDir(QApplication::applicationDirPath());
    basedir.cdUp();
    basedir.cd("config");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    basedir.absolutePath()+"/models.ini",
                                                    tr("Config file (*.ini)"));
    mog->load(fileName.toStdString());
    QMessageBox messageBox;
    messageBox.information(this, "Models loaded", "The mixture models have been loaded.");
}

void Control::on_record_button_clicked()
{
    viewer->save_memory = true;
}

void Control::on_stop_record_button_clicked()
{
    viewer->save_memory = false;
}

void Control::on_test_button_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Test", "Are you sure you want to test with the current data of "+ QString::number(viewer->rgb_images.size())+" images ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      if ( viewer->rgb_images.size() == viewer->depth_images.size() && viewer->rgb_images.size()>0){
          mog->test(viewer->rgb_images);
          img_viewer->images = mog->test_result;
          img_viewer->reset();
          img_viewer->show();
          this->setEnabled(false);
      }
    }
}
