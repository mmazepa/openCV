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
int highThreshold = 90;
const int max_highThreshold = 255;
//const int ratio = 3;
const int kernel_size = 3;

const char* window_name = "Original";
const char* window_name2 = "Colorized";
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
}

void CannyThreshold(int, void*) {
	CannyThresholdChange();
}

int main() {
	VideoCapture cap;
	cap.open(0);

	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name2, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name3, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name4, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name5, CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	createTrackbar("Min Threshold:", window_name2, &lowThreshold, max_lowThreshold, CannyThreshold);
	createTrackbar("Max Threshold:", window_name2, &highThreshold, max_highThreshold, CannyThreshold);

	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_32F;

	cap >> frame;
	Mat image = Mat::zeros(frame.rows, frame.cols, CV_8UC3);

	while (1) {
		try {
			cap >> frame;
			dst.create(frame.size(), frame.type());
			imshow(window_name, frame);		
			CannyThresholdChange();

			GaussianBlur(frame_gray, frame_gray, Size(7, 7), 0, 0, BORDER_DEFAULT);

			Sobel(frame_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
			Sobel(frame_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);

			convertScaleAbs(grad_x, abs_grad_x);
			convertScaleAbs(grad_y, abs_grad_y);

			Mat orientation = Mat::zeros(image.rows, image.cols, CV_32F);
			phase(grad_x, grad_y, orientation, true);

			Mat res_gx, res_gy;
			threshold(abs_grad_x, res_gx, lowThreshold, 255, THRESH_BINARY);
			threshold(abs_grad_y, res_gy, lowThreshold, 255, THRESH_BINARY);

			Vec3b col1 = Vec3b(255, 255, 255);
			Vec3b col2 = Vec3b(255, 0, 0);
			Vec3b col3 = Vec3b(0, 255, 0);
			Vec3b col4 = Vec3b(0, 0, 255);
			Vec3b col5 = Vec3b(0, 0, 0);

			for (int y = 0; y < orientation.rows; y++) {
				for (int x = 0; x < orientation.cols; x++) {	
					double orient = (double)orientation.at<float>(Point(x, y));

					if (orient > 45 && orient <= 135) image.at<Vec3b>(Point(x, y)) = col1;
					else if (orient > 135 && orient <= 225) image.at<Vec3b>(Point(x, y)) = col2;
					else if (orient > 225 && orient <= 315) image.at<Vec3b>(Point(x, y)) = col3;
					else if (orient > 315 && orient <= 360) image.at<Vec3b>(Point(x, y)) = col4;
					else image.at<Vec3b>(Point(x, y)) = col5;
				}
			}

			imshow(window_name3, image);
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