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
	GaussianBlur(thresh, thresh, Size(0, 0), 3);
	threshold(thresh, thresh, 50, 255, CV_THRESH_BINARY);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(thresh, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	return (contours.size() > 0);
}

VideoWriter initializeVideo(string outputVideoName, VideoWriter outputVideo) {
	string path = "C:/Users/Mariusz/Desktop/opencv_tmp/lab12/";
	string outputPath = path + outputVideoName;
	int codec = CV_FOURCC('M', 'J', 'P', 'G');
	Size size = Size(640, 480);
	outputVideo.open(outputPath, codec, 15, size, true);
	return outputVideo;
}

int recordVideo(VideoWriter outputVideo, Mat frameToDisplay) {
	if (movement) outputVideo.write(frameToDisplay);
	return 0;
}

string prepareDateTime(string purpose) {
	SYSTEMTIME lt;
	GetLocalTime(&lt);
	string locDate, locTime, locDateTime;
	if (purpose == "frame") {
		locDate = format("%02d.%02d.%04d", lt.wDay, lt.wMonth, lt.wYear);
		locTime = format("%02d:%02d:%02d", lt.wHour, lt.wMinute, lt.wSecond);
		locDateTime = locDate + " " + locTime;
	} else if (purpose == "filename") {
		locDate = format("%02d-%02d-%04d", lt.wDay, lt.wMonth, lt.wYear);
		locTime = format("%02dh%02dm%02ds", lt.wHour, lt.wMinute, lt.wSecond);
		locDateTime = locDate + "_" + locTime;
	}
	return locDateTime;
}

int main() {
	cap.open(0);
	namedWindow("window", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	cap >> frame1;
	frame2 = frame1.clone();
	VideoWriter outputVideo;

	int choice = 2;

	while (1) {
		try {
			cap >> frame1;

			movement = motionDetection(frame1, frame2, thresh);
			if (movement == 0) cout << "..." << endl;
			else cout << "motion detected!" << endl;

			frameToDisplay = frame1.clone();
		
			string datetimeFrame = prepareDateTime("frame");
			string datetimeFile = prepareDateTime("filename");

			putText(frameToDisplay, datetimeFrame, Point(15, 465), FONT_HERSHEY_PLAIN, 1.25, Scalar(0, 255, 0), 2);
			string videoName = "video_" + datetimeFile + ".avi";

			Mat frameStatus = frameToDisplay.clone();

			if (GetAsyncKeyState('1') || (!outputVideo.isOpened() && choice < 3)) {
				outputVideo = initializeVideo(videoName, outputVideo);
				frameStatus.setTo(Scalar(255, 255, 255));
			}
			if (GetAsyncKeyState('2')) {
				choice = 2;
			}
			if (GetAsyncKeyState('3')) {
				choice = 3;
			}
			if (GetAsyncKeyState('4')) {
				choice = 4;
				outputVideo.release();
			}

			if (choice == 2)
				recordVideo(outputVideo, frameToDisplay);

			string currentState;
			if (choice == 2) {
				currentState = "Recording...";
			} else if (choice == 3) {
				currentState = "Paused!";
			} else if (choice == 4) {
				currentState = "Stopped!";
			}

			putText(frameStatus, currentState, Point(15, 25), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 2);

			imshow("window", frameStatus);
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