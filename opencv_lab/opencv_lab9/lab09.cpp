#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

Mat frame_next, frame_prev;
vector<Point2f> corners_prev, corners_next;

vector<uchar> status;
vector<float> err;

bool needToInit = true;

TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03);
Size subPixWinSize(10, 10), winSize(31, 31);

Mat corner_frame;
VideoCapture cap;

// int...
int maxCorners = 50;
int maxCorners_max = 100;

// double...
int qualityLevel = 1;
int qualityLevel_max = 100;

// double...
int minDistance = 250;
int minDistance_max = 250;

int counter = 0;
int timeDelay = 15;

int threshold_value = 0;
int erode_value = 0;
int dilate_value = 0;

double prepareDoubleValue(int min, int max, int ratio) {
	double val = (double) min / max;
	return val * ratio;
}

double guardRange(double value) {
	if (value == 0.0) value += 0.0001;
	else if (value == 1.0) value -= 0.0001;
	return value;
}

void textWithShadow(string text, double value, Point point, Scalar shadowColor) {
	Point shadow1 = Point(point.x + 1, point.y + 1);
	Point shadow2 = Point(point.x + 1, point.y - 1);
	Point shadow3 = Point(point.x - 1, point.y + 1);
	Point shadow4 = Point(point.x - 1, point.y - 1);
	string textToDisplay = text + format(": %.5g", value);
	putText(corner_frame, textToDisplay, shadow1, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(corner_frame, textToDisplay, shadow2, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(corner_frame, textToDisplay, shadow3, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(corner_frame, textToDisplay, shadow4, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(corner_frame, textToDisplay, point, FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255));
}

void putTrackbarRealValuesOnFrame(double newQuality, double newDistance) {
	textWithShadow("Max Corners", maxCorners, Point(5, 15), Scalar(150, 0, 0));
	textWithShadow("Quality Level", newQuality, Point(5, 30), Scalar(0, 100, 0));
	textWithShadow("Min Distance", newDistance, Point(5, 45), Scalar(0, 0, 150));
	textWithShadow("Points", corners_next.size(), Point(5, 60), Scalar(50, 50, 50));
}

void displayCircles(vector<Point2f> corners, Scalar color) {
	for (int i = 0; i < corners.size(); i++)
		circle(corner_frame, corners[i], 4, color, 2);
}

bool pointInsideImage(Point2f point) {
	if (point.x < 0 || point.x > 640) return false;
	if (point.y < 0 || point.y > 480) return false;
	return true;
}

Mat motionDetection(VideoCapture cap, Mat currentFrame, Mat frameToCompare, int threshold_value, int erode_value, int dilate_value) {
	Mat diff;
	absdiff(frameToCompare, currentFrame, diff);
	cvtColor(diff, diff, CV_RGB2GRAY);
	threshold(diff, diff, threshold_value, 255, THRESH_BINARY);
	erode(diff, diff, Mat(), Point(-1, -1), erode_value);
	dilate(diff, diff, Mat(), Point(-1, -1), dilate_value);
	return diff;
}

void ValueChanger() {
	Mat tmp = frame_next.clone();	
	tmp = motionDetection(cap, frame_next, frame_prev, threshold_value, erode_value, dilate_value);

	double newQuality = guardRange(prepareDoubleValue(qualityLevel, qualityLevel_max, 1));
	double newDistance = prepareDoubleValue(minDistance, minDistance_max, 5);

	if (needToInit) {
		goodFeaturesToTrack(tmp, corners_next, maxCorners, newQuality, newDistance, Mat(), 3, 0, false, 0.04);
		needToInit = false;
	} else if (!corners_prev.empty()) {
		calcOpticalFlowPyrLK(frame_prev, frame_next, corners_prev, corners_next, status, err, winSize, 3, termcrit, 0, 0.001);
	
		displayCircles(corners_next, Scalar(0, 150, 0));

		for (int i = status.size() - 1; i >= 0; i--) {
			if (status[i] == 1 && !err[i] == 0)
				line(corner_frame, corners_prev[i], corners_next[i], Scalar(0, 255, 0), 1);

			if (!pointInsideImage(corners_next[i])) {
				cout << "Point " << i << " deleted!" << endl;
				corners_next.erase(corners_next.begin() + i);
				corners_prev.erase(corners_prev.begin() + i);
			}
		}

		if (counter % timeDelay == 0)
			goodFeaturesToTrack(tmp, corners_next, maxCorners, newQuality, newDistance, Mat(), 3, 0, false, 0.04);
		counter++;
	}

	vector<Point> locations;
	findNonZero(tmp, locations);

	if (locations.size() > 0) {
		while (corners_next.size() < maxCorners / 2) {
			Point point = locations[rand() % locations.size()];
			corners_next.push_back(point);
			cout << point << " added!" << endl;
		}
	}

	putTrackbarRealValuesOnFrame(newQuality, newDistance);

	frame_prev = frame_next.clone();
	corners_prev = corners_next;

	corners_next.clear();
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

int askForNumber(String question) {
	int number;
	cout << question << endl;
	cin >> number;
	return number;
}

void menu() {
	cout << "MENU:" << endl;
	cout << "   Optical Flow" << endl;
	cout << "      1 - video (bike.avi)" << endl;
	cout << "      2 - camera" << endl;
	cout << "   Other options" << endl;
	cout << "      3 - exit" << endl;
	cout << endl;
}

int main() {
	menu();
	int choice = askForNumber("Your choice:");

	switch (choice) {
		case 1:
			maxCorners = 10;
			cap.open("C:/Users/Mariusz/Desktop/opencv_tmp/lab4/bike.avi");
			threshold_value = 20;
			erode_value = 1;
			dilate_value = 1;
			break;
		case 2:
			cap.open(0);
			threshold_value = 30;
			erode_value = 1;
			dilate_value = 2;
			break;
		case 3:
			cleanUpOnExit(cap);
			pressAnyKey("Good-bye");
			return 0;
			break;
		default:
			cleanUpOnExit(cap);
			pressAnyKey("Unknown choice");
			return 0;
			break;
	}

	namedWindow("window", CV_WINDOW_AUTOSIZE);
	namedWindow("motion", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	createTrackbar("maxCorners", "motion", &maxCorners, maxCorners_max, ValueChange);
	createTrackbar("qualityLevel", "motion", &qualityLevel, qualityLevel_max, ValueChange);
	createTrackbar("minDistance", "motion", &minDistance, minDistance_max, ValueChange);

	cap >> frame_prev;

	while (1) {
		try {
			cap >> frame_next;
			if (frame_next.empty()) {
				cap.set(CAP_PROP_POS_FRAMES, 0);
				cap >> frame_next;
			}
			corner_frame = frame_next.clone();

			ValueChanger();
			cout << maxCorners << ", " << qualityLevel << ", " << minDistance << endl;

			imshow("window", frame_next);
			imshow("motion", corner_frame);
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