#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

int main() {
	Mat image;
	VideoCapture cap;
	cap.open(0);

	namedWindow("window", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	while (1) {
		try {
			cap >> image;
			imshow("window", image);
		}
		catch (Exception e) {
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