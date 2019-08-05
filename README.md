# ofxYoloSolo

ofxYoloSolo is a openFrameworks wrapper for darknet's Yolo Rea-Time Object Detection.

This addon is based on the superb ofxDarknet addon (thanks a lot!) which is using Darknet.

Darknet is an open source neural network framework written in C and CUDA. It is fast, easy to install, and supports CPU and GPU computation. http://pjreddie.com/darknet/

ofxYoloSolo is a stripped down version of ofxDarknet with the sole purpose of real-time object detection.

## Features

### YOLO: Real-Time Object Detection (http://pjreddie.com/darknet/yolo/)

Darknet comes with two pre-trained models for this task. Additionally each has a smaller (and faster) but therefore less accurate version:

MS COCO dataset (80 different classes)
* yolo.cfg & [yolo.weights](http://pjreddie.com/media/files/yolo.weights) (256 MB COCO-model)
* tiny-yolo.cfg & [tiny-yolo.weights](http://pjreddie.com/media/files/tiny-yolo.weights) (60 MB COCO-model)

```
	std::string cfgfile = ofToDataPath( "cfg/tiny-yolo.cfg" );
	std::string weightfile = ofToDataPath( "tiny-yolo.weights" );
	std::string nameslist = ofToDataPath( "cfg/names.list" );
	darknet.init( cfgfile, weightfile, nameslist );
```

Pascal VOC dataset (20 different classes)
* yolo-voc.cfg & [yolo-voc.weights](http://pjreddie.com/media/files/yolo-voc.weights) (256 MB VOC-model)
* tiny-yolo-voc.cfg & [tiny-yolo-voc.weights](http://pjreddie.com/media/files/tiny-yolo-voc.weights) (60 MB VOC-model)

```
	std::string cfgfile = ofToDataPath( "cfg/tiny-yolo-voc.cfg" );
	std::string weightfile = ofToDataPath( "tiny-yolo-voc.weights" );
	std::string nameslist = ofToDataPath( "cfg/voc.names" );
	darknet.init( cfgfile, weightfile, nameslist );
```

YOLO2 with 9000 classes

* yolo9000.cfg & [yolo9000.weights](http://pjreddie.com/media/files/yolo9000.weights) (190MB COCO-model)

```
	std::string datacfg = ofToDataPath( "cfg/combine9k.data" );
	std::string cfgfile = ofToDataPath( "cfg/yolo9000.cfg" );
	std::string weightfile = ofToDataPath( "yolo9000.weights" );
	darknet.init( cfgfile, weightfile, datacfg );
```

```
	float thresh = 0.25;
	std::vector< detected_object > detections = darknet.yolo( image.getPixelsRef(), thresh );

	for( detected_object d : detections )
	{
		ofSetColor( d.color );
		glLineWidth( ofMap( d.probability, 0, 1, 0, 8 ) );
		ofNoFill();
		ofDrawRectangle( d.rect );
		ofDrawBitmapStringHighlight( d.label + ": " + ofToString(d.probability), d.rect.x, d.rect.y + 20 );
	}
```

![YOLO2](https://raw.githubusercontent.com/mrzl/ofxDarknet/master/images/yolo2.jpg)



## Setup

### Windows

-- Note: this addon hasn't been tested on windows --

Install the dependencies for building darknet on Windows 10:
* [Visual Studio 2015 (Community)](https://www.microsoft.com/download/details.aspx?id=48146)
* [CUDA 10.1 64bit](https://developer.nvidia.com/cuda-downloads)

There are some more necessary steps that don't work with the OF project generator:

* Compile as Debug or Release in x64 mode
* Within VS2015 Solution Explorer, rightclick on the generated project -> Build Dependencies -> Build Customizations -> Tick CUDA 8.0
* Copy pthreadVC2.dll from ofxDarknet\libs\3rdparty\dll\x64 to your applications bin folder

### OSX

-- Note: this addon hasn't been tested on windows --

First make sure to install [CUDA 10.1 64bit](https://developer.nvidia.com/cuda-downloads) (Driver & Toolkit). CUDA requires an NVIDIA graphics card and a reasonably recent Mac OS.

After that, projects should compile fine from the Project Generator. Make sure to download the necessary weights (links can be found [here](http://pjreddie.com/darknet/yolo/) and include the required cfg files (found in the examples) in any app that opens them.

## Building the library from source

If you want to make changes to the darknet lib, you can build it from source with cmake. `cd` into `libs/darknet/cMake/` and then run:

    cmake .
    make

Note, you need to have CUDA and OpenCV installed on your system first.


## Credits

* Original Code: https://github.com/pjreddie/darknet
* cpp version of darknet: https://github.com/AlexeyAB/darknet/
* Original addon: https://github.com/schwittlick/ofxDarknet
