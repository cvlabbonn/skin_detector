#include "control.h"
#include "ui_control.h"

Control::Control(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Control)
{
    ui->setupUi(this);
    mog = new(MOG);
}

Control::~Control()
{
    delete ui;
    delete mog;
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
    reply = QMessageBox::question(this, "Test", "Are you sure you want to train "+ QString::number(viewer->rgb_images.size())+" images ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
      if ( viewer->rgb_images.size() == viewer->depth_images.size() && viewer->rgb_images.size()>0){
          mog->train(viewer->rgb_images, viewer->depth_images);
      }
    }
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
