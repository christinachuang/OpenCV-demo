// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <highgui.h>
using namespace cv;
using namespace std;

int slider_value;
Mat src1, src2;
Mat src, src_gray;
Mat dst2, detected_edges;
Mat dst;

int edgeThresh = 1;
int lowThreshold=50;
int ratio = 3;
int kernel_size = 3;
void on_trackbar(int, void*) {
	double alpha = (double)slider_value / 100.0;
	double beta = (1.0 - alpha);
	addWeighted(dst2, alpha, src, beta, 0.0, dst);
}

void CannyThreshold(int, void*) {
	blur(src_gray, detected_edges, Size(3, 3));
	Canny(detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size);
	dst2 = Scalar::all(0);
	src.copyTo(dst2, detected_edges);
}
int main(){
	VideoCapture cap(0);
	VideoCapture cap2("LIVE.MP4");
	if (!cap.isOpened())
		return -1;
	if (!cap2.isOpened())
		return -2;

	slider_value = 0;
	int slider_max_value = 100;
		namedWindow("trackbar demo", CV_WINDOW_KEEPRATIO);
	createTrackbar("Ratio", "trackbar demo", &slider_value, slider_max_value, on_trackbar);

	for (;;) {
		Mat frame, frame_gray;
		Mat frame2;
		cap >> frame;
		cap2 >> frame2;
		resize(frame2, frame2, Size(frame2.cols / 1.5, frame2.rows / 1.5));
		resize(frame, frame, Size(frame.cols / 1.5, frame.rows / 1.5));
		src = frame;
		if (!src.data)
			return -3;
		dst2.create(src.size(), src.type());
		cvtColor(src, src_gray, CV_BGR2GRAY);
		CannyThreshold(0, 0);
		on_trackbar(slider_value, 0);
		Mat srcImagel = frame2;
		Mat imageROI = srcImagel(Rect(10, 5, dst.cols, dst.rows));
		dst.copyTo(imageROI);
		imshow("trackbar demo", srcImagel);
		waitKey(30);
	}
    return 0;
}

