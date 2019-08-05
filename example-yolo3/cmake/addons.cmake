set(ADDON_DIRECTORY "${OF_DIRECTORY}/addons")
set(ADDON_DIRECTORY_LOCAL "addons")
set(OF_ADDON_HEADERS)
set(OF_ADDON_LIBS)


include_directories(${OF_ADDON_HEADERS})

# includes

# -- OFX_CV --
file(GLOB OFX_CV
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/Calibration.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/ContourFinder.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/Distance.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/Flow.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/Helpers.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/Kalman.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/ObjectFinder.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/RunningBackground.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/Tracker.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/Utilities.cpp"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/src/Wrappers.cpp")

set(HEADER_OFX_CV "${ADDON_DIRECTORY}/ofxCv/src/"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv"
        "${ADDON_DIRECTORY}/ofxCv/libs/ofxCv/include"
        "${ADDON_DIRECTORY}/ofxCv/libs/CLD/include/CLD/" )


# -- OFX_SPINNAKER --

set(HEADER_OFX_SPINNAKER "/usr/include/spinnaker" )

set(LINK_DIR_OFX_SPINNAKER "/usr/src/spinnaker/lib" )

set(LIBS_OFX_SPINNAKER Spinnaker)



# -- OFX_YOLOSOLO --

file(GLOB OFX_YOLOSOLO
        "${ADDON_DIRECTORY}/ofxYoloSolo/src/ofxYoloSolo.cpp"
        )

set(HEADER_OFX_YOLOSOLO
        "${ADDON_DIRECTORY}/ofxYoloSolo/src/"
        "${ADDON_DIRECTORY}/ofxYoloSolo/libs/darknet/include/"
        "/usr/local/cuda/include")

set(LINK_DIR_OFX_YOLOSOLO ${ADDON_DIRECTORY}/ofxYoloSolo/libs/darknet/lib/linux64)

set(LIBS_OFX_YOLOSOLO darknet)


# -- OFX_OPENCV --

file(GLOB OFX_OPENCV
        "${ADDON_DIRECTORY}/ofxOpenCv/src/ofxCvColorImage.cpp"
        "${ADDON_DIRECTORY}/ofxOpenCv/src/ofxCvContourFinder.cpp"
        "${ADDON_DIRECTORY}/ofxOpenCv/src/ofxCvFloatImage.cpp"
        "${ADDON_DIRECTORY}/ofxOpenCv/src/ofxCvGrayscaleImage.cpp"
        "${ADDON_DIRECTORY}/ofxOpenCv/src/ofxCvHaarFinder.cpp"
        "${ADDON_DIRECTORY}/ofxOpenCv/src/ofxCvImage.cpp"
        "${ADDON_DIRECTORY}/ofxOpenCv/src/ofxCvShortImage.cpp"
        "${ADDON_DIRECTORY}/ofxOpenCv/src/ofxCv"
        )

set(HEADER_OFX_OPENCV
        "${ADDON_DIRECTORY}/ofxOpenCv/src/"
        "${ADDON_DIRECTORY}/ofxOpenCv/libs/opencv/include"
        "${ADDON_DIRECTORY}/ofxOpenCv/libs/opencv/include/opencv"
        "${ADDON_DIRECTORY}/ofxOpenCv/libs/opencv/include/opencv2"
        )

find_package( OpenCV REQUIRED )

set(LIBS_OFX_OPENCV ${OpenCV_LIBS})



#####
set(ADDON_SOURCE_FILES
        ${OFX_CV}
        ${OFX_YOLOSOLO}
        ${OFX_OPENCV})

set(OF_ADDON_HEADERS
        ${HEADER_OFX_CV}
        ${HEADER_OFX_YOLOSOLO}
        ${HEADER_OFX_OPENCV}
        )

set( OF_ADDON_LIBS
        ${LIBS_OFX_OPENCV}
        ${LIBS_OFX_YOLOSOLO}
        )

set( OF_ADDON_LINK_DIRS
        ${LINK_DIR_OFX_YOLOSOLO}
        )