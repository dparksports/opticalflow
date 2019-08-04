#include "ReadImages.h"
#pragma once

ReadImages::ReadImages(std::string basepath, const std::string imagename, const std::string suffix)
{
    / / Unify the folder separator in the path to ' / ' (can not need this for loop, will be automatically unified, the command used by the statement reference C + + Primer)
    For (auto &c : basepath)
    {
        If (c == '\\')
        {
            c = '/';
        }
    }

    _imgSource._basepath = basepath + "/"; //here '/' instead of '\\'
    _imgSource._imagename = imagename; //image name (without number)
    _imgSource._suffix = suffix; //image extension

}

/ / Read a single picture
/ / Input: imgId, generally read into the sequence picture, the picture has a number
Cv::Mat ReadImages::loadImage(int imgId)
{
    / / Turn the picture number into a good string
    Std::stringstream ss;
    Std::string imgNum;
    Ss << imgId;
    Ss >> imgNum;

    / / Get the complete absolute path of the picture
    Std::string path = _imgSource._basepath + _imgSource._imagename + imgNum + _imgSource._suffix;

    Cv::Mat img = cv::imread(path,0);

    Return img;
}