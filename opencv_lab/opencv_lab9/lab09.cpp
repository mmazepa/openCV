#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/video.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

Mat frame_next, frame_prev;
vector<Point2f> corners_prev, corners_next;
vector<Point2f> tmp_corners_prev, tmp_corners_next;
vector<uchar> status;
vector<float> err;

Mat corner_frame;
VideoCapture cap;

// int...
int maxCorners = 150;
int maxCorners_max = 250;

// double...
int qualityLevel = 2;
int qualityLevel_max = 100;

// double...
int minDistance = 10;
int minDistance_max = 250;

int counter = 0;

double prepareDoubleValue(int min, int max) {
	return (double) min / max;
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
}

void displayCircles(vector<Point2f> corners, Scalar color) {
	for (int i = 0; i < corners.size(); i++) {
		circle(corner_frame, corners[i], 4, color, 2);
	}
}

void displayCircles(vector<Point2f> corners1, vector<Point2f> corners2, Scalar color1, Scalar color2) {
	displayCircles(corners1, color1);
	displayCircles(corners2, color2);
}

bool pointInsideImage(Point2f point) {
	if (point.x < 0 && point.x > 640) return false;
	if (point.y < 0 && point.y > 480) return false;
	return true;
}

void ValueChanger() {
	Mat tmp = frame_next.clone();
	cvtColor(tmp, tmp, CV_RGB2GRAY);

	double newQuality = guardRange(prepareDoubleValue(qualityLevel, qualityLevel_max));
	double newDistance = prepareDoubleValue(minDistance, minDistance_max);

	goodFeaturesToTrack(tmp, corners_next, maxCorners, newQuality, newDistance, Mat(), 3, 0, false, 0.04);
	//cornerSubPix(tmp, corners_next, Size(15, 15), Size(-1, -1), TermCriteria());

	//displayCircles(corners_prev, Scalar(0, 0, 200));

	//tmp_corners_next = corners_next;
	//tmp_corners_prev = corners_prev;

	if (!corners_prev.empty() && !frame_prev.empty())
		calcOpticalFlowPyrLK(frame_prev, frame_next, corners_prev, corners_next, status, err);

	displayCircles(corners_next, Scalar(0, 150, 0));

	//displayCircles(corners_prev, corners_next, Scalar(0, 0, 200), Scalar(0, 150, 0));

	//cout << status.size() << endl;
	for (int i = 0; i < status.size(); i++) {
		if (!pointInsideImage(corners_prev[i]) || !pointInsideImage(corners_next[i])) {
			//corners_prev.erase(corners_prev.begin() + i);
			//corners_next.erase(corners_next.begin() + i);
			//goodFeaturesToTrack(tmp, corners_next, maxCorners, newQuality, newDistance, Mat(), 3, 0, false, 0.04);
		}

		if (status[i] == 1 && !err[i] == 0)
			line(corner_frame, corners_prev[i], corners_next[i], Scalar(0, 255, 0), 1);
	}
	
	frame_prev = frame_next.clone();

	if (counter % 5 == 0) corners_prev.clear();
	corners_prev = corners_next;
	if (counter % 5 == 0) corners_next.clear();
	counter++;

	putTrackbarRealValuesOnFrame(newQuality, newDistance);
}

void ValueChange(int, void*) {
	ValueChanger();
}

int main() {
	cap.open(0);
	//cap.open("C:/Users/Mariusz/Desktop/opencv_tmp/lab4/bike.avi");

	namedWindow("window", CV_WINDOW_AUTOSIZE);
	namedWindow("motion", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	createTrackbar("maxCorners", "motion", &maxCorners, maxCorners_max, ValueChange);
	createTrackbar("qualityLevel", "motion", &qualityLevel, qualityLevel_max, ValueChange);
	createTrackbar("minDistance", "motion", &minDistance, minDistance_max, ValueChange);

	while (1) {
		try {
			cap >> frame_next;
			if (frame_next.empty()) {
				cap.set(CAP_PROP_POS_FRAMES, 0);
				cap >> frame_next;
			}
			corner_frame = frame_next.clone();

			ValueChanger();

			imshow("window", frame_next);
			imshow("motion", corner_frame);
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