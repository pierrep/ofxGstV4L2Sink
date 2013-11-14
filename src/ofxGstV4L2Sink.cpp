/*
 * ofxGstV4L2Sink.cpp
 *
 *  Created on: 04/03/2012
 *      Author: arturo
 */

#include "ofxGstV4L2Sink.h"
#include "ofConstants.h"
#include <gst/app/gstappsink.h>
#if GST_VERSION_MAJOR==0
#include <gst/app/gstappbuffer.h>
#endif

ofxGstV4L2Sink::ofxGstV4L2Sink() {
	// TODO Auto-generated constructor stub

}

ofxGstV4L2Sink::~ofxGstV4L2Sink() {
	// TODO Auto-generated destructor stub
}

void ofxGstV4L2Sink::setup(int w, int h, int dev){
    width = w;
    height = h;
    device = dev;
#if GST_VERSION_MAJOR==0
	string appsrc = "appsrc  name=video_src is-live=true do-timestamp=true ! "
				"video/x-raw-rgb,width=" + ofToString(width) + ",height=" + ofToString(height) + ",depth=24,bpp=24,framerate=30/1,endianness=4321,red_mask=16711680, green_mask=65280, blue_mask=255 ! queue ! ";
#else
	string appsrc = "appsrc  name=video_src is-live=true do-timestamp=true ! "
				"video/x-raw,format=RGB,width=" + ofToString(width) + ",height=" + ofToString(height) + ",depth=24,bpp=24,framerate=30/1,endianness=4321,red_mask=16711680, green_mask=65280, blue_mask=255 ! queue ! ";
#endif
	string videorate;//  = "videorate ! video/x-raw-rgb,depth=24,bpp=24,framerate=25/2,endianness=4321,red_mask=16711680, green_mask=65280, blue_mask=255 ! ";
	string videoscale;// = "videoscale ! video/x-raw-rgb,width=" + ofToString(width) + ",height=" + ofToString(height) + ",depth=24,bpp=24,endianness=4321,red_mask=16711680, green_mask=65280, blue_mask=255 ! ";
#if GST_VERSION_MAJOR==0
	string colorspace = " ffmpegcolorspace ! video/x-raw-yuv,width=" + ofToString(width) + ",height=" + ofToString(height) + " ! ";
#else
	string colorspace = " videoconvert ! video/x-raw,format=YUY2,width=" + ofToString(width) + ",height=" + ofToString(height) + " ! ";
#endif

	string pipeline = appsrc + videorate + videoscale + colorspace + " v4l2sink name=video_sink device=/dev/video" + ofToString(device);

	gst.setPipelineWithSink(pipeline,"video_sink",false);
	gstSrc = (GstAppSrc*)gst_bin_get_by_name(GST_BIN(gst.getPipeline()),"video_src");
	if(gstSrc){
		gst_app_src_set_stream_type (gstSrc,GST_APP_STREAM_TYPE_STREAM);
		g_object_set (G_OBJECT(gstSrc), "format", GST_FORMAT_TIME, NULL);
	}

	pixels.allocate(w,h,OF_IMAGE_COLOR);

	gst.play();
}

void ofxGstV4L2Sink::update(ofPixels & pixels){
  GstBuffer * buffer;
#if GST_VERSION_MAJOR==0
	buffer = gst_app_buffer_new (pixels.getPixels(), width*height*3, NULL, pixels.getPixels());
#else
	buffer = gst_buffer_new_wrapped_full(GST_MEMORY_FLAG_READONLY, pixels.getPixels(), width*height*3, 0, width*height*3, pixels.getPixels(), NULL);
#endif
	GstFlowReturn flow_return = gst_app_src_push_buffer(gstSrc, buffer);
	if (flow_return != GST_FLOW_OK) {
		ofLog(OF_LOG_WARNING,"error pushing buffer");
	}
}
