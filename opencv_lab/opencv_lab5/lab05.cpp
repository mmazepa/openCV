#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

Mat frame, frame_gray;
Mat dst, gaussian_effect, detected_edges;

Mat grad, grad_x, grad_y;
Mat abs_grad_x, abs_grad_y;
Mat res_gx, res_gy;

Mat orientation, colorized_grad, colorized_edges;

int lowThreshold = 50;
const int max_lowThreshold = 255;

int highThreshold = 90;
const int max_highThreshold = 255;

int gaussian = 1;
const int max_gaussian = 15;

int sobel_x_mask = 1;
const int max_sobel_x_mask = 10;

int sobel_y_mask = 1;
const int max_sobel_y_mask = 10;

const int kernel_size = 3;

int scale = 1;
int delta = 0;
int ddepth = CV_32F;

const char* window_name = "TrackBars";
const char* window_name2 = "Canny";
const char* window_name3 = "Grad_X";
const char* window_name4 = "Grad_Y";
const char* window_name5 = "Grad";
const char* window_name6 = "Colorized_Grad";
const char* window_name7 = "Colorized_Edges";

char* trackbar_name = "Min Threshold";
char* trackbar_name2 = "Max Threshold";
char* trackbar_name3 = "Gaussian";
char* trackbar_name4 = "X-Mask";
char* trackbar_name5 = "Y-Mask";

Vec3b color1 = Vec3b(255, 255, 255);
Vec3b color2 = Vec3b(255, 0, 0);
Vec3b color3 = Vec3b(0, 255, 0);
Vec3b color4 = Vec3b(0, 0, 255);
Vec3b color5 = Vec3b(0, 0, 0);

int makeItOdd(int value) {
	return value * 2 + 1;
}

void SobelXMaskChange() {
	Sobel(gaussian_effect, grad_x, ddepth, 1, 0, makeItOdd(sobel_x_mask), scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);
}

void SobelXMask(int, void*) {
	SobelXMaskChange();
}

void SobelYMaskChange() {
	Sobel(gaussian_effect, grad_y, ddepth, 0, 1, makeItOdd(sobel_y_mask), scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);
}

void SobelYMask(int, void*) {
	SobelYMaskChange();
}

void GaussianFilterChange() {
	int tmpGaussian = makeItOdd(gaussian);
	GaussianBlur(frame_gray, gaussian_effect, Size(tmpGaussian, tmpGaussian), 0, 0, BORDER_DEFAULT);
}

void GaussianFilter(int, void*) {
	GaussianFilterChange();
}

void CannyThresholdChange() {
	Canny(gaussian_effect, detected_edges, lowThreshold, highThreshold, kernel_size);
}

void CannyThreshold(int, void*) {
	CannyThresholdChange();
}

bool isInRange(double value, double min, double max) {
	if (value > min && value <= max) return true;
	else return false;
}

void gradients() {
	GaussianFilterChange();

	SobelXMaskChange();
	SobelYMaskChange();

	threshold(abs_grad_x, res_gx, lowThreshold, max_highThreshold, THRESH_BINARY);
	threshold(abs_grad_y, res_gy, lowThreshold, max_highThreshold, THRESH_BINARY);
	addWeighted(res_gx, 1.0, res_gy, 1.0, 0, grad);
}

void getOrientation() {
	orientation = Mat::zeros(frame.rows, frame.cols, CV_32F);
	phase(grad_x, grad_y, orientation, true);
}

void setColor(Mat picture, int x, int y, Vec3b color) {
	picture.at<Vec3b>(Point(x, y)) = color;
}

void colorize(Mat picture, bool pixel, double orient, int x, int y) {
	if (pixel && isInRange(orient, 45, 135)) setColor(picture, x, y, color1);
	else if (pixel && isInRange(orient, 135, 225)) setColor(picture, x, y, color2);
	else if (pixel && isInRange(orient, 225, 315)) setColor(picture, x, y, color3);
	else if (pixel && isInRange(orient, 315, 360)) setColor(picture, x, y, color4);
	else setColor(picture, x, y, color5);
}

int main() {
	VideoCapture cap;
	cap.open(0);

	namedWindow(window_name, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name2, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name3, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name4, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name5, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name6, CV_WINDOW_AUTOSIZE);
	namedWindow(window_name7, CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	createTrackbar(trackbar_name, window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
	createTrackbar(trackbar_name2, window_name, &highThreshold, max_highThreshold, CannyThreshold);
	createTrackbar(trackbar_name3, window_name, &gaussian, max_gaussian, GaussianFilter);
	createTrackbar(trackbar_name4, window_name, &sobel_x_mask, max_sobel_x_mask, SobelXMask);
	createTrackbar(trackbar_name5, window_name, &sobel_y_mask, max_sobel_y_mask, SobelYMask);

	cap >> frame;
	colorized_grad = Mat::zeros(frame.rows, frame.cols, CV_8UC3);
	colorized_edges = Mat::zeros(frame.rows, frame.cols, CV_8UC3);

	while (1) {
		try {
			cap >> frame;
			dst.create(frame.size(), frame.type());

			cvtColor(frame, frame_gray, CV_RGB2GRAY);
			GaussianFilterChange();
			CannyThresholdChange();

			gradients();
			getOrientation();

			for (int y = 0; y < orientation.rows; y++) {
				for (int x = 0; x < orientation.cols; x++) {	
					double orient = (double)orientation.at<float>(Point(x, y));

					int point = grad.at<uchar>(Point(x, y));
					bool pixel = !(point < lowThreshold) && (point >= highThreshold);
					colorize(colorized_grad, pixel, orient, x, y);

					int point2 = detected_edges.at<uchar>(Point(x, y));
					bool pixel2 = !(point2 < lowThreshold) && (point2 >= highThreshold);
					colorize(colorized_edges, pixel2, orient, x, y);
				}
			}

			imshow(window_name, frame);
			imshow(window_name2, detected_edges);
			imshow(window_name3, res_gx);
			imshow(window_name4, res_gy);
			imshow(window_name5, grad);
			imshow(window_name6, colorized_grad);
			imshow(window_name7, colorized_edges);
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