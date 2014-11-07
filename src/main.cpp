#include "control.h"
#include "viewer.h"
#include <QApplication>

int main( int argc, char *argv[] ){
    QApplication a( argc, argv );
    Control w;
    Viewer viewer;
    w.show();
    w.viewer = &viewer;
    w.setInitialVariables();
    viewer.initialize();
    int ret  = viewer.loop();
    if (ret == 0){
        viewer.~Viewer();
        exit(0);
    }

    return a.exec();
}
