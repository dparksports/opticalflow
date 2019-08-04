/*
* File name: main.cpp
* Dependent library: opencv3.1.0 or other version
* Test environment: VS2015 x64
* Abstract: The test function of the calcOpticalFlowFeaturesPyrLKSAD function, the test data set is the ETH data set, which contains a total of 107 frames.
* Include file: calcOpticalFlowFeaturesPyrLKSAD.cpp/.h, main test file; ReadImages.cpp/.h read in photo class
* Partial test results: pyrLKSize this parameter compares the impact time: take pyrLKSize = 5, 107 frames take about 3.4 seconds; take 15 for about 6.5 seconds,
* Calculating the number of optical flows numOF also affects the time, generally taking numOF=18 or slightly more (trying to take 15 and taking less than 0.4 seconds)
* Example of use: See the sample program main.cpp
* 2016.05.24
*/

#pragma once
#include "calcOpticalFlowFeaturesPyrLKSAD.h"
#include "ReadImages.h"
#include <time.h>

Int main()
{
    Const int numOF = 18; //calculate the number of optical flows
    Const int pyrLKSize = 5; //pyramid LK search area
    Const int sadBlockSize = 4; //SAD matches the block size (the actual size is 2*sadBlockSize+1)
    Const int blockSize = 5; // non-maximum value suppresses the true size of the window
    Const int maxPixShift = 30; //Set the maximum pixel displacement (between adjacent frames)

    Std::vector<cv::Point2f> prevXY; //store the coordinates of the calculated optical flow point in the previous frame
    Std::vector<cv::Point2f> currXY; //stores the coordinates of the calculated optical stream point in the next frame

    Float ofXY[numOF][2]; //store the calculated optical flow

//Image path: ETH dataset image, a total of 107 frames
    ReadImages imageReader("G:\\ETH ASL\\ETH-IMAGE-MODIFIED\\Img1", "", ".png");

    Cv::Mat prev;
    Cv::Mat curr;

    / / Calculate the first two frames first
    Prev = imageReader.loadImage(1);
    Curr = imageReader.loadImage(2);

    calcOpticalFlowFeaturesPyrLKSAD(
            Prev,
            Curr,
            prevXY,
            currXY,
            ofXY,
            1,
            numOF,
            pyrLKSize,
            sadBlockSize,
            blockSize,
            maxPixShift);

    / / Do not pollute the original picture, copy the original picture, draw a point on the copy picture, display the result
    Cv::Mat prev_copy, curr_copy;
    prev.copyTo(prev_copy);
    curr.copyTo(curr_copy);

    / / Display the results of the first two frames
    For (int iter2 = 0;iter2 < prevXY.size();iter2++)
    {
        Std::cout << prevXY[iter2].x << ", " << prevXY[iter2].y << std::endl;
        Std::cout << currXY[iter2].x << ", " << currXY[iter2].y << std::endl;
        Std::cout << ofXY[iter2][0] << ", " << ofXY[iter2][1] << std::endl << std::endl;
        Cv::circle(prev_copy, prevXY[iter2], 3, cv::Scalar(0, 0, 255), -1, 8);
        Cv::circle(curr_copy, currXY[iter2], 3, cv::Scalar(0, 0, 255), -1, 8);
    }
    Cv::imshow("prevImg", prev_copy);
    Cv::imshow("currImg", curr_copy);
    Cv::waitKey(1000);

    curr.copyTo(prev);
    Int imgID = 3;

//Timed
    Double timeConsume;
    Clock_t start, finish;
    Start = clock();

    / / Enter the loop test
    While (1)
    {
        Curr = imageReader.loadImage(imgID); //Read in the current frame

        If (curr.empty()) break; //loop termination condition

        prev.copyTo(prev_copy);
        curr.copyTo(curr_copy);
        prevXY.swap(currXY); //Exchange: the current frame becomes the coordinates of the previous frame
        currXY.clear(); //***** Remember to clear *****

        calcOpticalFlowFeaturesPyrLKSAD(
                Prev,
                Curr,
                prevXY,
                currXY,
                ofXY,
                0,
                numOF,
                pyrLKSize,
                sadBlockSize,
                blockSize,
                maxPixShift);

        //show result
        For (int iter2 = 0;iter2 < prevXY.size();iter2++)
        {
//std::cout << prevXY[iter2].x << ", " << prevXY[iter2].y << std::endl;
//std::cout << currXY[iter2].x << ", " << currXY[iter2].y << std::endl;
            Std::cout << ofXY[iter2][0] << ", " << ofXY[iter2][1] << std::endl << std::endl;
            Cv::circle(prev_copy, prevXY[iter2], 3, cv::Scalar(0, 0, 255), -1, 8);
            Cv::circle(curr_copy, currXY[iter2], 3, cv::Scalar(0, 0, 255), -1, 8);
        }
        Cv::imshow("prevImg", prev_copy);
        Cv::imshow("currImg", curr_copy);
        Cv::waitKey(500);

        curr.copyTo(prev); //The current frame becomes the next frame
        Std::cout << std::endl << "**************************** Completed " << imgID << "Frame****************************" << std::endl << std::endl;
        imgID++;
    }

    Finish = clock();

    / / Calculate and display the time
    timeConsume = (double)(finish - start) / CLOCKS_PER_SEC;
    Std::cout << std::endl << "timeConsume= " << timeConsume << std::endl << std::endl;

    System("pause");
}