#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

Mat frame;

vector<Point2f> corners;
Mat corner_frame;

VideoCapture cap;

// int...
int maxCorners = 20;
int maxCorners_max = 100;
// double...
int qualityLevel = 2;
int qualityLevel_max = 100;
// double...
int minDistance = 10;
int minDistance_max = 100;

void ValueChanger() {
	Mat tmp = frame.clone();
	cvtColor(tmp, tmp, CV_RGB2GRAY);

	double newQuality = (double)qualityLevel * 5 / qualityLevel_max;
	if (newQuality == 0.0) newQuality = 0.01;
	double newDistance = (double)minDistance * 5 / minDistance_max;

	corners.clear();
	goodFeaturesToTrack(tmp, corners, maxCorners, newQuality, newDistance);
	//cout << "[" << maxCorners << ", " << newQuality << ", " << newDistance << "]" << endl;

	for (int i = 0; i < corners.size(); i++) {
		circle(corner_frame, corners[i], 5, (0, 0, 255), 3);
	}
}

void ValueChange(int, void*) {
	ValueChanger();
}

int main() {
	cap.open(0);

	namedWindow("window", CV_WINDOW_AUTOSIZE);
	namedWindow("motion", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	createTrackbar("maxCorners", "motion", &maxCorners, maxCorners_max, ValueChange);
	createTrackbar("qualityLevel", "motion", &qualityLevel, qualityLevel_max, ValueChange);
	createTrackbar("minDistance", "motion", &minDistance, minDistance_max, ValueChange);

	while (1) {
		try {
			cap >> frame;
			corner_frame = frame.clone();

			ValueChanger();

			imshow("window", frame);
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