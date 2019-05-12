#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/objdetect/objdetect.hpp>
#include<iostream>
#include<conio.h>
#include<windows.h>

using namespace cv;
using namespace std;

Mat frame, gray;
VideoCapture cap;

int choice = 1;

String path = "M:/Programy/OpenCV/opencv/sources/data/haarcascades/";
CascadeClassifier face_cascade = CascadeClassifier(path + "haarcascade_frontalface_default.xml");
CascadeClassifier eyes_cascade = CascadeClassifier(path + "haarcascade_eye_tree_eyeglasses.xml");

// double...
int scaleFactor = 15;
int scaleFactor_max = 100;

// int...
int minNeighbors = 3;
int minNeighbors_max = 10;

// size...
int minSize = 50;
int minSize_max = 250;

double prepareDoubleValue(int min, int max, int ratio) {
	double val = (double)min / max;
	return val * ratio;
}

double guardRange(double value) {
	if (value <= 1.0) value = 1.1;
	return value;
}

void textWithShadow(string text, Point point, Scalar shadowColor) {
	Point shadow1 = Point(point.x + 1, point.y + 1);
	Point shadow2 = Point(point.x + 1, point.y - 1);
	Point shadow3 = Point(point.x - 1, point.y + 1);
	Point shadow4 = Point(point.x - 1, point.y - 1);
	putText(frame, text, shadow1, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(frame, text, shadow2, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(frame, text, shadow3, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(frame, text, shadow4, FONT_HERSHEY_PLAIN, 1.0, shadowColor);
	putText(frame, text, point, FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255));
}

void putTrackbarRealValuesOnFrame(double scaleFactor, int minNeighbors, Size minSize) {
	string sf_text = "Scale factor: " + format(": %.5g", scaleFactor);
	string mn_text = "Min Neighbors: " + to_string(minNeighbors);
	string ms_text = "Min Size: Size(" + to_string(minSize.width) + "," + to_string(minSize.height) + ")";

	textWithShadow(sf_text, Point(5, 30), Scalar(150, 0, 0));
	textWithShadow(mn_text, Point(5, 45), Scalar(0, 100, 0));
	textWithShadow(ms_text, Point(5, 60), Scalar(0, 0, 150));
}

void putChoiceOnFrame() {
	string mode;
	switch (choice) {
		case 1:
			mode = "Face Detection";
			break;
		case 2:
			mode = "Gaussian Blur";
			break;
		case 3:
			mode = "Black Rectangle";
			break;
		case 4:
			mode = "Face Copying";
			break;
	}
	textWithShadow("Choice:", Point(5, 15), Scalar(100, 100, 100));
	textWithShadow(mode, Point(75, 15), Scalar(150, 0, 150));
}

int choiceChange() {
	if (GetAsyncKeyState('1'))
		choice = 1;
	else if (GetAsyncKeyState('2'))
		choice = 2;
	else if (GetAsyncKeyState('3'))
		choice = 3;
	else if (GetAsyncKeyState('4'))
		choice = 4;
	return choice;
}

void ValueChanger() {
	vector<Rect> faces;
	double newScaleFactor = guardRange(prepareDoubleValue(scaleFactor, scaleFactor_max, 10));
	Size newMinSize = Size(minSize, minSize);
	putTrackbarRealValuesOnFrame(newScaleFactor, minNeighbors, newMinSize);

	face_cascade.detectMultiScale(gray, faces, newScaleFactor, minNeighbors, 0, newMinSize);

	for (int i = 0; i < faces.size(); i++) {
		if (choice == 1) {
			Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
			int face_radius = cvRound((faces[i].width + faces[i].height)*0.25);
			circle(frame, center, face_radius, Scalar(0, 100, 0), 3);
		}

		Mat faceROI = gray(faces[i]);
		vector<Rect> eyes;

		eyes_cascade.detectMultiScale(faceROI, eyes, newScaleFactor, minNeighbors, 0, newMinSize/2);
	
		if (choice == 1) {
			for (int j = 0; j < eyes.size(); j++) {
				Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
				int eye_radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(frame, eye_center, eye_radius, Scalar(0, 0, 200), 3);
			}
		}

		if (choice == 2)
			GaussianBlur(frame(faces[i]), frame(faces[i]), Size(0, 0), 10);

		if (choice == 3) {
			if (eyes.size() >= 2) {
				int y = faces[i].y + max(eyes[0].y, eyes[1].y);
				int height = (eyes[0].height, eyes[1].height);
				Rect censored_black = Rect(faces[i].x, y, faces[i].width, height);
				rectangle(frame, censored_black, Scalar(0, 0, 0), CV_FILLED);
			}
		}

		if (choice == 4) {
			Rect firstFace;
			if (faces.size() > 0) firstFace = faces[0];

			if (faces.size() > 1 && !firstFace.empty()) {
				Mat face = frame(firstFace);
				if (i > 0 && !face.empty()) {
					resize(face, face, Size(faces[i].width, faces[i].height), 0, 0, INTER_CUBIC);
					face.copyTo(frame(faces[i]));
				}
			}
		}
	}
}

void ValueChange(int, void*) {
	ValueChanger();
}

int main() {
	cap.open(0);

	namedWindow("window", CV_WINDOW_AUTOSIZE);

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	createTrackbar("scaleFactor", "window", &scaleFactor, scaleFactor_max, ValueChange);
	createTrackbar("minNeighbors", "window", &minNeighbors, minNeighbors_max, ValueChange);
	createTrackbar("minSize", "window", &minSize, minSize_max, ValueChange);

	while (1) {
		try {
			cap >> frame;
			if (frame.empty()) {
				cap.set(CAP_PROP_POS_FRAMES, 0);
				cap >> frame;
			}
			cvtColor(frame, gray, CV_RGB2GRAY);
			equalizeHist(gray, gray);

			choiceChange();
			putChoiceOnFrame();
			ValueChanger();

			imshow("window", frame);
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