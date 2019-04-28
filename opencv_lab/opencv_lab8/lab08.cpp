#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

Mat frame, canny_frame;
vector<Vec4i> lines;
Mat frame_with_lines;

Mat foreground_frame;
Mat background_frame;

VideoCapture cap;

Ptr<BackgroundSubtractorMOG2> pMOG2 = createBackgroundSubtractorMOG2();

int history = 5;
int history_max = 10;
int nmixtures = 5;
int nmixtures_max = 5;

void ValueChanger() {
	pMOG2->apply(frame, foreground_frame);
	pMOG2->setHistory(history);
	pMOG2->setNMixtures(nmixtures);
	pMOG2->getBackgroundImage(background_frame);
	
	erode(foreground_frame, foreground_frame, Mat(), Point(-1, -1), 1);
	dilate(foreground_frame, foreground_frame, Mat(), Point(-1, -1), 2);
}

void ValueChange(int, void*) {
	ValueChanger();
}

void cleanUpOnExit(VideoCapture cap) {
	cap.release();
	destroyAllWindows();
}

void pressAnyKey(String action) {
	cout << action + ", press ANY key to continue..." << endl;
	_getch();
}

int do_backgroundSubtractorMog2(string capType) {
	if (capType == "camera") cap.open(0);
	else if (capType == "bike.avi") cap.open("C:/Users/Mariusz/Desktop/opencv_tmp/lab4/bike.avi");

	namedWindow("background", CV_WINDOW_AUTOSIZE);
	namedWindow("foreground", CV_WINDOW_AUTOSIZE);

	createTrackbar("history", "background", &history, history_max, ValueChange);
	createTrackbar("nmixtures", "background", &nmixtures, nmixtures_max, ValueChange);

	while (1) {
		try {
			cap >> frame;
			if (frame.empty()) {
				cap.set(CAP_PROP_POS_FRAMES, 0);
				cap >> frame;
			}

			ValueChanger();

			vector<vector<Point>> contours;
			findContours(foreground_frame, contours, RETR_LIST, CHAIN_APPROX_NONE);

			for (int i = 0; i < contours.size(); i++) {
				double a = contourArea(contours[i], false);
				if (a > 100)
					drawContours(frame, contours, i, Scalar(0, 0, 255), 1);
			}

			imshow("foreground", frame);
			imshow("background", background_frame);
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

int do_houghLinesP(string capType) {
	if (capType == "camera") cap.open(0);
	else if (capType == "bike.avi") cap.open("C:/Users/Mariusz/Desktop/opencv_tmp/lab4/bike.avi");

	namedWindow("window", CV_WINDOW_AUTOSIZE);

	while (1) {
		try {
			cap >> frame;
			if (frame.empty()) {
				cap.set(CAP_PROP_POS_FRAMES, 0);
				cap >> frame;
			}
			frame_with_lines = frame.clone();

			ValueChanger();

			Canny(frame, canny_frame, 100, 255, 3);
			HoughLinesP(canny_frame, lines, 1, CV_PI / 180, 50, 50, 10);

			for (int i = 0; i < lines.size(); i++) {
				Point p1 = Point(lines[i][0], lines[i][1]);
				Point p2 = Point(lines[i][2], lines[i][3]);
				line(frame_with_lines, p1, p2, Scalar(0, 0, 255), 1);
			}

			imshow("window", frame_with_lines);
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

int askForNumber(String question) {
	int number;
	cout << question << endl;
	cin >> number;
	return number;
}

void menu() {
	cout << "MENU:" << endl;
	cout << "   BackgroundSubtractorMOG2" << endl;
	cout << "      1 - video (bike.avi)" << endl;
	cout << "      2 - camera" << endl;
	cout << "   HoughLinesP" << endl;
	cout << "      3 - video (bike.avi)" << endl;
	cout << "      4 - camera" << endl;
	cout << "   Other options" << endl;
	cout << "      5 - exit" << endl;
	cout << endl;
}

int main() {
	menu();
	int choice = askForNumber("Your choice:");

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	switch (choice) {
	case 1:
		do_backgroundSubtractorMog2("bike.avi");
		break;
	case 2:
		do_backgroundSubtractorMog2("camera");
		break;
	case 3:
		do_houghLinesP("bike.avi");
		break;
	case 4:
		do_houghLinesP("camera");
		break;
	case 5:
		cleanUpOnExit(cap);
		pressAnyKey("Good-bye");
		break;
	default:
		cleanUpOnExit(cap);
		pressAnyKey("Unknown choice");
		break;
	}

	return 0;
}