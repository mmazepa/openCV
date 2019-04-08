#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>
#include<ctime>

using namespace cv;
using namespace std;

Mat frame, frame_hsv, frame_threshold;
Mat frame_with_contours;

vector<vector<Point>> contours;
vector<Vec4i> hierarchy;

//pomaranczowy / ¿ó³ty
//	hmin 0  hmax 41  smin 116 smax 226 vmin 171 vmax 219
//zielony
//	hmin 34 hmax 82  smin 65  smax 253 vmin 75  vmax 154
//niebieski
//	hmin 89 hmax 147 smin 144 smax 318 vmin  45 vmax 195

int hsv_h_min = 34;
int hsv_h_max = 82;
int hsv_s_min = 65;
int hsv_s_max = 253;
int hsv_v_min = 75;
int hsv_v_max = 154;
const int hsv_max_value = 360;

void drawAim() {
	Moments m = moments(frame_threshold, true);
	Point center = Point(m.m10 / m.m00, m.m01 / m.m00);

	Point center_p1 = Point(m.m10 / m.m00 - 15, m.m01 / m.m00);
	Point center_p2 = Point(m.m10 / m.m00 + 15, m.m01 / m.m00);
	Point center_p3 = Point(m.m10 / m.m00, m.m01 / m.m00 - 15);
	Point center_p4 = Point(m.m10 / m.m00, m.m01 / m.m00 + 15);

	circle(frame_with_contours, center, 30, (0, 255, 0), 3);
	line(frame_with_contours, center_p1, center_p2, (0, 255, 0), 3);
	line(frame_with_contours, center_p3, center_p4, (0, 255, 0), 3);

	Point center_p5 = Point(m.m10 / m.m00, m.m01 / m.m00 - 30);
	putText(frame_with_contours, format("(%d,%d)", center.x, center.y), center_p5, FONT_HERSHEY_DUPLEX, 1.0, (0, 255, 0));
}

void changeValues(int hsv_h_min2, int hsv_h_max2, int hsv_s_min2, int hsv_s_max2, int hsv_v_min2, int hsv_v_max2) {
	hsv_h_min = hsv_h_min2;
	hsv_h_max = hsv_h_max2;
	hsv_s_min = hsv_s_min2;
	hsv_s_max = hsv_s_max2;
	hsv_v_min = hsv_v_min2;
	hsv_v_max = hsv_v_max2;
}

void HSV_ValueChanger() {
	//pomaranczowy / ¿ó³ty
	//	hmin 0  hmax 41  smin 116 smax 226 vmin 171 vmax 219
	//zielony
	//	hmin 34 hmax 82  smin 65  smax 253 vmin 75  vmax 154
	//niebieski
	//	hmin 89 hmax 147 smin 144 smax 318 vmin  45 vmax 195

	time_t current_time;
	current_time = time(NULL);

	if (current_time%15 < 5) changeValues(78, 208, 137, 241, 119, 203);
	else if (current_time%15 < 10) changeValues(34, 82, 65, 253, 75, 154);
	else if (current_time % 15 < 15) changeValues(25, 236, 141, 292, 112, 140);

	inRange(frame_hsv, Scalar(hsv_h_min, hsv_s_min, hsv_v_min), Scalar(hsv_h_max, hsv_s_max, hsv_v_max), frame_threshold);
	erode(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 2);
	dilate(frame_threshold, frame_threshold, Mat(), Point(-1, -1), 2);

	findContours(frame_threshold, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	int largest_area = 0;
	int largest_contour_index = 0;
	Rect bounding_rect;

	for (int i = 0; i < contours.size(); i++) {
		double a = contourArea(contours[i], false);
		if (a > largest_area) {
			largest_area = a;
			largest_contour_index = i;
			bounding_rect = boundingRect(contours[i]);
		}
	}

	//drawContours(frame, contours, largest_contour_index, (0, 255, 0), CV_FILLED, 8, hierarchy);
	frame_with_contours = frame.clone();

	drawAim();
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
	namedWindow("frame_with_contours", CV_WINDOW_AUTOSIZE);

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