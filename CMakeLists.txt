project(skin_detector)
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories( ${PROJECT_SOURCE_DIR}/include )

# Opencv required
find_package(OpenCV REQUIRED)
include_directories(${OPENCV_INCLUDE_DIRS})
link_directories(${OPENCV_LIBRARY_DIRS})
add_definitions(${OPENCV_DEFINITIONS})

message("\n\nFound OpenCV\n\n")

# QT4 required
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

message("\n\nFound QT4\n\n")

# Openni2 required
set(Openni2_INCLUDE $ENV{OPENNI2_INCLUDE})
if(NOT Openni2_INCLUDE)
  message(FATAL_ERROR "Could not find OPENNI2_INCLUDE environment variable")
endif(NOT Openni2_INCLUDE)

set(Openni2_REDIST $ENV{OPENNI2_REDIST})
if(NOT Openni2_REDIST)
  message(FATAL_ERROR "Could not find OPENNI2_REDIST environment variable")
endif(NOT Openni2_REDIST)

include_directories( ${Openni2_INCLUDE} )
link_directories( ${Openni2_REDIST} )

message("\n\nFound Openni2\n\n")

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


set(VIEWER_SOURCES
    src/main.cpp
    src/control.cpp
    src/imageviewer.cpp
    src/mog.cpp
    src/mog_io.cpp
    src/mog_train.cpp
    src/mog_test.cpp
    src/mixture.cpp
    src/viewer.cpp
    src/viewer_error.cpp
    src/viewer_visualizer.cpp
    src/viewer_keyio.cpp
    include/mog.h
    include/viewer.h
    include/mixture.h
)


# set QT headers
SET(QT_HEADERS
    include/control.h
    include/imageviewer.h
    )

#set QT forms
SET(QT_FORMS
    ui/control.ui
    ui/imageviewer.ui
)

# create moc for QT
QT4_WRAP_CPP(QT_MOC ${QT_HEADERS})

# process ui
QT4_WRAP_UI(QT_FORMS_HEADERS ${QT_FORMS})

ADD_EXECUTABLE(skin_detector ${VIEWER_SOURCES}
    ${QT_HEADERS}
    ${QT_MOC}
    ${QT_FORMS})

TARGET_LINK_LIBRARIES(skin_detector ${QT_LIBRARIES} ${OpenCV_LIBS} OpenNI2)
