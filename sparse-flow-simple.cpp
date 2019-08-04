#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;

static string current_filename;
Mat filenameMat() {
    string filename;
    cin >> filename;
    filename = "/home/pose/Pictures/s190401/" + filename;
    current_filename = filename;
    Mat frame = imread( filename, IMREAD_COLOR );
    return frame;
}

int main(int argc, char **argv) {
    bool trackFeatures = true;

    // Create some random colors
    vector<Scalar> colors;
    RNG rng;
    for(int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r,g,b));
    }
    Mat old_gray;
    vector<Point2f> p0, p1;
    // Take first frame and find corners in it
    Mat old_frame = filenameMat();

    cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
    goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
    // Create a mask image for drawing purposes
    Mat mask = Mat::zeros(old_frame.size(), old_frame.type());

    // Take first frame and find corners in it
    while(true){
        Mat frame_gray;
        Mat frame = filenameMat();
        if (frame.empty())
            break;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);

        if (p0.empty()) {
            p0.clear();
            p1.clear();
            goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
        } else {
            // calculate optical flow
            vector<uchar> status;
            vector<float> err;
            TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
            calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(15,15), 2, criteria);

            vector<Point2f> good_new;
            for(uint i = 0; i < p0.size(); i++)
            {
                // Select good points
                if(status[i] == 1) {
                    good_new.push_back(p1[i]);
                    // draw the tracks
                    line(mask,p1[i], p0[i], colors[i], 2);
                    circle(frame, p1[i], 5, colors[i], -1);
                }
            }

            // Now update the previous frame and previous points
            old_gray = frame_gray.clone();
            p0 = good_new;
        }

        cv::putText(frame,
                    current_filename,
                    cv::Point(5,5 * 10), // Coordinates
                    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                    1.0, // Scale. 2.0 = 2x bigger
                    cv::Scalar(255,255,255), // BGR Color
                    1 // Line Thickness (Optional)
        );

        Mat img;
        add(frame, mask, img);
        imshow("Frame", img);
        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch( c )
        {
            case 'r':
                trackFeatures = true;
                break;
            case 'c':
                p0.clear();
                p1.clear();
                break;
            case 'n':
                break;
        }
    }
}
