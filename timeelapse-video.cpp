/**
  @file videocapture_basic.cpp
  @brief A very basic sample for using VideoCapture and VideoWriter
  @author PkLab.net
  @date Aug 24, 2016
*/

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <sstream>

using namespace cv;
using namespace std;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}


void writeOutputImage(Mat& image, char *folder, int fileNumber) {
    std::string timestring = currentDateTime();
    cout << timestring << endl;

    std::stringstream buffer;
    buffer << "image" << fileNumber << "-" << timestring << ".jpg";
    std::string path = buffer.str();
    imwrite(path, image );
}

void getLastFrame(cv::VideoCapture& video, cv::Mat& frame)
{
    //Get total number of frames in the video
    //Won't work on live video capture
    const int frames = video.get(CV_CAP_PROP_FRAME_COUNT);

    //Seek video to last frame
    video.set(CV_CAP_PROP_POS_FRAMES,frames-1);

    //Capture the last frame
    video>>frame;

    //Rewind video
    video.set(CV_CAP_PROP_POS_FRAMES,0);
}

int main( int argc, char** argv )
{
    char DEFAULT_OUTPUT_FODLER[] = ".";
    int DEFAULT_TIME_INTERVAL = 1;
    int DEFAULT_CAMERA = 0;
    int currentFileCount = 1;

    char *outputFolder = DEFAULT_OUTPUT_FODLER;
    int timeInterval = DEFAULT_TIME_INTERVAL;
    int cameraId = DEFAULT_CAMERA;

    if(argc>3) outputFolder = argv[3];
    if(argc>1) sscanf(argv[1], "%d", &timeInterval);
    if(argc>2) sscanf(argv[2], "%d", &cameraId);


    Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID + apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl
        << "Press any key to terminate" << endl;
    while (true) {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        // show live and wait for a key with timeout long enough to show images
        writeOutputImage(frame, outputFolder, currentFileCount++);
        if (waitKey(5) >= 0)
            break;
        sleep(timeInterval);
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
