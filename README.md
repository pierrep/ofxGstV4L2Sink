
This addon turns your OF app into a virtual "Video4Linux2" (V4L2) camera.
It allows you to send the output of an OF application to any other application that supports V4L2 devices.
Example uses are any applications that support a V4L2 camera, e.g. Zoom, Microsoft Teams, etc

It uses the v4l2sink GStreamer element to create a virtual device which can be used as a camera in any application that supports V4L2

## USAGE
To use it just call setup() to create the device, and update() to send the pixels for each frame.

The setup() can automatically find the loopback device if there's only one:
```
virtualCam.setup(1280,720)
```
Otherwise you need to specify the number of the device. If you have multiple video loopback devices, and want to use /dev/video4 then use the number '4' as the final parameter for setup:
```
virtualCam.setup(1280,720,4);
```
then pass your pixels to the loopback device:
```
virtualCam.update(myPixels);
```

## SETUP THE LOOPBACK DEVICE
To install the loopback device on Ubuntu:
```
sudo apt install v4l2loopback-dkms
```

To create the loopback video device, then run
```
sudo modprobe v4l2loopback card_label="myVirtualCam"
```

If you want your camera to appear in a chrome/chromium application, run this instead:
```
sudo modprobe v4l2loopback card_label="myVirtualCam" exclusive_caps=1
```


