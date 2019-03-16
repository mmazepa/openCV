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

String getPath(String videoName) {
	String path = "C:/Users/Mariusz/Desktop/opencv_tmp/";
	return path + "/" + videoName + "/" + videoName + ".avi";
}

Mat motionDetection(VideoCapture cap, Mat currentFrame, Mat frameToCompare, int threshold_value, int erode_value, int dilate_value) {
	Mat diff;
	absdiff(frameToCompare, currentFrame, diff);
	cvtColor(diff, currentFrame, CV_RGB2GRAY);
	threshold(currentFrame, diff, threshold_value, 255, THRESH_BINARY);
	erode(diff, diff, Mat(), Point(-1, -1), erode_value);
	dilate(diff, diff, Mat(), Point(-1, -1), dilate_value);
	return diff;
}

int motionDetector(VideoCapture cap, int choice) {
	Mat firstFrame, currentFrame, previousFrame;
	Mat diff;

	namedWindow("original", CV_WINDOW_AUTOSIZE);
	namedWindow("modified", CV_WINDOW_AUTOSIZE);

	if (choice == 1 || choice == 2) {
		cap.open(getPath("robot_no_loop"));
	} else if (choice == 3 || choice == 4) {
		cap.open(0);
	}

	cap >> firstFrame;
	previousFrame = firstFrame.clone();

	while (1) {
		try {
			cap >> currentFrame;
			if (currentFrame.empty()) {
				pressAnyKey();
				cleanUpOnExit(cap);
				return 0;
			}

			if (choice == 1) diff = motionDetection(cap, currentFrame, firstFrame, 20, 3, 1);
			else if (choice == 2) diff = motionDetection(cap, currentFrame, previousFrame, 15, 1, 3);
			else if (choice == 3) diff = motionDetection(cap, currentFrame, firstFrame, 20, 3, 1);
			else if (choice == 4) diff = motionDetection(cap, currentFrame, previousFrame, 20, 3, 1);

			imshow("original", currentFrame);
			imshow("modified", diff);
			previousFrame = currentFrame.clone();
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

int addLogo(VideoCapture cap, int choice) {
	Mat frame;

	if (choice == 5) {
		cap.open(getPath("robot_no_loop"));
	}
	else if (choice == 6) {
		cap.open(0);
	}

	while (1) {
		try {
			cap >> frame;
			if (frame.empty()) {
				pressAnyKey();
				cleanUpOnExit(cap);
				return 0;
			}
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
	cout << "MOTION DETECTION" << endl;
	cout << "----------------" << endl;
	cout << "MENU:" << endl;
	cout << "1 - video (first frame)" << endl;
	cout << "2 - video (previous frame)" << endl;
	cout << "3 - camera (first frame)" << endl;
	cout << "4 - camera (previous frame)" << endl;
	cout << "5 - video (add logo)" << endl;
	cout << "6 - camera (add logo)" << endl;
	cout << "7 - exit" << endl;
	cout << endl;
}

int main() {
	VideoCapture cap;

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	menu();
	int choice = askForNumber("Your choice:");

	if (choice < 5) {
		motionDetector(cap, choice);
	} else if (choice < 7) {
		addLogo(cap, choice);
	} else if (choice == 7) {
		destroyAllWindows();
		cout << "Good-bye!" << endl;
		pressAnyKey();
		return 0;
	} else {
		cout << "Unknown choice." << endl;
		pressAnyKey();
		return 0;
	}
	return 0;
}