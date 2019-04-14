#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>
#include "TrackedObject.h"

using namespace cv;
using namespace std;

Mat frame, frame_hsv, frame_threshold;
Mat frame_with_contours;

vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

Scalar color = (0, 255, 0);

vector<TrackedObject> trackedVector;

// aktualnie ustawiony: zielony
// int hsv_h_min = 34;
// int hsv_h_max = 82;
// int hsv_s_min = 65;
// int hsv_s_max = 253;
// int hsv_v_min = 75;
// int hsv_v_max = 154;

// u mnie to jest zielony...
int hsv_h_min = 5;
int hsv_h_max = 75;
int hsv_s_min = 132;
int hsv_s_max = 267;
int hsv_v_min = 101;
int hsv_v_max = 183;

const int hsv_max_value = 360;

Point calculateCenter(Moments m) {
	return Point(m.m10 / m.m00, m.m01 / m.m00);
}

Point addToPoint(Point p, int x, int y) {
	return Point(p.x + x, p.y + y);
}

void drawCircle(TrackedObject tracked) {
	circle(frame_with_contours, tracked.getPoint(), 30, (0, 255, 0), 3);
}

void drawPlus(TrackedObject tracked) {
	Point center = tracked.getPoint();

	Point center_p1 = addToPoint(center, -15, 0);
	Point center_p2 = addToPoint(center, 15, 0);
	Point center_p3 = addToPoint(center, 0, -15);
	Point center_p4 = addToPoint(center, 0, 15);

	line(frame_with_contours, center_p1, center_p2, color, 3);
	line(frame_with_contours, center_p3, center_p4, color, 3);
}

void drawPosition(TrackedObject tracked) {
	Point center = tracked.getPoint();
	Point center_p = addToPoint(tracked.getPoint(), -75, -35);
	putText(frame_with_contours, format("(%d,%d)", center.x, center.y), center_p, FONT_HERSHEY_DUPLEX, 1.0, color);
}

void drawAim(TrackedObject trackedObject) {
	drawCircle(trackedObject);
	drawPlus(trackedObject);
	drawPosition(trackedObject);
}

void createTrackedObject(vector<vector<Point>> contours, int i) {
	Moments m = moments(contours[i], true);
	Point center = calculateCenter(m);
	trackedVector.push_back(TrackedObject(center, m));
}

void HSV_ValueChanger() {
	inRange(frame_hsv, Scalar(hsv_h_min, hsv_s_min, hsv_v_min), Scalar(hsv_h_max, hsv_s_max, hsv_v_max), frame_threshold);
	erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 2);
	dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 2);

	findContours(frame_threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	frame_with_contours = frame.clone();

	trackedVector.clear();
	for (int i = 0; i < contours.size(); i++) {
		double a = contourArea(contours[i], false);
		if (a > 150) createTrackedObject(contours, i);
	}

	for (int i = 0; i < trackedVector.size(); i++) {
		drawAim(trackedVector.at(i));
	}
}

void HSV_ValueChange(int, void*) {
	HSV_ValueChanger();
}

int main() {
	VideoCapture cap;
	cap.open(0);

	namedWindow("window", CV_WINDOW_AUTOSIZE);
	namedWindow("hsv", CV_WINDOW_AUTOSIZE);
	namedWindow("threshold", CV_WINDOW_AUTOSIZE);
	namedWindow("contours", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	createTrackbar("H_min", "window", &hsv_h_min, hsv_max_value, HSV_ValueChange);
	createTrackbar("H_max", "window", &hsv_h_max, hsv_max_value, HSV_ValueChange);
	createTrackbar("S_min", "window", &hsv_s_min, hsv_max_value, HSV_ValueChange);
	createTrackbar("S_max", "window", &hsv_s_max, hsv_max_value, HSV_ValueChange);
	createTrackbar("V_min", "window", &hsv_v_min, hsv_max_value, HSV_ValueChange);
	createTrackbar("V_max", "window", &hsv_v_max, hsv_max_value, HSV_ValueChange);

	while (1) {
		try {
			cap >> frame;
			cvtColor(frame, frame_hsv, CV_RGB2HSV);

			HSV_ValueChanger();

			imshow("window", frame);
			imshow("hsv", frame_hsv);
			imshow("threshold", frame_threshold);
			imshow("contours", frame_with_contours);
		} catch (Exception e) {
			cap.open(1);
		}

		if (waitKey(15) == 27) {
			cap.release();
			destroyAllWindows();
			return 0;
		}
	}
	return 0;
}