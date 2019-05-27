#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<windows.h>

using namespace cv;
using namespace std;

Mat frame1, frame2;
Mat frameToDisplay;
Mat thresh;
VideoCapture cap;

bool movement = false;

bool motionDetection(Mat frame1, Mat frame2, Mat thresh) {
	cvtColor(frame1, frame1, CV_BGR2GRAY);
	cvtColor(frame2, frame2, CV_BGR2GRAY);
	absdiff(frame1, frame2, thresh);
	threshold(thresh, thresh, 40, 255, CV_THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(thresh, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//imshow("new", thresh);
	return (contours.size() > 0);
}

String outputPath = "";

int recordVideo(String outputVideoName, Mat frameToDisplay) {
	String path = "C:/Users/Mariusz/Desktop/opencv_tmp/lab12/";
	if (outputPath == "") outputPath = path + outputVideoName;
	
	VideoWriter outputVideo;
	int codec = CV_FOURCC('M', 'J', 'P', 'G');
	Size size = Size(640, 480);
	if (outputPath != "") outputVideo.open(outputPath, codec, 30, size, true);

	int counter = 0;

	if (movement) {
		counter = 0;
		outputVideo.write(frameToDisplay);
		//imshow("motion", frameToDisplay);
	} else if (!movement && counter <= 5) {
		counter++;
	} else if (!movement && counter > 5) {
		outputVideo.release();
		outputPath = "";
	}
	return 0;
}

int main() {
	cap.open(0);
	namedWindow("window", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	cap >> frame1;
	frame2 = frame1.clone();

	while (1) {
		try {
			cap >> frame1;

			movement = motionDetection(frame1, frame2, thresh);
			cout << movement << endl;
			frameToDisplay = frame1.clone();
		
			SYSTEMTIME lt;
			GetLocalTime(&lt);
			string locDate = format("%02d-%02d-%04d", lt.wDay, lt.wMonth, lt.wYear);
			string locTime = format("%02d-%02d-%02d", lt.wHour, lt.wMinute, lt.wSecond);

			string locDateTime = locDate + "_" + locTime;
			putText(frameToDisplay, locDateTime, Point(15, 465), FONT_HERSHEY_PLAIN, 1.25, Scalar(0, 255, 0), 2);

			recordVideo("video_" + locDateTime + ".avi", frameToDisplay);

			imshow("window", frameToDisplay);
			frame2 = frame1.clone();
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