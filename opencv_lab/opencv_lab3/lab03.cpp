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

int main() {
	Mat firstFrame, currentFrame, previousFrame;
	Mat diff, eroded, dilated;
	Mat gray, prev_gray;

	VideoCapture cap;
	cap.open(getPath("robot_no_loop"));

	namedWindow("window", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	cap >> firstFrame;
	currentFrame = firstFrame;
	previousFrame = currentFrame;
	diff = currentFrame;

	while (1) {
		try {
			cap >> currentFrame;
			if (currentFrame.empty()) {
				pressAnyKey();
				cleanUpOnExit(cap);
				return 0;
			}

			cap >> diff;
			cvtColor(currentFrame, gray, CV_RGB2GRAY);
			cvtColor(previousFrame, prev_gray, CV_RGB2GRAY);
			
			absdiff(gray, prev_gray, diff);
			threshold(gray, diff, 40, 255, THRESH_BINARY);
			erode(diff, eroded, Mat(), Point(-1,-1), 5);
			dilate(eroded, dilated, Mat(), Point(-1,-1), 3);

			imshow("window", dilated);
			previousFrame = currentFrame;
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