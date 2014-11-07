#include <viewer.h>

void Viewer::key_parse( char key ){
    if ( key == 27 || key == 'q' || key == 'Q' )
        exit_flag = true;
    else if ( key == ' ' ) {
        save_memory = !save_memory;
        if ( save_memory ) {
            std::cout << "Saving Images into memory" << std::endl;
            rgb_images.clear();
            depth_images.clear();
        } else{
            std::cout << "Saved " << rgb_images.size() << " images into memory" << std::endl;
        }
    }
}
