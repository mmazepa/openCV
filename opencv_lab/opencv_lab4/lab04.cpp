#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

VideoCapture cap;

double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

const int threshold_max = 255;
int threshold_value = 50;

const int alpha_max = 100;
int alpha = 30;

string thresholdTrackBarName = "Threshold";
string alphaTrackBarName = "Alpha";

void on_threshold_trackbar(int, void*) {
	threshold_value = getTrackbarPos(thresholdTrackBarName, "modified");
}

void on_alpha_trackbar(int, void*) {
	alpha = getTrackbarPos(alphaTrackBarName, "modified");
}

void cleanUpOnExit(VideoCapture cap) {
	cap.release();
	destroyAllWindows();
}

void pressAnyKey() {
	cout << "The end, press ANY key to exit..." << endl;
	_getch();
}

Mat motionDetection(VideoCapture cap, Mat currentFrame, Mat frameToCompare, Mat accumulator, int threshold_value, double alpha) {
	Mat diff;	

	absdiff(currentFrame, frameToCompare, diff);
	cvtColor(diff, diff, CV_RGB2GRAY);
	threshold(diff, diff, threshold_value, 255, THRESH_BINARY);	
	accumulateWeighted(diff, accumulator, alpha);
	convertScaleAbs(accumulator, diff);
	
	return diff;
}

Mat playInALoop(Mat currentFrame) {
	cap.set(CV_CAP_PROP_POS_FRAMES, 1);
	cap >> currentFrame;
	return currentFrame;
}

void exitOnVideoEnd() {
	pressAnyKey();
	cleanUpOnExit(cap);
}

int movingAverage(String capture) {
	Mat firstFrame, currentFrame;
	Mat diff;

	if (capture == "camera") {
		cap.open(0);
	} else {
		String path = "C:/Users/Mariusz/Desktop/opencv_tmp/lab4/";
		cap.open(path + capture);
	}

	namedWindow("original", CV_WINDOW_AUTOSIZE);
	namedWindow("modified", CV_WINDOW_AUTOSIZE);

	cap >> firstFrame;
	currentFrame = firstFrame.clone();

	Mat accumulator = Mat::zeros(firstFrame.size(), CV_64FC1);

	createTrackbar(thresholdTrackBarName, "modified", &threshold_value, threshold_max, on_threshold_trackbar);
	createTrackbar(alphaTrackBarName, "modified", &alpha, alpha_max, on_alpha_trackbar);

	while (1) {
		try {
			cap >> currentFrame;
			if (currentFrame.empty()) {
				currentFrame = playInALoop(currentFrame);
				//exitOnVideoEnd(); break;
			}
			double alpha_value = (double)alpha / alpha_max;
			diff = motionDetection(cap, currentFrame, firstFrame, accumulator, threshold_value, alpha_value);
			imshow("original", currentFrame);
			imshow("modified", diff);
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

string int2str(int num) {
	string s;
	stringstream ss(s);
	ss << num;
	return ss.str();
}

void framesSavedInfo(int framesAmount, String framesPath) {
	cout << int2str(framesAmount) + " frames saved to " + framesPath << endl;
	//pressAnyKey();
}

void videoSavedInfo(String videoName, String videoPath) {
	cout << "\"" + videoName + "\"" + " saved to " + videoPath << endl;
	//pressAnyKey();
}

vector<int> setCompressionParams() {
	vector<int> compression_params;
	compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	return compression_params;
}

void saveFramesLoop(int framesAmount, VideoCapture cap, Mat frame, String framesPath) {
	for (int i = 1; i <= framesAmount; i++) {
		cap >> frame;
		imwrite(framesPath + "Frame" + int2str(i) + ".jpg",
			frame, setCompressionParams());
		cout << "Frame" + int2str(i) + ".jpg" << endl;
	}
}

int askForNumber(String question) {
	int number;
	cout << question << endl;
	cin >> number;
	return number;
}

int videoToFrames(String capture, int framesAmount) {
	Mat frame;

	String path = "C:/Users/Mariusz/Desktop/opencv_tmp/lab4/";
	String framesPath = path + "klatki/";

	if (capture == "camera") {
		cap.open(0);
	} else {
		cap.open(path + capture);
	}
	
	saveFramesLoop(framesAmount, cap, frame, framesPath);
	framesSavedInfo(framesAmount, framesPath);

	cleanUpOnExit(cap);
	return 0;
}

int framesToVideo(String outputVideoName, int framesAmount) {
	String path = "C:/Users/Mariusz/Desktop/opencv_tmp/lab4/";
	String framesPath = path + "klatki/";
	
	VideoWriter outputVideo;

	String outputPath = framesPath + outputVideoName;
	int codec = CV_FOURCC('M', 'J', 'P', 'G');
	int fps = 15;
	double width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	Size size = Size(width, height);
	
	outputVideo.open(outputPath, codec, fps, size, true);

	for (int i = 0; i < framesAmount; i++) {
		Mat frame = imread(framesPath + "Frame" + int2str(i+1) + ".jpg", CV_LOAD_IMAGE_UNCHANGED);
		outputVideo.write(frame);
	}
	outputVideo.release();
	videoSavedInfo(outputVideoName, framesPath);

	cleanUpOnExit(cap);
	return 0;
}

int videoToFramesAndBack(String capture, int framesAmount) {
	videoToFrames(capture, framesAmount);
	framesToVideo("aVideo.avi", framesAmount);
	pressAnyKey();
	return 0;
}

void menu() {
	cout << "MENU:" << endl;
	cout << "   Motion Detection" << endl;
	cout << "      1 - video (bike.avi)" << endl;
	cout << "      2 - video (robot_no_loop.avi)" << endl;
	cout << "      3 - camera" << endl;
	cout << "   Images to Video" << endl;
	cout << "      4 - video (bike.avi)" << endl;
	cout << "      5 - video (robot_no_loop.avi)" << endl;
	cout << "      6 - camera" << endl;
	cout << "   Other options" << endl;
	cout << "      7 - exit" << endl;
	cout << endl;
}

int main() {
	menu();
	int choice = askForNumber("Your choice:");
	int framesAmount;

	switch (choice) {
		case 1:
			movingAverage("bike.avi");
			break;
		case 2:
			movingAverage("robot_no_loop.avi");
			break;
		case 3:
			movingAverage("camera");
			break;
		case 4:
			framesAmount = askForNumber("Number of frames:");
			videoToFramesAndBack("bike.avi", framesAmount);
			break;
		case 5:
			framesAmount = askForNumber("Number of frames:");
			videoToFramesAndBack("robot_no_loop.avi", framesAmount);
			break;
		case 6:
			framesAmount = askForNumber("Number of frames:");
			videoToFramesAndBack("camera", framesAmount);
			break;
		case 7:
			cleanUpOnExit(cap);
			cout << "Good-bye!" << endl;
			pressAnyKey();
			break;
		default:
			cleanUpOnExit(cap);
			cout << "Unknown choice." << endl;
			pressAnyKey();
			break;
	}

	return 0;
}