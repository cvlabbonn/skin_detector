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
    viewer = NULL;
    ui->with_cam->setEnabled(false);
    trained=false;
}

Control::~Control()
{
    delete ui;
    delete mog;
    delete img_viewer;
}

void Control::closeEvent(QCloseEvent *){
    viewer->exit_flag=true;
    viewer->~Viewer();
}

void Control::setInitialVariables(){
    ui->x_min_slider->setRange(0, viewer->frame_width);
    ui->x_max_slider->setRange(0, viewer->frame_width);
    ui->x_max_slider->setValue(ui->x_max_slider->maximum());
    on_x_max_slider_valueChanged(ui->x_max_slider->maximum());
    ui->y_min_slider->setRange(0, viewer->frame_height);
    ui->y_max_slider->setRange(0, viewer->frame_height);
    ui->y_max_slider->setValue(ui->y_max_slider->maximum());
    on_y_max_slider_valueChanged(ui->y_max_slider->maximum());
    ui->z_min_slider->setRange(0, 10000);
    ui->z_max_slider->setRange(0, 10000);
    ui->z_max_slider->setValue(ui->z_max_slider->maximum());
    on_z_max_slider_valueChanged(ui->z_max_slider->maximum());

}

void Control::trainMoG(){
    if (!viewer || viewer->rgb_images.size() == 0){
        QMessageBox messageBox;
        messageBox.information(this, "No training images", "You need to capture the images that will be trained.");
    } else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Train", "Are you sure you want to train "+ QString::number(viewer->rgb_images.size())+" images ?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          if ( viewer->rgb_images.size() == viewer->depth_images.size() && viewer->rgb_images.size()>0){
              mog->train(viewer->rgb_images, viewer->depth_images);
              trained=true;
          }
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
    if (!viewer || viewer->rgb_images.size() == 0){
        QMessageBox messageBox;
        messageBox.information(this, "No recorded images", "You need to capture the images first.");
    } else {
        img_viewer->show();
        img_viewer->images = viewer->rgb_images;
        img_viewer->reset();
        this->setEnabled(false);
    }
}

void Control::on_save_model_button_clicked()
{
    if(trained){
        QDir basedir = QDir(QApplication::applicationDirPath());
        basedir.cdUp();
        basedir.cd("config");
        QString fileName = QFileDialog::getSaveFileName(this, tr("Open File"),
                                                        basedir.absolutePath()+"/models.ini",
                                                        tr("Config file (*.ini)"));
        mog->save(fileName.toStdString());
        QMessageBox messageBox;
        messageBox.information(this, "Models saved", "The current mixture models have been saved.");
    } else{
        QMessageBox messageBox;
        messageBox.information(this, "No Model Found", "There is no model to be saved.");
    }
}

void Control::on_load_model_button_clicked()
{
    QDir basedir = QDir(QApplication::applicationDirPath());
    basedir.cdUp();
    basedir.cd("config");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    basedir.absolutePath()+"/models.ini",
                                                    tr("Config file (*.ini)"));
    bool ret = mog->load(fileName.toStdString());
    QMessageBox messageBox;
    if (ret){
        messageBox.information(this, "Models loaded", "The mixture models have been loaded.");
        trained = true;
    } else {
        messageBox.information(this, "Models not loaded", "The mixture models could not be loaded.");
    }
}

void Control::on_record_button_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Record", "This will delete any previous record, are you sure you want to continue ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
          viewer->key_parse(' ');
          ui->stop_record_button->setEnabled(true);
          ui->record_button->setEnabled(false);
    }

}

void Control::on_stop_record_button_clicked()
{
    viewer->key_parse(' ');
    ui->record_button->setEnabled(true);
    ui->stop_record_button->setEnabled(false);
}

void Control::on_test_button_clicked()
{

    if (!trained){
        QMessageBox messageBox;
        messageBox.information(this, "Models not loaded", "You need to train your models first or load a trained model.");
    } else {
        if (!viewer || viewer->rgb_images.size() == 0){
            QMessageBox messageBox;
            messageBox.information(this, "No test images", "You need to capture the images that will be tested.");
        } else{
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Test", "Are you sure you want to test with the current data of "+ QString::number(viewer->rgb_images.size())+" images ?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
                if ( viewer->rgb_images.size() == viewer->depth_images.size() && viewer->rgb_images.size()>0){
                    this->setEnabled(false);
                    std::cout << "Testing images, please wait." << std::endl;
                    mog->test(viewer->rgb_images);
                    img_viewer->images = mog->test_result;
                    img_viewer->reset();
                    img_viewer->show();

                }
            }
        }
    }
}

void Control::on_start_camera_clicked()
{
    //Viewer view;
    if (!viewer){
        viewer = new Viewer();
        setInitialVariables();
    }

    bool isOk = viewer->initialize();
    if (!isOk) {
        QMessageBox messageBox;
        messageBox.information(this, "Error initializing the camera", "The camera could not initialize, please try again.");
        viewer->shutdownCameras();
        return;
    }
    ui->with_cam->setEnabled(true);
    viewer->reset();
    int ret  = viewer->loop();

}

void Control::on_stop_camera_clicked()
{
    viewer->exit_flag = true;
    viewer->shutdownCameras();
    ui->with_cam->setEnabled(false);
}
