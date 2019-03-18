#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

void cleanUpOnExit(VideoCapture cap) {
	cap.release();
	destroyAllWindows();
}

void pressAnyKey() {
	cout << "The end, press ANY key to exit..." << endl;
	_getch();
}

Mat motionDetection(VideoCapture cap, Mat currentFrame, Mat frameToCompare, int threshold_value, int alpha) {
	Mat diff;
	Mat accumulator = Mat::zeros(currentFrame.size(), CV_32FC1);

	absdiff(frameToCompare, currentFrame, diff);
	cvtColor(diff, diff, CV_RGB2GRAY);
	threshold(diff, diff, threshold_value, 255, THRESH_BINARY);
	accumulateWeighted(diff, accumulator, alpha);
	convertScaleAbs(accumulator, diff);
	return diff;
}

int main() {
	Mat firstFrame, currentFrame;
	Mat diff;
	
	VideoCapture cap;

	String path = "C:/Users/Mariusz/Desktop/opencv_tmp/lab4/";
	//cap.open(0);
	//cap.open(path + "robot_no_loop.avi");
	cap.open(path + "bike.avi");

	namedWindow("original", CV_WINDOW_AUTOSIZE);
	namedWindow("modified", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	cap >> firstFrame;
	currentFrame = firstFrame.clone();

	double threshold_value = 30;
	double alpha = 1.0;

	while (1) {
		try {
			cap >> currentFrame;
			if (currentFrame.empty()) {
				pressAnyKey();
				cleanUpOnExit(cap);
				return 0;
			}
			diff = motionDetection(cap, currentFrame, firstFrame, threshold_value, alpha);
			imshow("original", currentFrame);
			imshow("modified", diff);
		} catch (Exception e) {
			cap.open(1);
		}

		if (waitKey(15) == 27) {
			cleanUpOnExit(cap);
			return 0;
		}
	}
	return 0;
}