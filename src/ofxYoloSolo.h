#pragma once

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define GPU
#define OPENCV
//#define TRACK_OPTFLOW

#include "ofMain.h"

// #include "ofxCv.h"
// #include "ofxOpenCv.h"

#include "yolo_v2_class.hpp"    // imported functions from DLL

#ifdef OPENCV
#include "opencv2/highgui/highgui_c.h"
#endif


struct DetectedObject {
    unsigned int id;
	ofRectangle rect;
    ofRectangle rectPredicted;
	std::string label;
	float probability;
	uint64_t lastDetected;
};

struct DetectedObjects {
//    ofPixels pix;
    std::map<unsigned int, DetectedObject> objects;
};


class ofxYoloSolo: public ofThread
{
public:
	ofxYoloSolo();
	~ofxYoloSolo();

	void init( std::string cfgfile, std::string weightfile, std::string nameslist = "");
    bool isLoaded() {return loaded;};
    bool hasNewData();
    void yolo_nono( ofPixels & pix, float threshold = 0.24f, float maxOverlap = 0.5f );

    DetectedObjects getDetectedObjects();

    int getTrackingMaxDist(){ return trackingMaxDist;};
    void setTrackingMaxDist(int trackingMaxDist){ this->trackingMaxDist = trackingMaxDist;};

    int getTrackingFrameHistory(){ return trackingFrameHistory;};
    void setTrackingFrameHistory(int trackingFrameHistory){ this->trackingFrameHistory = trackingFrameHistory;};

    bool isTrackingChangeHistory(){ return trackingChangeHistory;};
    void setTrackingChangeHistory(bool trackingChangeHistory){ this->trackingChangeHistory = trackingChangeHistory;};

    uint64_t getTrackingMSUntilRemove(){ return trackingMSUntilRemove;};
    void setTrackingMSUntilRemove(uint64_t trackingMSUntilRemove){ this->trackingMSUntilRemove = trackingMSUntilRemove;};

protected:

    void threadedFunction();
    void update(ofEventArgs & a);
    void convert( ofPixels & pix, image_t* image );
    ofPixels convert( image_t & image );
    
	// list1 *options1;
//	char **names;

    string cfgfile;
    string weightfile;
    string nameslist;

    bool loaded = false;
    bool labelsAvailable = false;
    bool bDetectionIsBusy = false;
    bool bHasNewData = false;

    int trackingMaxDist = 200;
    int trackingFrameHistory = 40;
    bool trackingChangeHistory = true;
    uint64_t trackingMSUntilRemove = 500;
    uint skippedFrameCounter = 0;

    std::shared_ptr<Detector> detector;
    std::vector<std::string> obj_names;


    track_kalman_t trackKalman;


    struct AnalyseObject {
        ofPixels pixels;
        float threshold;
        float maxOverlap;
        std::vector<bbox_t> result_vec;
    };
    std::vector<std::string> objectsNamesFromFile(std::string const filename);
    ofThreadChannel<AnalyseObject> toAnalyze;
    ofThreadChannel<AnalyseObject> analyzed;

    DetectedObjects detectedObjects;

};
