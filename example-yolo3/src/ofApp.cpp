#include "ofApp.h"

void ofApp::setup() 
{

	ofSetLogLevel(OF_LOG_VERBOSE);
	// std::string cfgfile = ofToDataPath( "cfg/yolo9000.cfg" );
	// std::string weightfile = ofToDataPath( "yolo9000.weights" );
	// std::string nameslist = ofToDataPath( "cfg/9k.names" );

	// std::string cfgfile = ofToDataPath( "cfg/yolo.cfg" );
	// std::string weightfile = ofToDataPath( "yolo.weights" );
	// std::string nameslist = ofToDataPath( "cfg/names.list" );

	// std::string cfgfile = ofToDataPath( "cfg/yolo-voc.cfg" );
	// std::string weightfile = ofToDataPath( "yolo-voc.weights" );
	// std::string nameslist = ofToDataPath( "cfg/voc.names" );	

    std::string  nameslist = "data/cfg/coco.names";
    std::string  cfgfile = "data/cfg/yolov3.cfg";
    std::string  weightfile = "data/yolov3.weights";


	yolo.init( cfgfile, weightfile, nameslist );
#ifdef USE_WEBCAM
	video.setDeviceID( 0 );
	video.setDesiredFrameRate( 30 );
	video.initGrabber( 640, 480 );
	ofSetWindowShape(video.getWidth()*scale,video.getHeight()*scale);
#else
	auto videoFile = "/home/nono/Desktop/videos/test-convert.mp4";
//    auto videoFile = "/home/um/Desktop/videos/test-convert.mp4";
//    auto videoFile = "/home/nono/Documents/workspaces/openframeworks/of_v0.10.1_linux64gcc5_release/apps/umbrellium/colas-crossing-cv/bin/data/videos/MVI_9284_short.mp4";
//    auto videoFile = "/home/nono/Desktop/videos/1-nicole-jaywalk-then-cross-back.avi";
//    auto videoFile = "/home/nono/Downloads/MVI_9290_short.mp4";
//    auto videoFile = "/home/nono/Downloads/scrutton_st_02.mp4";
    player.load(videoFile);
//    player.setSpeed(.5);
//	player.setLoopState(OF_LOOP_NORMAL);
    player.setLoopState(OF_LOOP_NONE);
	player.play();
	ofSetWindowShape(player.getWidth()*scale,player.getHeight()*scale);
#endif

}

void ofApp::update()
{
#ifdef USE_WEBCAM
	video.update();
#else
	player.update();
#endif
}

void ofApp::draw()
{
    // detected objects with confidence < threshold are omitted
//	float thresh = ofMap( ofGetMouseX(), 0, ofGetWidth(), 0, 1 );
    float thresh = 0.29;
    // if a detected object overlaps >maxOverlap with another detected
    // object with a higher confidence, it gets omitted
    float maxOverlap = 0.25;
    
	ofSetColor( 255 );
	bool isNewFrame = false;
	ofPixels pix;
#ifdef USE_WEBCAM
	video.draw( 0, 0 , ofGetWidth(), ofGetHeight());
	if(video.isFrameNew()){
		isNewFrame = true;
		pix = video.getPixels();
		currentFrame++;
	}
#else
	player.draw( 0, 0 , ofGetWidth(), ofGetHeight());
	if(currentFrame!=player.getCurrentFrame()){
        currentFrame=player.getCurrentFrame();
        isNewFrame = true;
        pix = player.getPixels();
    }

	#endif

	if( isNewFrame ) {
        yolo.yolo_nono(pix, thresh);
    }
    {
        float scale = .5;
		std::map< unsigned int, DetectedObject > detections = yolo.getDetectedObjects().objects;// = darknet.yolo_nono( pix, thresh, maxOverlap );
		ofNoFill();
		ofPushMatrix();
		ofScale(scale,scale,1);
		for( const auto pair : detections )
		{
		    const auto d = pair.second;
			ofNoFill();
            ofSetColor( ofColor::bisque );
            ofDrawRectangle( d.rectPredicted );
            ofSetColor( ofColor::azure );
            ofDrawRectangle( d.rect );
			ofDrawBitmapStringHighlight( ofToString(d.id) + "    " + d.label + ": " + ofToString(d.probability,3), d.rect.x+3, d.rect.y + 18 );
            glLineWidth( 6 );
			ofDrawLine(glm::vec2(d.rect.x,d.rect.getBottom()-3), glm::vec2(d.rect.x+d.rect.getWidth()*d.probability,d.rect.getBottom()-3));
            glLineWidth( 1 );

            // optionally, you can grab the 1024-length feature vector associated
            // with each detected object
//            vector<float> & features = d.features;
		}
		ofPopMatrix();

		if(yolo.isLoaded()){
            ofDrawBitmapStringHighlight("Detections: " + ofToString(detections.size()), 20, 30 );
            ofDrawBitmapStringHighlight("Threshold: " + ofToString(thresh,2), 20, 50 );
        }else{
            ofDrawBitmapStringHighlight("Loading Yolo...", 20, 30 );
        }

	}
}
