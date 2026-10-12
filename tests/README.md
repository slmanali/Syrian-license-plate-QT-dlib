# CaptureThread tests

This target builds the capture class separately from the recognition application.
It needs Qt 5, OpenCV 3/4 and LibVLC 3. The commands below use Ubuntu's OpenCV 4
packages. No camera, model weights, database or display server is needed.

```sh
sudo apt-get install --no-install-recommends build-essential qtbase5-dev pkg-config \
    libopencv-core-dev libopencv-imgproc-dev libopencv-videoio-dev \
    libvlc-dev vlc-plugin-base vlc-plugin-video-output
mkdir -p build-capture-tests
cd build-capture-tests
qmake ../tests/capturethread_test.pro
make -j2
./capturethread_test
```

The tests generate temporary MJPEG videos and exercise both OpenCV file capture
and the same LibVLC callback path used for RTSP. They check three concurrent
instances with distinct colors and sizes, independent pause/resume and stop,
frame ownership, end-of-file, invalid sources, invalid stream dimensions,
immediate stop, reopening and destruction while paused.

Local file playback through LibVLC does not test RTSP authentication, network
outages or a camera driver's blocking reads. Check those with the actual devices.

# Using the consolidated class

All three UI panels now use `CaptureThread`. Each object has its own source,
player, frame buffer and control state. Connect `frameCaptured` and `finished`
before calling `start()`. `captureError` reports open/decode errors, and
`errorString()` retains the message for the UI's finish handler.

Use `setPaused(bool)` or `togglePause()` for either backend. Use `requestStop()`
and `wait()` before deleting an object; the destructor also performs that cleanup.
Create a new object when changing the source. Control methods synchronize access
without exposing a timer or the OpenCV/VLC objects to the UI. The file capture
interval remains 40 ms; playback-speed and recognition-queue changes are separate
work.

The capture thread stops cooperatively. If an OpenCV camera driver blocks inside
`open()` or `read()`, shutdown waits for that call to return. The API does not claim
a fixed shutdown deadline for physical cameras.

The full application still uses the existing Windows paths in `Plate_Rec.pro` and
requires its external dlib/YOLO libraries, model files, databases and resources.
