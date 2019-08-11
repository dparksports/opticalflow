#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <list>

using namespace cv;

const int max_value_H = 360/2;
const int max_value = 255;
const String window_capture_name = "Video Capture";
const String window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 50;
int high_H = max_value_H, high_S = max_value, high_V = 51;
static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}

using namespace std;

string parseTimestamp(string str) {
    stringstream ss(str);
    string index, year, month, date, hour, minute, second;
    int iyear, imonth, idate, ihour, iminute, isecond;


    getline(ss, index, '-');
    getline(ss, index, '-');
    stringstream wordstream1(index);


    getline(ss, year, '-');
    stringstream wordstream2(year);
    wordstream2 >> iyear;

    getline(ss, month, '-');
    stringstream wordstream3(month);
    wordstream3 >> imonth;

    getline(ss, date, '_');
    stringstream wordstream4(date);
    wordstream4 >> idate;

    getline(ss, hour, '-');
    stringstream wordstream5(hour);
    wordstream5 >> ihour;

    getline(ss, minute, '-');
    stringstream wordstream6(minute);
    wordstream6 >> iminute;

    getline(ss, second, '.');
    stringstream wordstream7(second);
    wordstream7 >> isecond;

    string timestamp = month + "/" + date + "-" + hour + ":" + minute + ":" + second;
    return timestamp;
}

string parsePounds(string str) {
    stringstream ss(str);
    string token, year, month, date, hour, minute, second;
    string pound;


    getline(ss, token, ' ');
    stringstream wordstream1(token);

    wordstream1 >> pound;

    return pound;
}

int parseIndex(string str) {
    stringstream ss(str);
    string token;

    getline(ss, token, '-');
    getline(ss, token, '-');
    stringstream wordstream(token);

    int number;
    wordstream >> number;
    return number;
}

static string current_index;
static string current_timestamp;
static string current_filename;

void calculate_average(list<int> list);

Mat filenameMat() {
    string filename, line;
    cin >> line;

//    filename = parsePounds(line);
    current_filename = line;
    string filepath = "/media/pose/HFS630/190630/" + line;
    Mat frame = imread( filepath, IMREAD_COLOR );

    int index = parseIndex(line);
    current_timestamp = parseTimestamp(line);
    current_index = to_string(index);

    cin >> line;
    cin >> line;
    cin >> line;
    cout << line << endl;

    return frame;
}

static int current_average;
static int pcount;
static bool showWinddow = true;
static int hsv_index = 0;

int countHSV(Mat& src) {
    Size newSize = Size( src.cols/2, src.rows/2 );
    pyrDown( src, src, newSize);
//    Size newSize2 = Size( src.cols/2, src.rows/2 );
//    pyrDown( src, src, newSize2);

    int count;
    int HSVcomponent = 2;
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            Vec3b element = src.at<Vec3b>(i,j);
            int value = element[HSVcomponent];
            if (value > 50) {
                count++;
            }
        }
    }

    if (pcount ==  0) {
        pcount = count;
    }

    if (count > 54) {
        int diff = count - pcount;
        diff = abs(diff);

        float boundary = pcount * (1/4.0);
        if (diff > boundary) {
            float percentage = diff / (pcount * 1.0);
            int per = percentage * 100;
            cout << current_filename << "   " << count << "   " << per << " #" << endl;
//        printf("%.1f", percentage);
//        cout << endl;
            showWinddow = true;
        } else {
            hsv_index = hsv_index % 720;
            if (hsv_index == 0) {
                float percentage = diff / (pcount * 1.0);
                int per = percentage * 100;
                cout << current_filename << "   " << count << "   " << per << endl;
            }
            hsv_index++;
        }
    } else {
        hsv_index = hsv_index % 120;
        if (hsv_index == 0) {
            cout << current_filename << "   " << count << "   " << endl;
        }
        hsv_index++;
    }

    pcount = count;
    return count;
}

int main(int argc, char* argv[]) {
    pcount = 0;
    hsv_index = 0;
    showWinddow = true;

    namedWindow(window_capture_name);
    namedWindow(window_detection_name);

    // Trackbars to set thresholds for HSV values
//    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
//    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
//    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
//    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);

    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

    Mat frame, frame_HSV, frame_threshold;
    Mat frameResized;
    while (true) {
        frame = filenameMat();
        if(frame.empty()) {
            break;
        }

        Size newSize = Size( frame.cols/2, frame.rows/2 );
        pyrDown( frame, frame, newSize);
        Size newSize2 = Size( frame.cols/2, frame.rows/2 );
        pyrDown( frame, frame, newSize2);

        // Convert from BGR to HSV colorspace
        cvtColor(frame, frame_HSV, COLOR_BGR2HSV);
        // Detect the object based on HSV Range Values
        inRange(frame_HSV, Scalar(low_H, low_S, low_V), Scalar(high_H, high_S, high_V), frame_threshold);

        cv::putText(frame,
                    current_index,
                    cv::Point(5,5 * 5), // Coordinates
                    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                    1.0, // Scale. 2.0 = 2x bigger
                    cv::Scalar(255,255,255), // BGR Color
                    1 // Line Thickness (Optional)
        );

        cv::putText(frame,
                    current_timestamp,
                    cv::Point(5,frame.size().height - (5*5)), // Coordinates
                    cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                    1.0, // Scale. 2.0 = 2x bigger
                    cv::Scalar(255,255,255), // BGR Color
                    1 // Line Thickness (Optional)
        );

        int count = countHSV(frame_threshold);
        imshow(window_capture_name, frame);
        imshow(window_detection_name, frame_threshold);


        char key = (char) waitKey(1000 * 60);
        if (key == 'q' || key == 27)
        {
            break;
        }
        else if( key == ' ' ) {
            continue;
        }
        else if( key == 'd' ) {
        }
    }
    return 0;
}
