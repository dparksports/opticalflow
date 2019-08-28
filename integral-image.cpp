#include <iostream> // for standard I/O
#include <string>   // for strings

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    string filepath = "/home/pose/Pictures/street-pano.jpg";
    Mat image = imread( filepath, IMREAD_COLOR );
    if(image.empty()) {
        return 0;
    }

//    Mat grayImage;
//    cvtColor(image, grayImage, COLOR_RGB2GRAY );
//
    Mat image32;
    image.convertTo(image32, CV_32F);

    Mat IntegralImageCV;
    integral(image32, IntegralImageCV);

    const String integral_name = "Integral";
    const String original_name = "Original";
    namedWindow(integral_name);
    namedWindow(original_name);
    imshow(original_name, image);
    imshow(integral_name, IntegralImageCV);
    int delay = 1000 * 60 * 2;  // 1000 * 60
    char key = (char) waitKey(delay);
    if (key == 'q' || key == 27)
    {
        return 0;
    }
    return 0;
}
