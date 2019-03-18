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

String getPath(String folderName, String fileName, String extension) {
	String path = "C:/Users/Mariusz/Desktop/opencv_tmp/";
	return path + "/" + folderName + "/" + fileName + "." + extension;
}

String getVideoPath(String videoName) {
	return getPath(videoName, videoName, "avi");
}

String getLogoPath(String folderName, String logoName, String extension) {
	return getPath(folderName, logoName, extension);
}

Mat motionDetection(VideoCapture cap, Mat currentFrame, Mat frameToCompare, int threshold_value, int erode_value, int dilate_value) {
	Mat diff;
	absdiff(frameToCompare, currentFrame, diff);
	cvtColor(diff, diff, CV_RGB2GRAY);
	threshold(diff, diff, threshold_value, 255, THRESH_BINARY);
	erode(diff, diff, Mat(), Point(-1, -1), erode_value);
	dilate(diff, diff, Mat(), Point(-1, -1), dilate_value);
	return diff;
}

bool isFrameEmpty(VideoCapture cap, Mat frame) {
	if (frame.empty()) {
		pressAnyKey();
		cleanUpOnExit(cap);
		return true;
	} else {
		return false;
	}
}

int motionDetector(VideoCapture cap, int choice) {
	Mat firstFrame, currentFrame, previousFrame;
	Mat diff;

	namedWindow("original", CV_WINDOW_AUTOSIZE);
	namedWindow("modified", CV_WINDOW_AUTOSIZE);

	if (choice == 1 || choice == 2) {
		cap.open(getVideoPath("robot_no_loop"));
	} else if (choice == 3 || choice == 4) {
		cap.open(0);
	}

	cap >> firstFrame;
	previousFrame = firstFrame.clone();

	while (1) {
		try {
			cap >> currentFrame;
			if (isFrameEmpty(cap, currentFrame)) return 0;

			if (choice == 1) diff = motionDetection(cap, currentFrame, firstFrame, 20, 3, 1);
			else if (choice == 2) diff = motionDetection(cap, currentFrame, previousFrame, 16, 1, 3);
			else if (choice == 3) diff = motionDetection(cap, currentFrame, firstFrame, 20, 3, 1);
			else if (choice == 4) diff = motionDetection(cap, currentFrame, previousFrame, 20, 3, 1);

			imshow("original", currentFrame);
			imshow("modified", diff);
			previousFrame = currentFrame.clone();
		} catch (Exception e) {
			cap.open(1);
		}

		if (waitKey(15) == 27) {
			cleanUpOnExit(cap);
			return 0;
		}
	}
	return 0;
}

int addLogo(VideoCapture cap, int choice) {
	Mat frame;
	namedWindow("window", CV_WINDOW_AUTOSIZE);

	if (choice == 5) {
		cap.open(getVideoPath("robot_no_loop"));
	} else if (choice == 6) {
		cap.open(0);
	}

	Mat logo = imread(getLogoPath("robot_no_loop", "logo", "bmp"), CV_LOAD_IMAGE_UNCHANGED);

	Mat newLogo = Mat(logo.size(), CV_MAKE_TYPE(logo.depth(), 4));
	int from_to[] = { 0,0, 1,1, 2,2, 2,3 };
	mixChannels(&logo, 1, &newLogo, 1, from_to, 4);

	cout << "Original logo channels: " << logo.channels() << endl;
	cout << "Modified logo channels: " << newLogo.channels() << endl;

	Mat mask;
	vector<Mat> rgbLayer;

	if (newLogo.channels() == 4) {
		split(newLogo, rgbLayer);
		Mat cs[3] = { rgbLayer[0], rgbLayer[1], rgbLayer[2] };
		merge(cs, 3, newLogo);
		mask = rgbLayer[3];
	}

	while (1) {
		try {
			cap >> frame;
			if (isFrameEmpty(cap, frame)) return 0;
			newLogo.copyTo(frame(Rect(frame.cols-(newLogo.cols*1.1), frame.rows-(newLogo.rows*2.0), newLogo.cols, newLogo.rows)), mask);
			imshow("window", frame);
		} catch (Exception e) {
			cap.open(1);
		}

		if (waitKey(15) == 27) {
			cleanUpOnExit(cap);
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

void menu() {
	cout << "MENU:" << endl;
	cout << "   Motion Detection" << endl;
	cout << "      1 - video (first frame)" << endl;
	cout << "      2 - video (previous frame)" << endl;
	cout << "      3 - camera (first frame)" << endl;
	cout << "      4 - camera (previous frame)" << endl;
	cout << "   Add Logo" << endl;
	cout << "      5 - video (add logo)" << endl;
	cout << "      6 - camera (add logo)" << endl;
	cout << "   Other options" << endl;
	cout << "      7 - exit" << endl;
	cout << endl;
}

int main() {
	VideoCapture cap;

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	menu();
	int choice = askForNumber("Your choice:");

	if (choice < 5) {
		motionDetector(cap, choice);
	} else if (choice < 7) {
		addLogo(cap, choice);
	} else if (choice == 7) {
		destroyAllWindows();
		cout << "Good-bye!" << endl;
		pressAnyKey();
		return 0;
	} else {
		cout << "Unknown choice." << endl;
		pressAnyKey();
		return 0;
	}
	return 0;
}