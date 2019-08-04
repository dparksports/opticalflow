/*
* File name: Matcher.h
* Dependent library: opencv3.1.0 or other version
* Test environment: VS2015 x64
* Abstract: Calculate the optical flow using the restricted feature point detection + pyramid LK refinement + up to 7 SAD matching (SAD: Calculate the sum of the absolute values ​​of the gray value differences of all pixels in the block)
* Restricted: Refers to the feature points that are actually used for optical flow calculation. From the extracted points, select the points that satisfy certain constraints. The point and point satisfy a certain distance: maxPixShift+blockSize
* Pyramid LK: refers to the use of the pyramid LK algorithm to correct the value extracted by the feature point and accurate to the sub-pixel after the feature point is detected.
* Up to 7 SAD matches: Each time, at most 7 points are extracted in the corresponding area of ​​the next frame, and then SAD matching is performed in 7 points, and the minimum SAD value is selected as the matching point.
* Key points: limit distance, 7 matches, reinitialization (reinitialization is very important, when the point is lost, you need to reinitialize, otherwise it will continue to track the points in the previous frame)
* Note: The number of optical flows calculated by this program is fixed, that is, the dimension of ofXY is constant; this program is a simple optical flow calculation function, does not involve angular rate compensation and optical flow sorting and selection operations.
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

#include "calcOpticalFlowFeaturesPyrLKSAD.h"

Void calcOpticalFlowFeaturesPyrLKSAD(
        Cv::Mat prev,
        Cv::Mat curr,
        Std::vector<cv::Point2f>& prevXY,
        Std::vector<cv::Point2f>& currXY,
        Float ofXY[][2],
        Bool needInit,
        Int numOF,
        Int pyrLKSize,
        Int sadBlockSize,
        Int blockSize,
        Int maxPixShift)
{
    Int numPoints = 30 * numOF;
    If (needInit)
    {
        initGoodFeatures(
                Prev,
                prevXY,
                numOF,
                numPoints,
                maxPixShift,
                blockSize);
    }

    calcOF(
            Prev,
            Curr,
            prevXY,
            currXY,
            ofXY,
            numOF,
            pyrLKSize,
            sadBlockSize,
            blockSize,
            maxPixShift);
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%*/
/*------------------------------------------------ ----Fault dividing line ------------------------------------------- ---------*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%*/

Void initGoodFeatures(
        Cv::Mat prev,
        Std::vector<cv::Point2f>& prevXY,
        Int numOF,
        Int numPoints,
        Int maxPixShift,
        Int blockSize)
{
    Int minDis = maxPixShift + blockSize;
    Cv::Mat prev_copy;
    prev.copyTo(prev_copy);

    Std::vector<cv::Point2f> goodCorners;
    Cv::goodFeaturesToTrack(
            Prev, / / ​​to extract the image area of ​​goodFeatures
    goodCorners, / / ​​used to store the extracted good corner vector container
    numPoints, // extract and store the maximum number of corners of goodCorners, which is equal to numOF, which is the number of required optical flows.
            0.001, / / ​​quality level, the larger, the better the quality of the corner points extracted, the less the number of corner points extracted
    0, / / ​​extract the minimum distance between the corner points, set to 0, so that the corner points are not affected by the distance
    Cv::Mat(), //Define the area where you need to find the corner (mask)
            blockSize, //blockSize, this parameter should be the window for non-maximum suppression (similar to the non-maximum suppression window in Harris). The larger the angle, the fewer corner points are extracted (this parameter greatly affects the extraction. Corner points)
            True, //whether to use Harris corner detection
            0.01); //k parameter: the parameters in the Harris corner detection, see the opencv manual or the "Image Local Invariance Features and Descriptions" book 50 pages, k is alpha

    Int detected = goodCorners.size(); //The number of points extracted does not necessarily have numPoints
    Int rows = prev.rows;
    Int cols = prev.cols;
    Int counter = 0;

    For (int i = 0; i < detected; i++)
    {
        Int xx = goodCorners[i].x;
        Int yy = goodCorners[i].y;

        Bool qualified = false;
/****** The discriminant function in this if statement is very important to determine whether it is close to the edge. If it is close to the edge, it is not a good tracking point. You can change its range so that the found corner point is suitable for tracking for a long time. To reduce the more time-consuming initialization process******/
/*if (xx > (2 * minDis + 1) && xx < (cols - 2 * minDis + 1) && yy >(2 * minDis + 1) && yy < (rows - 2 * minDis + 1))*/
        If (xx >(2 * minDis + 1) && xx < (cols - 2 * minDis + 1) && yy >(2 * minDis + 1) && yy < (rows - 2 * minDis + 1))
        {
            Qualified = true;
            For (int j = 0; j < i; j++)
            {
//This if statement is to determine whether there is a certain distance between all the corner points of the corner point and the response value of the corner point. If it is less than the given search distance, it will be discarded. This can effectively avoid these points with larger response values. Interference, plus 1 just to ensure every possible possible impact
                If (abs(xx - goodCorners[j].x)<= minDis +1 && abs(yy - goodCorners[j].y) <= minDis +1)
                {
                    Qualified = false;
                    Break;
                }
            }
            If (qualified)
            {
                prevXY.push_back(goodCorners[i]);
                Counter++;
                If (counter >= numOF)
                {
                    Break; / / find full numOF suitable for tracking good corners, jump out of the loop
                }
            }
        }
    }

    / / If you do not find a suitable tracking point in numPoints, then recursively call this function, numPoints doubled, find more points and then choose
    If (prevXY.size() >= numOF)
    {
        / / Recursive function must have a termination loop, so the cornerSubPix () function must be placed into the else, can not be directly outside
                                                                                                                                               / / Accurate to sub-pixel
        Cv::cornerSubPix(prev, prevXY, cv::Size(blockSize / 2, blockSize / 2), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 0.01, 30));

        Std::vector<cv::Point2f>().swap(goodCorners);

        Return;
    }
    Else
    {
        prevXY.clear(); //Clear prevXY
        numPoints *= 2;

        / / If numPoints> 3000 has not found the corner point, then you can not find
        If (numPoints > 3000)
        {
            Std::cout << "The corner points that are suitable for calculating the optical flow and tracking are not detected!" << std::endl << std::endl;
            Assert(0);
            Return;
        }
        initGoodFeatures(
                Prev,
                prevXY,
                numOF,
                numPoints,
                maxPixShift,
                blockSize);
    }
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%*/
/*------------------------------------------------ ----Fault dividing line ------------------------------------------- ---------*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% %%%%%%%%%%%%%%%%*/

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
        Int maxPixShift)
{
//Before extracting the point, the limit distance is maxPixShift+blockSize, because there is a pyramid LK process, so add pyrLKSize to prevent the graph from being exceeded.