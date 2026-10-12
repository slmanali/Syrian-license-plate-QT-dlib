# Syrian License Plate Recognition

A C++ / Qt desktop application for recognizing Syrian vehicle license plates from
video files and IP cameras. It combines OpenCV image processing, dlib plate and
character detectors, a YOLO vehicle detection integration, and an Arabic interface
with three video panels.

**Project status:** this is a legacy Windows application being improved
incrementally. Video capture now uses one reusable `CaptureThread` class with
independent instances for each panel. The full application still needs external
models, databases, resources, and local build configuration. A fresh clone is not
yet a complete runnable installation.

## Features

- Three video panels with separate open, pause/resume, and stop controls.
- Local `.avi` / `.mp4` playback through OpenCV and RTSP capture through LibVLC.
- Plate, digit, and region recognition using dlib detectors, with OpenCV
  preprocessing and a YOLO integration in the first panel.
- Recognition history stored in SQLite, with saved plate and scene images.
- History filtering and alert logic that queries a separate stolen-vehicle
  database.
- Legacy serial-port trigger and vehicle-details web-service integrations.

The configured region detectors cover **Damascus, Rural Damascus, and Daraa**.
The interface lists additional regions, but corresponding recognition models are
not configured in `init_region()`.

## Project layout

| Path | Purpose |
| --- | --- |
| [Plate_Rec.pro](Plate_Rec.pro) | qmake application target and dependency paths |
| [main.cpp](main.cpp) | Qt application entry point |
| [mainwindow.cpp](mainwindow.cpp), [mainwindow.h](mainwindow.h) | UI coordination, recognition, database access, and integrations |
| [capturethread.cpp](capturethread.cpp), [capturethread.h](capturethread.h) | Reusable OpenCV / LibVLC capture worker |
| [ip_camera.cpp](ip_camera.cpp), [ip_camera.ui](ip_camera.ui) | Camera connection dialog |
| [form.cpp](form.cpp), [form.ui](form.ui) | Plate-result display |
| [include/](include/), [src/](src/) | Detector wrappers, YOLO interface, utilities, and legacy binaries |
| [httprequestworker.cpp](httprequestworker.cpp) | HTTP request helper for the vehicle-details service |
| [tests/](tests/) | Standalone capture regression tests |

## Dependencies

The application project references the following stack. These are the existing
build assumptions, not a claim that the full application has been verified with
current dependency releases.

| Dependency | Requirement / existing configuration |
| --- | --- |
| Qt 5 and qmake | Core, GUI, Widgets, Network, SQL, and Serial Port modules; SQLite driver at runtime |
| C++ compiler | A compatible MSVC toolchain for the Windows libraries; full detector code uses C++14 `std::make_unique` |
| OpenCV | `.pro` links OpenCV **3.4.1**, x64 / `vc14`; detector headers also reference the contrib tracking module |
| dlib | Paths reference **19.10**, including its source tree and a prebuilt MSVC library |
| LibVLC | Development headers, import libraries, runtime libraries, and VLC plugins; the capture target has been tested with LibVLC 3 |
| YOLO library | A compatible `yolo_cpp_dll.lib` and runtime DLL matching the included interface |

Keep compiler, architecture, debug/release mode, and library versions consistent.
The older DLLs under `src/` include OpenCV 3.2 binaries and are not a complete
deployment bundle for the OpenCV 3.4.1 project configuration.

## Build the application

1. Clone the repository:

   ```sh
   git clone https://github.com/slmanali/Syrian-license-plate-QT-dlib.git
   cd Syrian-license-plate-QT-dlib
   ```

2. Install the dependencies above and open `Plate_Rec.pro` in Qt Creator with a
   compatible Windows / MSVC kit.

3. Adapt `Plate_Rec.pro` to your machine:
   - Replace the absolute `F:\...` include and library paths.
   - Point the project include path at this repository's `include/` directory.
   - Update the external `dlib/all/source.cpp` path and review the dlib library
     entry for your chosen build.
   - Enable C++14 for the full application, replacing `CONFIG += ... c++11` with
     the equivalent `c++14` configuration. The standalone capture target only
     requires C++11.
   - Align OpenCV library names in the project and the `#pragma comment(lib, ...)`
     entries in the YOLO detector headers. The legacy debug entries also refer
     to OpenCV 3.3.
   - Restore `release/imageformats/rcc.qrc` and its image assets, or update the
     resource configuration and UI icon references. These files are not included.
   - Correct the `bulid/release/Cars.sqlite` distribution-file entry to match your
     restored database location.

4. Run qmake and build in Qt Creator. Once the local configuration is complete,
   an equivalent release build from a configured Qt / MSVC command prompt is:

   ```bat
   mkdir build
   cd build
   qmake ..\Plate_Rec.pro CONFIG+=release
   nmake
   ```

5. Deploy matching Qt plugins and third-party runtime libraries, then provide the
   runtime files below. Set the application's working directory to the executable
   directory so relative model and image paths resolve consistently.

The full application has not been rebuilt or validated as part of the capture
refactor. Restoring the original dependencies and assets may expose further build
or compatibility issues.

## Runtime files

The application loads dlib models and databases from the executable directory.
YOLO paths and saved-image paths are relative to the working directory. Use the
same directory for both when running the existing code.

| File or directory | Purpose |
| --- | --- |
| `1.svm`, `6.svm` | Rectangular and square plate detectors |
| `svm_detectors/object_detector_0.svm` through `object_detector_9.svm` | Ten digit detectors, all inside `svm_detectors/` |
| `object_detector_0.dat` through `object_detector_9.dat` | Ten dlib neural-network digit models |
| `object_detector_damascus.svm`, `object_detector_sub.svm`, `object_detector_deraa.svm` | Region detectors |
| `mmod_front_and_rear_end_vehicle_detector.dat` | Serialized vehicle detector and shape predictor |
| `net/yolo-voc.cfg`, `net/yolo-voc.weights`, `net/voc.names` | YOLO configuration, weights, and class names |
| `Cars.sqlite` | Recognition history, using an existing `Plate` table |
| `Stolen_Car.sqlite` | Database queried by the alert logic, also using a `Plate` table |
| `info.txt` | Camera entries used by the camera dialog's file-based credential check |
| `type.txt` | Optional web-service and recognition-cooldown configuration |
| `database/` | Output directory for plate and scene images; created by the application |

**The model files, YOLO configuration/weights, databases, and database schema are
not included in this repository.** Obtain compatible assets from the original
deployment before running recognition. The code does not create the required
database tables; empty SQLite files are insufficient.

All listed dlib models are loaded during `MainWindow` construction, including the
neural-network models, even when a particular recognition path does not use them.
Missing or incompatible models can therefore prevent startup.

### Camera configuration

The default camera-dialog page checks the entered credentials against `info.txt`
beside the executable. Each camera entry consists of three consecutive lines:

```text
192.0.2.10
CAMERA_USERNAME
CAMERA_PASSWORD
```

Replace the example with your camera's host and credentials. Enter the same host,
username, and password in the dialog, together with a positive output width and
height. A separate legacy dialog page still uses a hard-coded credential check.

The UI constructs these RTSP URLs in `MainWindow::done()`, `done1()`, and `done2()`:

| Panel | URL format |
| --- | --- |
| 1 | `rtsp://<username>:<password>@<host>:554` |
| 2 and 3 | `rtsp://<username>:<password>@<host>/MediaInput/h264` |

Adjust those functions if your camera uses another port or stream path.

### Optional `type.txt`

The application reads three lines:

1. `yes` enables the additional vehicle-details UI; any other value disables it.
2. The host, optionally with a port, of the existing vehicle-details service. The
   code adds `http://` and a fixed `/MCRS/api/clearance/all` endpoint.
3. The recognition cooldown in seconds used by the first panel's duplicate-entry
   handling.

That service is external to this repository. Without `type.txt`, the additional
details UI is disabled and the cooldown defaults to zero.

## Using the application

After restoring the build and runtime files:

1. Launch `Plate_Rec` with its working directory set as described above.
2. On a video panel, select the folder button for an `.avi` / `.mp4` file, or the
   camera button to configure an IP camera.
3. Choose the panel's display mode **before opening the source**. Checking
   **إظهار** selects preview-only playback; leaving it unchecked selects the
   recognition handler.
4. Use that panel's pause/resume and stop buttons to control its capture session.
5. Use the **السجل** history tab to inspect and filter saved records.

Recognition behavior differs between panels:

| Panel | Recognition trigger |
| --- | --- |
| 1 | Incoming frames, optionally limited by the interval control and duplicate-entry cooldown |
| 2 | The legacy serial obstacle-sensor logic and configured frame count |
| 3 | The manual trigger button |

For an initial recognition check without the serial hardware, start with panel 1.
The legacy recognition routines assume input frames wider than 1,000 pixels in
several preprocessing paths; smaller inputs need further handling in the code.

## Capture architecture

Each panel owns a separate instance of [CaptureThread](capturethread.h). Each
instance owns its capture backend, frame buffer, and synchronized control state.
The class supports OpenCV video files/camera indices and LibVLC stream URLs.
Camera-index capture is available through the class API; the current UI exposes
file and IP-camera inputs.

Connect `frameCaptured(cv::Mat)`, `captureError(QString)`, and `finished()` before
calling `start()`. Use `setPaused()` or `togglePause()` to pause, and
`requestStop()` followed by `wait()` before deleting a worker. Create a new worker
when changing sources. Emitted frames own their image data so subsequent capture
does not overwrite frames already queued for the UI.

Recognition still runs in `MainWindow` slots on the GUI thread, so expensive
processing can affect interface responsiveness. File capture uses a fixed 40 ms
interval rather than following each file's frame rate. Shutdown is cooperative;
a blocking OpenCV camera `open()` or `read()` can delay it.

## Capture tests

The capture class has a standalone QtTest target that does not need the recognition
models, databases, a camera, or a display server. From the repository root on
Ubuntu with Qt 5, OpenCV 4, and LibVLC 3 development packages:

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

Coverage includes three simultaneous sources, independent pause/resume and stop,
frame ownership, colors and dimensions, end-of-file, invalid inputs, reopening,
and destruction while paused. Both the OpenCV and LibVLC paths use generated local
videos.

The capture refactor passed on Ubuntu 24.04 with Qt 5.15.13, OpenCV 4.6.0, and
LibVLC 3.0.20: **13 QtTest passes**, including initialization and cleanup. This
validates the capture target only; real RTSP cameras, network failures, and
end-to-end plate recognition still require separate validation.

See [tests/README.md](tests/README.md) for capture-specific development notes.

## License

This repository currently has no top-level license file.
