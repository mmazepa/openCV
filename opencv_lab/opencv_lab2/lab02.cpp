#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<conio.h>

using namespace cv;
using namespace std;

string int2str(int num) {
	string s;
	stringstream ss(s);
	ss << num;
	return ss.str();
}

void cleanOnExit(VideoCapture cap) {
	cap.release();
	destroyAllWindows();
}

Mat modifyColor(Mat negative, int x, int y) {
	Vec3b color = negative.at<Vec3b>(Point(x, y));
	color[0] = 255 - color[0];
	color[1] = 255 - color[1];
	color[2] = 255 - color[2];
	negative.at<Vec3b>(Point(x, y)) = color;
	return negative;
}

int negativeCap() {
	Mat negative;
	VideoCapture cap;
	cap.open(0);

	namedWindow("window", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	while (1) {
		try {
			cap >> negative;
			for (int y = 0; y < negative.rows; y++) {
				for (int x = 0; x < negative.cols; x++) {
					negative = modifyColor(negative, x, y);
				}
			}
			imshow("window", negative);
		}
		catch (Exception e) {
			cap.open(1);
		}
		if (waitKey(15) == 27) {
			cleanOnExit(cap);
			return 0;
		}
	}
	return 0;
}

int askForNumber(String question) {
	int number;
	cout << question << endl;
	cin >> number;
	return number;
}

void pressAnyKey() {
	cout << "Press ANY key to exit..." << endl;
	_getch();
}

void framesSavedInfo(int framesAmount, String framesPath) {
	cout << int2str(framesAmount) + " frames saved to " + framesPath << endl;
	pressAnyKey();
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

int videoToFrames() {
	int framesAmount = askForNumber("Number of frames:");
	int milliseconds = askForNumber("Number of milliseconds:");

	String path = "C:/Users/Mariusz/Desktop/opencv_tmp/snake_synth/";
	String videoPath = path + "snake_synth.avi";
	String framesPath = path + "klatki/";

	VideoCapture cap(videoPath);
	Mat frame;

	cap.set(CV_CAP_PROP_POS_MSEC, milliseconds);
	saveFramesLoop(framesAmount, cap, frame, framesPath);
	framesSavedInfo(framesAmount, framesPath);

	cleanOnExit(cap);
	return 0;
}

int cameraToFrames() {
	int framesAmount = askForNumber("Number of frames:");
	int milliseconds = askForNumber("Number of milliseconds:");

	String path = "C:/Users/Mariusz/Desktop/opencv_tmp/snake_synth/";
	String framesPath = path + "klatki/";

	VideoCapture cap(0);
	Mat frame;

	cout << "You have to wait for " + int2str(milliseconds) + " milliseconds." << endl;
	waitKey(milliseconds);
	saveFramesLoop(framesAmount, cap, frame, framesPath);
	framesSavedInfo(framesAmount, framesPath);

	cleanOnExit(cap);
	return 0;
}

void menu() {
	cout << "MENU:" << endl;
	cout << "1 - negative" << endl;
	cout << "2 - video to frames" << endl;
	cout << "3 - camera to frames" << endl;
	cout << "4 - exit" << endl;
	cout << endl;
}

int main() {
	menu();
	int choice = askForNumber("Your choice:");
	switch (choice) {
	case 1:
		negativeCap();
		break;
	case 2:
		videoToFrames();
		break;
	case 3:
		cameraToFrames();
		break;
	case 4:
		destroyAllWindows();
		cout << "Good-bye!" << endl;
		pressAnyKey();
		break;
	default:
		cout << "Unknown choice." << endl;
		pressAnyKey();
		break;
	}
	return 0;
}