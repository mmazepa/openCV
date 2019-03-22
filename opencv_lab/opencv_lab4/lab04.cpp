#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

VideoCapture cap;

const int threshold_max = 255;
int threshold_value = 50;

const int alpha_max = 100;
int alpha = 30;

string thresholdTrackBarName = "Threshold";
string alphaTrackBarName = "Alpha";

void on_threshold_trackbar(int, void*) {
	threshold_value = getTrackbarPos(thresholdTrackBarName, "modified");
}

void on_alpha_trackbar(int, void*) {
	alpha = getTrackbarPos(alphaTrackBarName, "modified");
}

void cleanUpOnExit(VideoCapture cap) {
	cap.release();
	destroyAllWindows();
}

void pressAnyKey() {
	cout << "The end, press ANY key to exit..." << endl;
	_getch();
}

Mat motionDetection(VideoCapture cap, Mat currentFrame, Mat frameToCompare, Mat accumulator, int threshold_value, double alpha) {
	Mat diff;	

	absdiff(currentFrame, frameToCompare, diff);
	cvtColor(diff, diff, CV_RGB2GRAY);
	threshold(diff, diff, threshold_value, 255, THRESH_BINARY);	
	accumulateWeighted(diff, accumulator, alpha);
	convertScaleAbs(accumulator, diff);
	
	return diff;
}

Mat playInALoop(Mat currentFrame) {
	cap.set(CV_CAP_PROP_POS_FRAMES, 1);
	cap >> currentFrame;
	return currentFrame;
}

void exitOnVideoEnd() {
	pressAnyKey();
	cleanUpOnExit(cap);
}

int movingAverage(String capture) {
	Mat firstFrame, currentFrame;
	Mat diff;

	if (capture == "camera") {
		cap.open(0);
	} else {
		String path = "C:/Users/Mariusz/Desktop/opencv_tmp/lab4/";
		cap.open(path + capture);
	}

	//cap.open(path + "robot_no_loop.avi");
	//cap.open(path + "bike.avi");

	namedWindow("original", CV_WINDOW_AUTOSIZE);
	namedWindow("modified", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	cap >> firstFrame;
	currentFrame = firstFrame.clone();

	Mat accumulator = Mat::zeros(firstFrame.size(), CV_64FC1);

	createTrackbar(thresholdTrackBarName, "modified", &threshold_value, threshold_max, on_threshold_trackbar);
	createTrackbar(alphaTrackBarName, "modified", &alpha, alpha_max, on_alpha_trackbar);

	while (1) {
		try {
			cap >> currentFrame;
			if (currentFrame.empty()) {
				currentFrame = playInALoop(currentFrame);
				//exitOnVideoEnd(); break;
			}
			double alpha_value = (double)alpha / alpha_max;
			diff = motionDetection(cap, currentFrame, firstFrame, accumulator, threshold_value, alpha_value);
			imshow("original", currentFrame);
			imshow("modified", diff);
		}
		catch (Exception e) {
			cap.open(1);
		}

		if (waitKey(15) == 27) {
			cleanUpOnExit(cap);
			return 0;
		}
	}
	return 0;
}

int askForNumber(String question) {
	int number;
	cout << question << endl;
	cin >> number;
	return number;
}

void menu() {
	cout << "MENU:" << endl;
	cout << "   Motion Detection" << endl;
	cout << "      1 - video (bike.avi)" << endl;
	cout << "      2 - video (robot_no_loop.avi)" << endl;
	cout << "      3 - camera" << endl;
	cout << "   Other options" << endl;
	cout << "      4 - exit" << endl;
	cout << endl;
}

int main() {
	menu();
	int choice = askForNumber("Your choice:");

	switch (choice) {
		case 1:
			movingAverage("bike.avi");
			break;
		case 2:
			movingAverage("robot_no_loop.avi");
			break;
		case 3:
			movingAverage("camera");
			break;
		case 4:
			destroyAllWindows();
			cout << "Good-bye!" << endl;
			pressAnyKey();
			break;
		default:
			cout << "Unknown choice." << endl;
			pressAnyKey();
			break;
	}
}