#include <viewer.h>

int Viewer::loop(){
    char key;

    // loop until esc is pressed
    while ( !exit_flag )
        //std::cout << "enter" << std::endl;
        if ( device.getSensorInfo( openni::SENSOR_DEPTH ) != NULL ) {
//             read the next frame
            depth.readFrame( &depthFrame );
            if ( !only_depth )
                color.readFrame( &colorFrame );
             //             make sure the data is valid

            if (( depthFrame.isValid() && only_depth ) || ( !only_depth && colorFrame.isValid() && depthFrame.isValid())) {
                cv::Mat normDepth, rgbd, depth_thresh;
                //process both images
                // depth image
                const openni::DepthPixel* depthBuffer = (const openni::DepthPixel*)depthFrame.getData();
                frameDepth.create( depthFrame.getHeight(), depthFrame.getWidth(), CV_16UC1 );
                memcpy( frameDepth.data, depthBuffer, depthFrame.getHeight() * depthFrame.getWidth() * sizeof( uint16_t ) );
                cv::flip( frameDepth, frameDepth, 1 );

                if ( !only_depth ) {
                    // color image
                    const openni::RGB888Pixel* imageBuffer = (const openni::RGB888Pixel*)colorFrame.getData();
                    frame.create( colorFrame.getHeight(), colorFrame.getWidth(), CV_8UC3 );
                    memcpy( frame.data, imageBuffer, 3 * colorFrame.getHeight() * colorFrame.getWidth() * sizeof( uint8_t ) );
                    cv::flip( frame, frame, 1 );
                    cv::cvtColor( frame, bgrMat, CV_RGB2BGR );
                    cv::imshow( "Color", bgrMat );
                }

                // create the rgbd image
                createRGBD( frameDepth, bgrMat, rgbd, depth_thresh );
                if ( !only_depth )
                    cv::imshow( "RGBD", rgbd );

                //normalize and show depth data
                cv::normalize( depth_thresh, normDepth, 0, 255, CV_MINMAX, CV_8UC1 );
                cv::imshow( "Depth", normDepth );

                if ( save_memory ) {
                    //store all data
                    if ( !only_depth )
                        rgb_images.push_back( bgrMat.clone() );
                    depth_images.push_back( depth_thresh.clone() );

                }

                // get key interaction
                key = cv::waitKey( 1 );
                key_parse( key );

            }
        }
    return 0;
}


void Viewer::createRGBD( cv::Mat& depth_mat, cv::Mat& color_mat, cv::Mat& dst_rgbd, cv::Mat& dst_depth ){

    dst_rgbd = cv::Mat::zeros( depth_mat.rows, depth_mat.cols, CV_8UC3 );
    dst_depth = cv::Mat::zeros( depth_mat.rows, depth_mat.cols, CV_16UC1 );

    for ( int j = 0; j < depth_mat.rows; j++ ) {
        for ( int i = 0; i < depth_mat.cols; i++ ) {
            int depth_value = (int)depth_mat.at<unsigned short>( j, i );
            if ( depth_value != 0 && depth_value <= limitz_max && depth_value >= limitz_min )
                if ( limitx_min <= i && limitx_max >= i && limity_min <= j && limity_max >= j ) {
                    if ( !only_depth )
                        dst_rgbd.at<cv::Vec3b>( j, i )  = color_mat.at<cv::Vec3b>( j, i );
                    dst_depth.at<unsigned short>( j, i )  = depth_mat.at<unsigned short>( j, i );
                }
        }
    }
}
