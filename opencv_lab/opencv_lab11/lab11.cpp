#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>
#include<windows.h>

using namespace cv;
using namespace std;

int choice = 1;

void choiceChange() {
	if (GetAsyncKeyState('1'))
		choice = 1;
	else if (GetAsyncKeyState('2'))
		choice = 2;
	else if (GetAsyncKeyState('3'))
		choice = 3;
}

Mat textWithShadow(Mat frame, string text, Point point, Scalar shadowColor) {
	Point shadow1 = Point(point.x + 1, point.y + 1);
	Point shadow2 = Point(point.x + 1, point.y - 1);
	Point shadow3 = Point(point.x - 1, point.y + 1);
	Point shadow4 = Point(point.x - 1, point.y - 1);
	putText(frame, text, shadow1, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(frame, text, shadow2, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(frame, text, shadow3, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(frame, text, shadow4, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(frame, text, point, FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255));
	return frame;
}
string mode;
Mat putChoiceOnFrame(Mat frame) {
	
	switch (choice) {
		case 1:
			mode = "Running...";
			break;
		case 2:
			mode = "Paused";
			break;
		case 3:
			mode = "Off";
			break;
	}
	frame = textWithShadow(frame, "Choice:", Point(5, 15), Scalar(150, 0, 0));
	frame = textWithShadow(frame, mode, Point(75, 15), Scalar(0, 100, 0));
	return frame;
}

Point getCenter(vector<Point> hull) {
	double x = 0.0;
	double y = 0.0;
	for (int j = 0; j < hull.size(); j++) {
		x += hull[j].x;
		y += hull[j].y;
	}
	x = x / hull.size();
	y = y / hull.size();
	return Point(x, y);
}

void putTrackingInfoOnFrame(Mat frame, Point center) {
	string text = "Tracking at: (" + to_string((int)center.x) + ", " + to_string((int)center.y) + ")";
	circle(frame, center, 3, Scalar(0, 0, 200), 2);
	putText(frame, text, Point(center.x + 10, center.y), FONT_HERSHEY_PLAIN, 1.25, Scalar(0, 0, 200), 2);
}

vector<Point> maxContour;

int main() {
	Mat frame;
	Mat frame1, frame2;
	Mat diff, blurDiff;
	Mat thresholded;

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	VideoCapture cap;
	cap.open(0);

	namedWindow("diff", CV_WINDOW_AUTOSIZE);
	namedWindow("blur_diff", CV_WINDOW_AUTOSIZE);
	namedWindow("threshold", CV_WINDOW_AUTOSIZE);

	namedWindow("window", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	while (1) {
		try {
			cap >> frame;

			frame1 = frame.clone();
			cvtColor(frame1, frame1, CV_RGB2GRAY);
			cap >> frame2;
			cvtColor(frame2, frame2, CV_RGB2GRAY);

			absdiff(frame1, frame2, diff);
			GaussianBlur(diff, blurDiff, Size(0, 0), 3);

			threshold(blurDiff, thresholded, 20, 255, THRESH_BINARY);
			findContours(thresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

			if (choice == 1) {
				maxContour.clear();
				double area = 0.0;
				for (int i = 0; i < contours.size(); i++) {
					double area_i = contourArea(contours[i], false);
					if (area_i > area) {
						area = area_i;
						maxContour = contours[i];
					}
				}
			}

			if (!maxContour.empty() && choice != 3) {
				Rect brect = boundingRect(maxContour);
				vector<Point> hull;
				convexHull(maxContour, hull, true, true);

				rectangle(frame, brect, Scalar(0, 150, 0), 2);
				polylines(frame, hull, true, Scalar(255, 0, 0), 2);

				Point center = getCenter(hull);
				putTrackingInfoOnFrame(frame, center);
			}

			choiceChange();
			frame = putChoiceOnFrame(frame);

			imshow("diff", diff);
			imshow("blur_diff", blurDiff);
			imshow("threshold", thresholded);

			imshow("window", frame);
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