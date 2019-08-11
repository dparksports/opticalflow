
#include "iostream"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
const char* window_name = "Gaussian pyramid";

using namespace std;
static string current_filename;
Mat filenameMat() {
    string filename;
    cin >> filename;
    filename = "/media/pose/HFS16/190630/" + filename;
    current_filename = filename;
//    Mat frame = imread( filename, IMREAD_COLOR );
    Mat frame = imread( filename, IMREAD_GRAYSCALE);
    return frame;
}

int main( int argc, char** argv ) {
    Mat src = filenameMat();
    if(src.empty()){
        return -1;
    }

    for(;;) {
        imshow( window_name, src );
        char c = (char)waitKey(0);
        if( c == 27 ) {
            break;
        }
        else if( c == 'd' ) {
            Size newSize = Size( src.cols/2, src.rows/2 );
            pyrDown( src, src, newSize);

            newSize = Size( src.cols/2, src.rows/2 );
            pyrDown( src, src, newSize);

            newSize = Size( src.cols/2, src.rows/2 );
            pyrDown( src, src, newSize);

            cout << src.size() << endl;
            cout << src << endl;
        }
    }
    return 0;
}
