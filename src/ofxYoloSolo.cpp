#include "ofxYoloSolo.h"

ofxYoloSolo::ofxYoloSolo()
{
    loaded = false;
    labelsAvailable = false;
    ofLogVerbose("ofxYoloSolo::ofxYoloSolo") << "OpenCV version : " << CV_VERSION << endl;
}

ofxYoloSolo::~ofxYoloSolo()
{
	toAnalyze.close();
	analyzed.close();
	waitForThread(true);
    ofRemoveListener(ofEvents().update, this, &ofxYoloSolo::update);
}

void ofxYoloSolo::init( std::string cfgfile, std::string weightfile, std::string nameslist )
{
    this->cfgfile = cfgfile;
    this->weightfile = weightfile;
    this->nameslist = nameslist;

	ofAddListener(ofEvents().update, this, &ofxYoloSolo::update);
    startThread();
    ofLogVerbose("ofxYoloSolo::init") << "All done.";
}
//int tempCounter = 0;
void ofxYoloSolo::update(ofEventArgs & a){
    AnalyseObject ao;
    auto timestamp = ofGetElapsedTimeMillis();
	if(analyzed.tryReceive(ao)) {
        trackKalman.correct(ao.result_vec);
        for(auto res: ao.result_vec){

            auto rect = ofRectangle(res.x,res.y,res.w,res.h);

            // Don't create new objects on top of old ones... most likely an error
            bool discardObject = false;
            if(detectedObjects.objects.count(res.track_id) == 0){
                for(auto objComp: detectedObjects.objects){
                    auto overlapRect = rect.getIntersection(objComp.second.rect);
                    float overlap = (overlapRect.getWidth()*overlapRect.getHeight()) / (objComp.second.rect.getWidth()*objComp.second.rect.getHeight());
                    if(ao.maxOverlap < overlap && res.track_id != objComp.second.id ){
//                        if(overlap>0) ofLog() << "Discard # " << res.track_id << " because of existing # " << objComp.second.id << " overlap : " << overlap;
                        discardObject = true;
                        break;
                    }
                }
            }
            if(discardObject) continue;

            DetectedObject dectObj;
            if(labelsAvailable){
                dectObj.label = obj_names[res.obj_id];
            }else{
                dectObj.label = "N/A";
            }
            dectObj.rectPredicted = dectObj.rect = rect;
            dectObj.probability = res.prob;
            dectObj.id = res.track_id;
            dectObj.lastDetected = timestamp;
//            if(detectedObjects.objects.count(res.track_id) == 0){
//                tempCounter ++;
////                ofLog() << "Creating new Object #" << res.track_id << "     tempCounter: " << tempCounter;
//            }
            detectedObjects.objects[res.track_id] = (dectObj);
        }
        bHasNewData = true;
    }else {
        auto result_vec = trackKalman.predict();
        for(auto res: result_vec){
            if(detectedObjects.objects.count(res.track_id)){
                detectedObjects.objects[res.track_id].rectPredicted = ofRectangle(res.x,res.y,res.w,res.h);
            }
        }
    }
    std::vector<unsigned int> idsToRemove;
    for(const auto obj: detectedObjects.objects) {
        if(timestamp-obj.second.lastDetected>trackingMSUntilRemove){
            idsToRemove.push_back(obj.first);
        }
    }
    for(auto id: idsToRemove){
        detectedObjects.objects.erase(id);
    }
}

// std::vector< detected_object > ofxYoloSolo::yolo_nono( ofPixels & pix, float threshold /*= 0.24f */, float maxOverlap /*= 0.5f */ )
void ofxYoloSolo::yolo_nono( ofPixels & pix, float threshold /*= 0.24f */, float maxOverlap /*= 0.5f */ )
{
    if(!loaded) return;
    if(!bDetectionIsBusy){
        AnalyseObject obj;
        obj.pixels = pix;
        obj.threshold = max(0.01f,threshold);
        obj.maxOverlap = maxOverlap;
        toAnalyze.send(obj);
    }else{
        skippedFrameCounter++;
        ofLogVerbose("ofxYoloSolo::yolo_nono") << "Yolo busy. Skipping frame. [" << skippedFrameCounter<<"]";
    }
}

void ofxYoloSolo::convert( ofPixels & pix, image_t* image_ptr )
{
    unsigned char *data = ( unsigned char * ) pix.getData();
    int h = pix.getHeight();
    int w = pix.getWidth();
    int c = pix.getNumChannels();
    int step = w * c;

//    std::shared_ptr<image_t> image_ptr(new image_t, [](image_t *img) { if (img->data) free(img->data); delete img; });
    // image_ptr->data = convert( pix2 ).data;
    if(w*h*c != image_ptr->w*image_ptr->h*image_ptr->c){
        image_ptr->w = w;
        image_ptr->h = h;
        image_ptr->c = c;
        image_ptr->data = (float *)calloc(w*h*c, sizeof(float));
    }

    // image im = make_image( w, h, c );
    int i, j, k, count = 0;;

    for( k = 0; k < c; ++k ) {
        for( i = 0; i < h; ++i ) {
            for( j = 0; j < w; ++j ) {
                image_ptr->data[ count++ ] = data[ i*step + j*c + k ] / 255.;
            }
        }
    }
}


ofPixels ofxYoloSolo::convert( image_t & im )
{
	unsigned char *data = ( unsigned char* ) calloc( im.w*im.h*im.c, sizeof( char ) );
	int i, k;
	for( k = 0; k < im.c; ++k ) {
		for( i = 0; i < im.w*im.h; ++i ) {
			data[ i*im.c + k ] = ( unsigned char ) ( 255 * im.data[ i + k*im.w*im.h ] );
		}
	}

	ofPixels pix;
	pix.setFromPixels( data, im.w, im.h, im.c );
	return pix;
}



std::vector<std::string> ofxYoloSolo::objectsNamesFromFile(std::string const filename) {
    std::ifstream file(filename);
    std::vector<std::string> file_lines;
    if (!file.is_open()) return file_lines;
    for(std::string line; getline(file, line);) file_lines.push_back(line);
    return file_lines;
}



void ofxYoloSolo::threadedFunction(){
    AnalyseObject objToAnalyse;
    image_t imageToAnalyse;
    imageToAnalyse.w = 0;
    imageToAnalyse.h = 0;
    imageToAnalyse.c = 0;
    imageToAnalyse.data = 0;

    detector = std::make_shared<Detector>(cfgfile, weightfile);
    loaded = true;
    obj_names = objectsNamesFromFile(nameslist);
    labelsAvailable = obj_names.size() > 0;
    while(toAnalyze.receive(objToAnalyse)){

        bDetectionIsBusy = true;
		int originalWidth = objToAnalyse.pixels.getWidth();
		int originalHeight = objToAnalyse.pixels.getHeight();
		ofPixels  pix2( objToAnalyse.pixels );
	    if (pix2.getImageType() != OF_IMAGE_COLOR) {
	        pix2.setImageType(OF_IMAGE_COLOR);
	    }
	    if( pix2.getWidth() != detector->get_net_width() && pix2.getHeight() != detector->get_net_height() ) {
	        pix2.resize( detector->get_net_width(), detector->get_net_height() );
	    }

        convert( pix2, &imageToAnalyse );

	    std::vector<bbox_t> result_vec;
	    result_vec = detector->detect_resized(imageToAnalyse, originalWidth, originalHeight, objToAnalyse.threshold, true);  // true
        result_vec = detector->tracking_id(result_vec, trackingChangeHistory, trackingFrameHistory, trackingMaxDist);
        objToAnalyse.result_vec = result_vec;
        bDetectionIsBusy = false;

#if __cplusplus>=201103
        analyzed.send(std::move(objToAnalyse));
#else
        analyzed.send(objToAnalyse);
#endif
	}
}

bool ofxYoloSolo::hasNewData() {
    bool ret = bHasNewData;
    bHasNewData = false;
    return ret;
}

DetectedObjects ofxYoloSolo::getDetectedObjects(){
    return detectedObjects;
}