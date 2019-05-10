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
int scaleFactor = 5;
int scaleFactor_max = 100;

// int...
int minNeighbors = 5;
int minNeighbors_max = 100;

// size...
int minSize = 5;
int minSize_max = 100;

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

void putTrackbarRealValuesOnFrame() {
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
	textWithShadow("Choice:", Point(5, 15), Scalar(155, 0, 0));
	textWithShadow(mode, Point(75, 15), Scalar(0, 100, 0));
}

int choiceChange() {
	if (GetAsyncKeyState('1') & 0x8000)
		choice = 1;
	else if (GetAsyncKeyState('2') & 0x8000)
		choice = 2;
	else if (GetAsyncKeyState('3') & 0x8000)
		choice = 3;
	else if (GetAsyncKeyState('4') & 0x8000)
		choice = 4;
	return choice;
}

void ValueChanger() {
	vector<Rect> faces;
	face_cascade.detectMultiScale(gray, faces);
	//face_cascade.detectMultiScale(gray, faces, (double)scaleFactor, minNeighbors, 0, Size(minSize, minSize));

	for (int i = 0; i < faces.size(); i++) {
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		int face_radius = cvRound((faces[i].width + faces[i].height)*0.25);

		if (choice == 1)
			circle(frame, center, face_radius, Scalar(0, 100, 0), 3);

		if (choice == 2)
			GaussianBlur(frame(faces[i]), frame(faces[i]), Size(0, 0), 5);

		Mat faceROI = gray(faces[i]);
		vector<Rect> eyes;
		eyes_cascade.detectMultiScale(faceROI, eyes);
		//eyes_cascade.detectMultiScale(faceROI, eyes, (double)scaleFactor, minNeighbors, 0, Size(minSize, minSize));
	
		if (choice == 1) {
			for (int j = 0; j < eyes.size(); j++) {
				Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
				int eye_radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(frame, eye_center, eye_radius, Scalar(0, 0, 200), 3);
			}
		}

		if (choice == 3) {
			if (eyes.size() == 2) {
				Point eye_center1(faces[i].x + eyes[0].x + eyes[0].width / 2, faces[i].y + eyes[0].y + eyes[0].height / 2);
				Point eye_center2(faces[i].x + eyes[1].x + eyes[1].width / 2, faces[i].y + eyes[1].y + eyes[1].height / 2);

				Point bottomLeft(min(eye_center1.x, eye_center2.x)*0.8, min(eye_center1.y, eye_center2.y)*0.9);
				Point topRight(max(eye_center1.x, eye_center2.x)*1.2, max(eye_center1.y, eye_center2.y)*1.1);

				Rect censored_black = Rect(bottomLeft, topRight);
				rectangle(frame, censored_black, Scalar(0, 0, 0), CV_FILLED);
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

	double dWidth = cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	double dHeight = cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	//createTrackbar("scaleFactor", "window", &scaleFactor, scaleFactor_max, ValueChange);
	//createTrackbar("minNeighbors", "window", &minNeighbors, minNeighbors_max, ValueChange);
	//createTrackbar("minSize", "window", &minSize, minSize_max, ValueChange);

	while (1) {
		try {
			cap >> frame;
			cvtColor(frame, gray, CV_RGB2GRAY);
			equalizeHist(gray, gray);

			choiceChange();
			putTrackbarRealValuesOnFrame();
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