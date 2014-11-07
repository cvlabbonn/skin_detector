#ifndef CONTROL_H
#define CONTROL_H

#include <QMainWindow>
#include <viewer.h>
#include <mog.h>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class Control;
}

class Control : public QMainWindow
{
    Q_OBJECT

public:
    explicit Control(QWidget *parent = 0);
    void setInitialVariables();
    ~Control();

    Viewer* viewer;
    MOG* mog;

private:
    void closeEvent(QCloseEvent *event);
    void trainMoG();

private slots:
    void on_x_min_slider_valueChanged(int value);

    void on_x_max_slider_valueChanged(int value);

    void on_y_min_slider_valueChanged(int value);

    void on_y_max_slider_valueChanged(int value);

    void on_z_min_slider_valueChanged(int value);

    void on_z_max_slider_valueChanged(int value);

    void on_train_button_clicked();

private:
    Ui::Control *ui;
};

#endif // CONTROL_H
