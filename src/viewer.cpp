#include "viewer.h"

Viewer::Viewer(){
    exit_flag = false;
    save_memory = false;
    limitx_min = 0;
    limitx_max = 0;
    limity_min = 0;
    limity_max = 0;
    limitz_min = 0;
    limitz_max = 0;
    frame_width = 640;
    frame_height = 480;
}


bool Viewer::initialize(){
    // initialize device and sensors
    rc = openni::OpenNI::initialize();
    if ( rc != openni::STATUS_OK ){
        error_manager( 1 );
        return false;
    }

    openni::Array<openni::DeviceInfo> dev_inf;
    openni::OpenNI::enumerateDevices(&dev_inf);
    if (dev_inf.getSize() == 0){
        error_manager( 7 );
        return false;
    }

    rc = device.open( openni::ANY_DEVICE );
    if ( rc != openni::STATUS_OK ){
        error_manager( 2 );
        return false;
    }


    rc = depth.create( device, openni::SENSOR_DEPTH );
    if ( rc != openni::STATUS_OK ){
        error_manager( 3 );
        return false;
    }

    // set the new resolution and fps
    openni::VideoMode depth_videoMode  = depth.getVideoMode();
    depth_videoMode.setResolution( frame_width, frame_height );
    depth_videoMode.setFps( 30 );
    depth.setVideoMode( depth_videoMode );

    rc = depth.start();

    only_depth = false;

    if ( rc != openni::STATUS_OK ){
        error_manager( 4 );
        return false;
    }
    // try to read the color stream
    const openni::SensorInfo* tmp = device.getSensorInfo( openni::SENSOR_COLOR );
    if ( tmp != NULL ) {
        rc = color.create( device, openni::SENSOR_COLOR );
        if ( rc != openni::STATUS_OK ){
            error_manager( 5 );
            return false;
        }
        // set the new resolution and fps
        openni::VideoMode color_videoMode  = color.getVideoMode();
        color_videoMode.setResolution( frame_width, frame_height );
        color_videoMode.setFps( 30 );
        color.setVideoMode( color_videoMode );

        rc = color.start();
        if ( rc != openni::STATUS_OK ){
            error_manager( 6 );
            return false;
        }

        // align the depth and color image
        device.setImageRegistrationMode( openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR );
    } else
        only_depth = true;

    std::cout << "Sensors initialized" << std::endl;
    return true;
}


void Viewer::shutdownCameras(){
    std::cout << "Stopping sensors" << std::endl;
    cv::destroyAllWindows();
    depth.stop();
    depth.destroy();
    if ( !only_depth ) {
        color.stop();
        color.destroy();
    }
    device.close();
    //QFile::remove("recording.oni");
    openni::OpenNI::shutdown();
    std::cout << "Sensors stopped" << std::endl;
}

void Viewer::reset(){
    exit_flag = false;
    save_memory = false;
}

Viewer::~Viewer(){
    shutdownCameras();
}
