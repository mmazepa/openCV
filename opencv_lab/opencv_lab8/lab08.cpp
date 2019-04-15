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

VideoCapture cap;

Ptr<BackgroundSubtractorMOG2> pMOG2;
Mat fgMaskMOG2;

int history = 0;
int history_max = 100;
int nmixtures = 0;
int nmixtures_max = 100;

void ValueChanger() {
	pMOG2->apply(frame, fgMaskMOG2);
	//pMOG2->setHistory(history);
	//pMOG2->setNMixtures(nmixtures);
}

void ValueChange(int, void*) {
	ValueChanger();
}

int main() {
	cap.open(0);
	//cap.open("C:/Users/Mariusz/Desktop/opencv_tmp/lab4/bike.avi");

	namedWindow("background", CV_WINDOW_AUTOSIZE);
	namedWindow("foreground", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	pMOG2 = createBackgroundSubtractorMOG2(history, 0.0, true);
	//pMOG2->setHistory(history);
	//pMOG2->setNMixtures(nmixtures);

	createTrackbar("history", "foreground", &history, history_max, ValueChange);
	createTrackbar("nmixtures", "foreground", &nmixtures, nmixtures_max, ValueChange);

	while (1) {
		try {
			cap >> frame;
			frame_with_lines = frame.clone();

			ValueChanger();

			Canny(frame, canny_frame, 50, 255, 3);
			HoughLinesP(canny_frame, lines, 1, CV_PI / 180, 50, 50, 10);

			for (size_t i = 0; i < lines.size(); i++) {
				Point p1 = Point(lines[i][0], lines[i][1]);
				Point p2 = Point(lines[i][2], lines[i][3]);
				line(frame_with_lines, p1, p2, Scalar(0, 0, 255), 3);
			}

			imshow("background", frame_with_lines);
			imshow("foreground", fgMaskMOG2);
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