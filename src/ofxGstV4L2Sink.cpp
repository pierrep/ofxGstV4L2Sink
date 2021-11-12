/*
 * ofxGstV4L2Sink.cpp
 *
 *  Created on: 04/03/2012
 *      Author: arturo
 */

#include "ofxGstV4L2Sink.h"
#include "ofConstants.h"
#include "ofMain.h"
#include <gst/app/gstappsink.h>
#if GST_VERSION_MAJOR == 0
#include <gst/app/gstappbuffer.h>
#endif

ofxGstV4L2Sink::ofxGstV4L2Sink()
{
}

//--------------------------------------------------------------
ofxGstV4L2Sink::~ofxGstV4L2Sink()
{
    close();
}

//--------------------------------------------------------------
void ofxGstV4L2Sink::close()
{
    if (gstSrc) {
        gst_app_src_end_of_stream(GST_APP_SRC(gstSrc));
    }
    gst.close();
}

//--------------------------------------------------------------
void ofxGstV4L2Sink::setup(int _width, int _height)
{
    // try and automatically find the loopback device, only works if there is one
    string deviceName = ofSystem("ls /sys/devices/virtual/video4linux");
    deviceName.erase(std::remove(deviceName.begin(), deviceName.end(), '\n'),deviceName.end());
    if(!deviceName.empty()) {
        ofLogNotice() << "Found V4L2 loopback device: /dev/" << deviceName;
        int dev = deviceName.back() - 48;
        setup(_width,_height,dev);
    } else {
        ofLogError() << "Failed to find a V4L2 loopback device";
    }
}

//--------------------------------------------------------------
void ofxGstV4L2Sink::setup(int _width, int _height, int dev)
{
    width = _width;
    height = _height;
    device = dev;
    string w = ofToString(width);
    string h = ofToString(height);
    string pipeline;

#if GST_VERSION_MAJOR == 0
    string appsrc = "appsrc  name=video_src is-live=true do-timestamp=true ! "
                    "video/x-raw-rgb,width="
        + w + ",height=" + h + ",depth=24,bpp=24,framerate=30/1,endianness=4321,red_mask=16711680, green_mask=65280, blue_mask=255 ! queue ! ";
    string videorate; //  = "videorate ! video/x-raw-rgb,depth=24,bpp=24,framerate=25/2,endianness=4321,red_mask=16711680, green_mask=65280, blue_mask=255 ! ";
    string videoscale; // = "videoscale ! video/x-raw-rgb,width=" + ofToString(width) + ",height=" + ofToString(height) + ",depth=24,bpp=24,endianness=4321,red_mask=16711680, green_mask=65280, blue_mask=255 ! ";
    string colorspace = " ffmpegcolorspace ! video/x-raw-yuv,width=" + ofToString(width) + ",height=" + ofToString(height) + " ! ";
    pipeline = appsrc + videorate + videoscale + colorspace + " v4l2sink device=/dev/video" + ofToString(device);

#else    
    string incaps = "video/x-raw, format=RGB, width=" + w + ", height=" + h;
    string outcaps = "video/x-raw, format=YUY2, width=" + w + ", height=" + h;
    string appsrc = "appsrc name=video_src is-live=true do-timestamp=true caps=\"" + incaps + "\" ! queue !";
    string colorspace = " videoconvert ! " + outcaps + " ! ";
    pipeline = appsrc + colorspace + " v4l2sink name=video_sink device=/dev/video" + ofToString(device);
#endif

    gst.setPipelineWithSink(pipeline, "video_sink", false);
    gstSrc = (GstAppSrc*)gst_bin_get_by_name(GST_BIN(gst.getPipeline()), "video_src");
    if (gstSrc) {
        gst_app_src_set_stream_type(gstSrc, GST_APP_STREAM_TYPE_STREAM);
        g_object_set(G_OBJECT(gstSrc), "format", GST_FORMAT_TIME, NULL);
    }

    pixels.allocate(width, height, OF_IMAGE_COLOR);

    gst.startPipeline();
    gst.play();
}

//--------------------------------------------------------------
bool ofxGstV4L2Sink::update(ofPixels& pixels)
{
    GstBuffer* buffer;
    if(GST_IS_APP_SRC(gstSrc)) {
    #if GST_VERSION_MAJOR == 0
        buffer = gst_app_buffer_new(pixels.getPixels(), width * height * 3, NULL, pixels.getPixels());
    #else
        buffer = gst_buffer_new_wrapped_full(GST_MEMORY_FLAG_READONLY, pixels.getData(), width * height * 3, 0, width * height * 3, pixels.getData(), NULL);
    #endif
        GstFlowReturn flow_return = gst_app_src_push_buffer(gstSrc, buffer);
        if (flow_return != GST_FLOW_OK) {
            ofLog(OF_LOG_WARNING, "error pushing buffer");
            return false;
        }
        return true;
    }

    return false;
}
