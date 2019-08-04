/*
* File name: Matcher.cpp
* Dependent library: opencv3.1.0 or other version
* Test environment: VS2015 x64
* Abstract: Calculate the optical flow using the restricted feature point detection + pyramid LK refinement + up to 7 SAD matching (SAD: Calculate the sum of the absolute values ​​of the gray value differences of all pixels in the block)
* Restricted: Refers to the feature points that are actually used for optical flow calculation. From the extracted points, select the points that satisfy certain constraints. The point and point satisfy a certain distance: maxPixShift+blockSize
* Pyramid LK: refers to the use of the pyramid LK algorithm to correct the value extracted by the feature point and accurate to the sub-pixel after the feature point is detected.
* Up to 7 SAD matches: Each time, at most 7 points are extracted in the corresponding area of ​​the next frame, and then SAD matching is performed in 7 points, and the minimum SAD value is selected as the matching point.
*Key points: limit distance, 7 matches, reinitialization (reinitialization is very important, when the point is lost, you need to reinitialize, otherwise it will continue to track the points in the previous frame)
* Example of use: See the sample program main.cpp
* Note: 1) The number of optical flows calculated by this program is fixed, that is, the dimension of ofXY is constant; this program is a simple optical flow calculation function, which does not involve angular rate compensation and optical flow sorting and selection. operating
* 2) The optical flow program corresponding to this program should be: no absolute value sorting in XY directions, 2 maximum and 2 minimum optical flow values ​​in the XY direction, not lower than 2, because the program Set in
* Recalculate when the optical flow calculated by 3 points exceeds the set maximum pixel displacement, so that up to 8 points (possibly overlapping) will be discarded; of course, if the calculation
* If the number of optical streams is large, you can consider dropping more (because the program does not store the SAD value of each point, so SAD sorting is not supported to select the optical stream)
* 3) The program calculates the number of optical flows, generally numOF=18. If you take 18, after 8 points are discarded, there are 10 useful points to calculate the required speed and other information. Of course, if you don't need So many useful points,
* Can be less; when the picture quality is better, the texture is more but not messy, the picture size is appropriate, you can take more points, but it will take a little more time. When the picture quality is not good, numOF will be less because
* There may not be so many points.
* 2016.05.24
*/
#pragma once
#include<opencv2/opencv.hpp>

@Brief: Calculate the optical flow, use the feature point detection + pyramid LK algorithm + SAD matching to calculate the optical flow
@Input: prev: previous frame image, CV_8U1
// curr: current frame image, CV_8U1
// prevXY: the vector container of Point2f, declared outside the function, to calculate the x, y coordinates of the optical flow point in the image of the previous frame, the number of points containing is numOF
// currXY: Point2f's vector container, declared outside the function, to calculate the x, y coordinates of the optical flow point in the current frame image, the number of points containing is numOF
// ofXY[][2]: two-dimensional static array, declared outside the function, calculated optical flow value (including x, y) (relative displacement), the number of rows is numOF
// needInit: Whether it needs to be initialized. During the tracking process, the previously searched points may be lost. Then you need to find the optimal corner point suitable for tracking in the initialization (of course, the first initialization is necessary)
// numOF: number of optical flow vectors to be calculated
// pyrLKSize: the pyramid LK calculation area, the actual size is 2*pyrLKSize+1
// sadBlockSize: the size of the SAD block in the SAD match, the actual size is 2*sadBlockSize+1
// blockSize: This parameter is used by the goodFeaturesToTrack() function. This parameter should be the window (real size) for non-maximum suppression (similar to the non-maximum suppression window in Harris). The larger the angle, the more the extracted corner points. Less (this parameter greatly affects the number of corner points extracted)
// maxPixShift: set the maximum pixel displacement of the target between adjacent frames
@Output: No direct output, directly change prevXY, currXY, ofXY, and needInit
@Innotation: Note that all calls to goodFeaturesToTrack() in this program must be the same except for the maxCorner parameter.
Void calcOpticalFlowFeaturesPyrLKSAD(
        Cv::Mat prev,
        Cv::Mat curr,
        Std::vector<cv::Point2f>& prevXY,
        Std::vector<cv::Point2f>& currXY,
        Float ofXY[][2],
        Bool needInit,
        Int numOF = 9,
        Int pyrLKSize=5,
        Int sadBlockSize = 4,
        Int blockSize = 5,
        Int maxPixShift=20);


@Brief: Initialize the optimal corner point, extract the coordinates of the point of the desired optical flow by performing the goodFeaturesToTrack() function on the entire image.
@Input: prev: the previous frame image, CV_8U1, each initialization (or reinitialization), are to find the best corner in the previous frame image
// prevXY: finally determines the coordinates of the calculated optical flow point (actually the output)
// numOF: number of optical flow vectors to be calculated
// numPoints: the number of detection points, and then jump out of numOF from these points, calculate the optical flow and track
// maxPixShift: assumes the maximum pixel displacement of the target between adjacent frames to determine the pixel distance between the calculated optical flow points
// blockSize: This parameter is used by the goodFeaturesToTrack() function.
@Output: no direct output, directly change the value of prevXY[][2]
@Innotation: 1) The reason why numPoints=numOF is not taken directly is because the algorithm calculates a certain distance constraint between the optical flow points, and needs to extract numPonints points from the picture, and then according to the response value.
// Sort from big to small, taking the first numOF points that satisfy the distance constraint as the calculated optical flow point
Void initGoodFeatures(
        Cv::Mat prev,
        Std::vector<cv::Point2f>& prevXY,
        Int numOF,
        Int numPoints,
        Int maxPixShift,
        Int blockSize);


@Brief: After initialization, call this function to calculate the optical flow
@Input: curr: current frame, CV_8U1
// prevXY: the coordinates of the point in the previous frame image
// currXY: the coordinates of the point in the current frame image
// ofXY: calculated light flow
// numOF: number of optical streams to be calculated
// pyrLKSize: the pyramid LK calculation area, the actual size is 2*pyrLKSize+1
// sadBlockSize: the size of the SAD block in the SAD match, the actual size is 2*sadBlockSize+1
// blockSize: This parameter is used by the goodFeaturesToTrack() function. This parameter should be the window (real size) for non-maximum suppression (similar to the non-maximum suppression window in Harris). The larger the angle, the more the extracted corner points. Less (this parameter greatly affects the number of corner points extracted)
// maxPixShift: set the maximum pixel displacement of the target between adjacent frames
@Output: No direct output, directly change currXY[][2] and ofXY[][2], and it is possible to change prevXY[][2] (if reinitialized)
@Innotation: 1) This function, first in the square area around the point in the previous frame, use the goodFeaturesToTrack() function (using the same parameters, except searcSize no longer adds 2) to find 7 points
// Then use the pyramid LK function to accurately correct the corner points extracted by the goodFeaturesToTrack() function in the subpixel. Calculate the SAD value of the block where each point is located, and select the smallest SAD.
// value as a matching point
// 2) This function generally only uses the current frame of curr. Because prevXY has been used, it is not necessary to re-find the good corners in the picture every time. Only when it is lost, the initialization function will be called.
/ / Find the corner point, this can increase the speed, the input perv is the same as the time to re-initialize
// 3) The reason is 7 points because after reasonable analysis, according to this algorithm, there are usually 7 interference points at most.
        Void calcOF(
        Cv::Mat prev,
Cv::Mat curr,
        Std::vector<cv::Point2f>& prevXY,
Std::vector<cv::Point2f>& currXY,
        Float ofXY[][2],
Int numOF,
        Int pyrLKSize,
Int sadBlockSize,
        Int blockSize,
Int maxPixShift);