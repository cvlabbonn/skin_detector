#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Ui {
    class ImageViewer;
}

class ImageViewer : public QWidget {
    Q_OBJECT

public:
    explicit ImageViewer( QWidget *parent = 0 );
    ~ImageViewer();
    std::vector<cv::Mat> images;
    void reset();

private:
    Ui::ImageViewer *ui;
    int frame;
    void closeEvent(QCloseEvent *);
    void loadImage(cv::Mat image = cv::Mat());
    void changeCounter();
signals:
    void closeWindow();
private slots:
    void on_prev_button_clicked();
    void on_next_button_clicked();
    void on_threshold_slider_valueChanged(int value);
    void on_threshold_check_toggled(bool checked);
};

#endif // IMAGEVIEWER_H
