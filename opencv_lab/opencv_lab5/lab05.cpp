#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

Mat frame, frame_gray;
Mat dst, detected_edges;
int lowThreshold = 50;
const int max_lowThreshold = 100;
int highThreshold = 150;
const int max_highThreshold = 255;
//const int ratio = 3;
const int kernel_size = 3;

const char* window_name = "window";
const char* window_name2 = "Modified";
const char* window_name3 = "Grad";
const char* window_name4 = "Grad_X";
const char* window_name5 = "Grad_Y";

Mat grad_x, grad_y;
Mat abs_grad_x, abs_grad_y;


void CannyThresholdChange() {
	cvtColor(frame, frame_gray, CV_RGB2GRAY);
	GaussianBlur(frame_gray, detected_edges, Size(3, 3), 0, 0, BORDER_DEFAULT);
	Canny(detected_edges, detected_edges, lowThreshold, highThreshold, kernel_size);
	imshow(window_name2, detected_edges);

	//dst = Scalar::all(0);
	//frame.copyTo(dst, detected_edges);
	//imshow(window_name, dst);
}

void CannyThreshold(int, void*) {
	CannyThresholdChange();
}

int main() {
	VideoCapture cap;
	cap.open(0);

	namedWindow("original", CV_WINDOW_AUTOSIZE);
	//namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name2, CV_WINDOW_AUTOSIZE);
	//namedWindow(window_name3, CV_WINDOW_AUTOSIZE);
	//namedWindow(window_name4, CV_WINDOW_AUTOSIZE);
	//namedWindow(window_name5, CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	createTrackbar("Min Threshold:", window_name2, &lowThreshold, max_lowThreshold, CannyThreshold);
	createTrackbar("Max Threshold:", window_name2, &highThreshold, max_highThreshold, CannyThreshold);

	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	while (1) {
		try {
			cap >> frame;
			dst.create(frame.size(), frame.type());
			imshow("original", frame);		
			CannyThresholdChange();

			Sobel(frame_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
			convertScaleAbs(grad_x, abs_grad_x);
			
			Sobel(frame_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
			convertScaleAbs(grad_y, abs_grad_y);

			//addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

			Mat res_gx, res_gy;
			threshold(abs_grad_x, res_gx, lowThreshold, 255, THRESH_BINARY);
			threshold(abs_grad_y, res_gy, lowThreshold, 255, THRESH_BINARY);
			addWeighted(res_gx, 0.5, res_gy, 0.5, 0, grad);

			imshow(window_name3, grad);
			imshow(window_name4, res_gx);
			imshow(window_name5, res_gy);
		}
		catch (Exception e) {
			cap.open(1);
		}
		if (waitKey(15) == 27) {
			cap.release();
			destroyAllWindows();
			CannyThreshold(0, 0);
			return 0;
		}
	}
	return 0;
}