#pragma once
#include <opencv2/opencv.hpp>
//#include <sstream>

Typedef struct ImgSource
{
    Std::string _basepath = ""; //Image base path
    Std::string _imagename = ""; //image name (without number)
    Std::string _suffix = ""; //Image suffix
}ImageSource;

Class ReadImages
        {
                Public:
                ReadImages() {}
        ReadImages(const std::string basepath, const std::string imagename, const std::string suffix);

        Cv::Mat loadImage(int imgId); //Read in the image
        Private:
        ImageSource _imgSource; //Image path information
        Cv::Rect _roi;
        };